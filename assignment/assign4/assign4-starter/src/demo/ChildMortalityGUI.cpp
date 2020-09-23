#include "ProblemHandler.h"
#include "../datapoint.h"
#include "TemporaryComponent.h"
#include "GUIUtils.h"
#include "ginteractors.h"
#include "hashmap.h"
#include "../pqclient.h"
#include "../utilities/CSV.h"
#include "vector.h"
#include <algorithm>
#include <functional>
#include <vector>
#include <iomanip>
#include <sstream>
#include <ctime>
#include "urlstream.h"
#include "strlib.h"
#include "gthread.h"
using namespace std;

namespace {
    const string kDataFile = "res/UN-ChildMortality.csv";
    constexpr int kNumDisplayedCountries = 7;

    const string kBackgroundColor = "#FFFFFF"; // White
    const string kForegroundColor = "#5b92e5"; // United Nations blue

    /* The mortality rates are stored as doubles, but data points require integer values.
     * We'll fix this by using this conversion factor, which scales up the real numbers
     * to a range where they could operate as integers without loss in precision.
     */
    const double kIntegerScalar = 1000.0;

    /* Header constants. */
    const string kHeaderText =
        "As part of its Millennium Development Goals, the United Nations set a goal of reducing "
        "child mortality (defined as mortality for children ages 0 - 5) by 2015 to a level "
        "two-thirds that of the levels in 1990.\n"
        "This tool plots child mortality rates by country as of 2013, the most recent year "
        "for which the United Nations has released data. You can select which collection of "
        "countries to view using the controls at the bottom of the window.\n"
        "Numbers are expressed in child mortality per 1,000 live births. Data is taken from the "
        "United Nations.";
    const string kHeaderFont = "Serif-BOLDITALIC-16";

    /* Window boundary padding. */
    const double kWindowPadding = 10;

    /* Padding between bottom of the header and top of the graph. */
    const double kHeaderPadding = 10;

    /* Height of the heading. */
    const double kHeaderHeight = 200;

    /* Spacing from the graph bounding box to the axes. */
    const double kYAxisPadding = 10;
    const double kXAxisPadding = 10;

    /* Increments to use going up the side. */
    const int kMajorTickSize  = 50;
    const int kNumMinorTicks = 4;
    const string kAxisFont = "Monospaced-BOLD-13";

    /* Max and min values for the side. */
    const int kMaxYValue      = 350;

    /* Plotted line constants. */
    const vector<string> kPlottedLineColors = {
        "#343434", // Jet
        "#B53389", // Fandango
        "#007BA7", // Cerulean
        "#008000", // Office Green
        "#FFC40C", // Mikado Yellow
        "#EE7F2D", // Princeton Orange
        "#CC0000", // Rosso Corsa
    };

    /* Legend Constants */
    const double kLegendWidth         = 300;
    const double kLegendHeight        = 200;
    const string kLegendFont          = "Serif-ITALIC-14";
    const string kLegendFontColor     = "#2D4972"; // Darker United Nations blue
    const string kLegendBorderColor   = kLegendFontColor;

    /* Loads the mortality rate data. */
    void loadData(const string& filename,
                  HashMap<string, Vector<double>>& result,
                  int& lowYear, int& highYear) {
        ifstream input(filename);
        if (!input) error("Can't open data file " + filename);

        CSV data = CSV::parse(input);

        /* First column is country name, rest are data points. */
        for (size_t row = 0; row < data.numRows(); row++) {
            Vector<double> entries;
            for (size_t col = 1; col < data.numCols(); col++) {
                entries += stringToReal(data[row][col]);
            }
            result[data[row][0]] = entries;
        }

        auto headers = data.headers();
        lowYear  = stringToInteger(headers[1]);
        highYear = stringToInteger(headers[headers.size() - 1]);
    }

    /* Problem handler that lets the user see how child mortality rates have improved
     * since 1990.
     */
    class ChildMortalityGUI: public ProblemHandler {
    public:
        ChildMortalityGUI(GWindow& window);

        void actionPerformed(GObservable* source) override;

    protected:
        void repaint(GWindow& window) override;

    private:
        /* Graphics components. */
        Temporary<GButton> mLowest;
        Temporary<GButton> mHighest;
        Temporary<GButton> mAbsoluteImprovement;
        Temporary<GButton> mRelativeImprovement;
        Temporary<GButton> mRandom;

        /* Actively-displayed countries. */
        Vector<DataPoint> mCountries;

        /* Formatter function: takes in a DataPoint and returns a human-readable string
         * explaining that data point.
         */
        using Formatter = std::function<std::string (const DataPoint &)>;
        Formatter mFormatter;

        /* CSV data. Map goes from country -> year -> value, with years being represented
         * implicitly as mBaseYear + index.
         */
        HashMap<std::string, Vector<double>> mData;
        int mBaseYear, mHighYear;

        /* Renderer for the header. We cache this to avoid recomputing it unnecessarily. */
        std::shared_ptr<TextRender> mHeader;

        /* Sets which countries are displayed to the best countries by some metric. */
        using Scorer = std::function<int (const std::string&)>;
        void chooseBestCountriesBy(Scorer scorer, Formatter formatter);

        void drawGraph(GWindow& window, const GRectangle& bounds) const;
        void drawPlot(GWindow& window, const GRectangle& bounds) const;
        void drawLegend(GWindow& window, const GRectangle& bounds) const;
    };


    ChildMortalityGUI::ChildMortalityGUI(GWindow& window) :
        mLowest             (new GButton("Lowest as of 2013"),        window, "SOUTH"),
        mHighest            (new GButton("Highest as of 2013"),       window, "SOUTH"),
        mAbsoluteImprovement(new GButton("Most Improved (absolute)"), window, "SOUTH"),
        mRelativeImprovement(new GButton("Most Improved (relative)"), window, "SOUTH"),
        mRandom             (new GButton("Random Countries"), window, "SOUTH") {

        loadData(kDataFile, mData, mBaseYear, mHighYear);

        /* Set up graphics. */
        GThread::runOnQtGuiThread([&, this] {
            mHeader = TextRender::construct(kHeaderText, {
                                                kWindowPadding, kWindowPadding,
                                                window.getCanvasWidth() - 2 * kWindowPadding, kHeaderHeight - 2 * kWindowPadding
                                            }, kForegroundColor, kHeaderFont);
        });
    }

    void ChildMortalityGUI::actionPerformed(GObservable* source) {
        if (source == mLowest) {
            /* Use the negative value of each country's mortality rate. The "largest" entries
             * here then become the numerically lowest values.
             */
            chooseBestCountriesBy([this] (const string& country) {
                return -kIntegerScalar * mData[country].back();
            }, [] (const DataPoint& point) {
                ostringstream builder;
                builder << point.name << " (" << -point.priority / kIntegerScalar << ")";
                return builder.str();
            });
        } else if (source == mHighest) {
            chooseBestCountriesBy([this] (const string& country) {
                return kIntegerScalar * mData[country].back();
            }, [] (const DataPoint& point) {
                ostringstream builder;
                builder << point.name << " (" << point.priority / kIntegerScalar << ")";
                return builder.str();
            });
        } else if (source == mAbsoluteImprovement) {
            /* Measure in the revesre direction to ensure we find the most improved. */
            chooseBestCountriesBy([this] (const string& country) {
                return kIntegerScalar * (mData[country].front() - mData[country].back());
            }, [] (const DataPoint& point) {
                ostringstream builder;
                builder << point.name << " (-" << point.priority / kIntegerScalar << ")";
                return builder.str();
            });
        } else if (source == mRelativeImprovement) {
            /* Measure in the revesre direction to ensure we find the most improved. */
            chooseBestCountriesBy([this] (const string& country) {
                double now  = mData[country].back();
                double then = mData[country].front();
                return kIntegerScalar * (then - now) / then;
            }, [this] (const DataPoint& point) {
                double now  = mData[point.name].back();
                double then = mData[point.name].front();
                double improvement = 100.0 * (then - now) / then;

                ostringstream builder;
                builder << point.name << " (-" << fixed << setprecision(2) << improvement << "%)";
                return builder.str();
            });
        } else if (source == mRandom) {
            chooseBestCountriesBy([] (const string &) {
                return rand();
            }, [] (const DataPoint& point) {
                return point.name;
            });
        }
    }

    void ChildMortalityGUI::chooseBestCountriesBy(Scorer scorer, Formatter formatter) {
        /* Form a data stream from the list of all countries that populates data points
         * with the criterion.
         */
        stringstream stream;
        for (const auto& pt: mData) {
            stream << DataPoint{ pt, scorer(pt) };
        }

        /* Find the winners. */
        mCountries = topK(stream, kNumDisplayedCountries);
        mFormatter = formatter;

        requestRepaint();
    }

    void ChildMortalityGUI::repaint(GWindow& window) {
        clearDisplay(window, kBackgroundColor);

        /* Draw the header message. */
        mHeader->draw(window);

        /* From that, compute the bounding rectangle for our graph. */
        double headerBottom = mHeader->bounds().getY() + mHeader->bounds().getHeight();
        GRectangle graphArea = {
            kWindowPadding, headerBottom + kHeaderPadding,
            window.getCanvasWidth() - 2 * kWindowPadding,
            window.getCanvasHeight() - kWindowPadding - headerBottom - kHeaderPadding
        };

        drawGraph(window, graphArea);
    }

    void ChildMortalityGUI::drawGraph(GWindow& window, const GRectangle& bounds) const {
        /* Compute the graph content area. This is the area that the lines in the graph will
         * be plotted in.
         */
        GRectangle graphContent = {
            bounds.getX() + kXAxisPadding,
            bounds.getY() + kYAxisPadding,
            bounds.getWidth() - 2 * kXAxisPadding,
            bounds.getHeight() - 2 * kYAxisPadding
        };

        drawPlot(window, graphContent);
        drawLegend(window, graphContent);
    }

    void ChildMortalityGUI::drawPlot(GWindow& window, const GRectangle& bounds) const {
        /* Assemble our axis labels. */
        vector<string> xAxisLabels = { "" }; // Don't plot first year
        for (int year = mBaseYear + 1; year <= mHighYear; ++year) {
            xAxisLabels.push_back("'" + to_string(year % 100));
        }
        vector<string> yAxisLabels;
        for (int value = 0; value <= kMaxYValue; value += kMajorTickSize) {
            yAxisLabels.push_back(to_string(value));
        }

        /* Assemble our lines to draw. */
        vector<vector<GPoint>> lines;
        for (const auto& country: mCountries) {
            vector<GPoint> line;
            for (int i = 0; i < mData[country.name].size(); i++) {
                /* Coordinates should be in [0, 0] x [1, 1]. */
                double x = double(i) / (mData[country.name].size() - 1);
                double y = mData[country.name][i] / kMaxYValue;
                line.push_back({ x, y });
            }
            lines.push_back(line);
        }

        /* Draw everything! */
        LineGraphRender::construct(lines, xAxisLabels, yAxisLabels, 0, kNumMinorTicks, bounds,
                                   kAxisFont, kAxisFont, kPlottedLineColors, kForegroundColor)->draw(window);
    }

    void ChildMortalityGUI::drawLegend(GWindow& window, const GRectangle& bounds) const {
        /* If there's nothing selected, don't draw anything. */
        if (mCountries.isEmpty()) return;

        vector<string> labels;
        for (const auto& entry: mCountries) {
            labels.push_back(mFormatter(entry));
        }

        auto legend = LegendRender::construct(labels, kPlottedLineColors, {
                                    bounds.getX() + bounds.getWidth() - kLegendWidth, bounds.getY(),
                                    kLegendWidth, kLegendHeight
                                }, kLegendFontColor, kLegendFont, kLegendBorderColor);
        window.setColor(kBackgroundColor);
        window.fillRect(legend->computedBounds());
        legend->draw(window);
    }
}

shared_ptr<ProblemHandler> makeChildMortalityGUI(GWindow& window) {
    return make_shared<ChildMortalityGUI>(window);
}
