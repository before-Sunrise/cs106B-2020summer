#include "ProblemHandler.h"
#include "../pqclient.h"
#include "GUIUtils.h"
#include "../utilities/JSON.h"
#include "urlstream.h"
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <vector>
using namespace std;

namespace {
    /* Graphics constants. The background image is a precomputed Mollweide projection of
     * the earth centered on 90 degrees West longitude.
     */
    const double kLongitudeOffset = -90;
    const string kWorldFile = "res/mollweide-90.png";
    const string kBackgroundColor = "#FFFFFF";

    /* Our intended aspect ratio */
    const double kAspectRatio = 1;

    /* Earthquake magnitues are stored as doubles, but data points require integer values.
     * We'll fix this by using this conversion factor, which scales up the real numbers
     * to a range where they could operate as integers without loss in precision.
     */
    const double kIntegerScalar = 1000.0;

    /* Number of earthquakes to display. */
    const int    kNumQuakes = 5;

    /* Description text information. */
    const double kTextPadding = 15;
    const double kMinTextHeight = 100;
    const string kDescriptionColor = "#000000";
    const string kDescriptionFont  = "SansSerif-14";
    const string kDescription =
        "This tool displays the strongest recent earthquakes reported by the US Geological Survey. "
        "You can use the controls on the side of the window to select the time interval "
        "you're interested in. This visualizer will show the " + to_string(kNumQuakes) + " "
        "strongest earthquakes within that interval.\n"
        "Remember that the earthquake magnitude scale is logarithmic. An earthquake that is one "
        "magnitude in strength higher than another releases around 32 times as much energy.";


    /* Constants for displaying earthquakes. */
    const double kRadius = 2;
    const double kMagnitudeBase = 1.7;
    const double kRadiusStep = 5;
    const double kLineThickness = 2.5;

    /* Colors to use to display earthquakes. */
    const vector<string> kEarthquakeColors = {
        "#660000", // Blood red
        "#C81D11", // Persian red
        "#E25822", // Flame
        "#ED9121", // Carrot orange
        "#FFBF00", // Amber
    };

    /* Legend parameters. */
    const double kMinLegendHeight   = 100;
    const string kLegendFontColor   = kDescriptionColor;
    const string kLegendFont        = kDescriptionFont;
    const string kLegendBorderColor = kLegendFontColor;

    /* Type representing an earthquake. */
    struct Earthquake {
        double magnitude;
        double longitude, latitude;
        std::string time;
        std::string where;
    };

    /* Given a list of earthquakes, returns a list of the k largest. */
    Vector<Earthquake> largestEarthquakesIn(const Vector<Earthquake>& quakes, int k) {
        stringstream stream;
        for (int i = 0; i < quakes.size(); i++) {
            stream << DataPoint{ to_string(i), static_cast<int>(kIntegerScalar * quakes[i].magnitude) };
        }

        /* Place the top k data points into the result. The stream returns a bunch of structs
         * with string keys corresponding to the indices we want.
         */
        Vector<Earthquake> result;
        for (auto entry: topK(stream, k)) {
            result += quakes[stoi(entry.name)];
        }
        return result;
    }

    /* Given data from the original JSON, assembles the data into an Earthquake struct. */
    Earthquake assembleQuake(double magnitude, double longitude, double latitude,
                             int64_t timeMillis, string where) {
        /* The tricky bit is that the time is expressed as a integer encoding a number of
         * milliseconds that have elapsed since the start of the epoch, and we need to
         * convert that to something human-readable.
         */
        ostringstream timeBuilder;
        time_t eventTime = timeMillis / 1000;
        timeBuilder << put_time(localtime(&eventTime), "%I:%M:%S %p on %b %d, %Y");

        return { magnitude, longitude, latitude, timeBuilder.str(), where };
    }

    /* Given a URL, returns a list of all the earthquakes extracted from the USGS JSON available
     * at that file. Updates are passed along to the specified callback.
     */
    Vector<Earthquake> earthquakesFrom(const string& url, function<void (string)> onProgress) {
        iurlstream query;
        query.setUserAgent("Data Sagas, a programming assignment for Stanford's CS106B course. Contact: htiek@cs.stanford.edu.");

        onProgress("Downloading data from the US Geological Survey...");
        query.open(url);

        if (!query) error("Oops, didn't work.");

        onProgress("Parsing data file...");
        JSON quakes = JSON::parse(query);

        onProgress("Processing data file (" + to_string(quakes["features"].size()) + " earthquakes)...");

        Vector<Earthquake> result;
        for (auto quake: quakes["features"]) {
            auto data = quake["properties"];
            auto geo  = quake["geometry"]["coordinates"];

            if (data["mag"].type() != JSON::Type::NULLPTR_T) {
                result.add(assembleQuake(data["mag"].asDouble(),
                                         geo[0].asDouble(), geo[1].asDouble(),
                                         data["time"].asInteger(),
                                         data["place"].asString()));
            }
        }

        onProgress(" ");
        return result;
    }

    /* Draws a single earthquake centered at the indicated position. */
    void drawEarthquake(GWindow& window, const Earthquake& quake, double x, double y,
                        const string& color) {
        double radius = kRadius * pow(kMagnitudeBase, quake.magnitude);

        for (double r = radius; r > 0.0; r -= kRadiusStep) {
            GOval toDraw(x - r, y - r, 2 * r, 2 * r);
            toDraw.setLineWidth(max(1.0, kLineThickness));
            toDraw.setColor(color);
            window.draw(&toDraw);
        }
    }

    void drawEarthquakes(GWindow& window, const Vector<Earthquake>& quakes, const GRectangle& projection) {
        for (int i = 0; i < quakes.size(); i++) {
            auto coords = mollweideProjectionOf(quakes[i].latitude, quakes[i].longitude,
                                                kLongitudeOffset);

            /* Scale everything to window coordinates. Remember that the y axis is inverted in
             * our system.
             */
            double x = (projection.getX() + projection.getWidth()  / 2.0) + get<0>(coords) * projection.getHeight() / 2.0;
            double y = (projection.getY() + projection.getHeight() / 2.0) - get<1>(coords) * projection.getHeight() / 2.0;

            drawEarthquake(window, quakes[i], x, y, kEarthquakeColors[i]);
        }
    }

    void drawDescriptiveText(GWindow& window, const Vector<Earthquake>& quakes,
                             shared_ptr<TextRender> header,
                             const GRectangle& legendBounds) {
        /* Render the header. */
        header->draw(window);

        /* If there are no earthquakes, we're done. */
        if (quakes.isEmpty()) return;

        /* Convert from Earthquakes to strings. */
        vector<string> descriptions;
        for (const auto& quake: quakes) {
            ostringstream builder;
            builder << "Magnitude " << quake.magnitude << " "
                    << quake.where << " at " << quake.time;
            descriptions.push_back(builder.str());
        }

        /* Render the legend. */
        LegendRender::construct(descriptions,
                                kEarthquakeColors,
                                legendBounds,
                                kLegendFontColor,
                                kLegendFont,
                                kLegendBorderColor)->draw(window);
    }

    /* Given a window, returns a bounding rectangle within that window that fits within those bounds. */
    GRectangle boundsFor(GWindow& window) {
        double width  = window.getCanvasWidth();
        double height = window.getCanvasHeight();

        /* Aspect ratio is kAspectRatio : 1. We want to get that aspect ratio. */
        double scaledWidth, scaledHeight;
        if (width / height >= kAspectRatio) {
            scaledHeight = height;
            scaledWidth =  scaledHeight * kAspectRatio;
        } else {
            scaledWidth = width;
            scaledHeight = scaledWidth / kAspectRatio;
        }

        return {
            (width  - scaledWidth)  / 2.0,
            (height - scaledHeight) / 2.0,
            scaledWidth, scaledHeight
        };
    }

    /* Problem handler that lets the user see powerful earthquakes over time. */
    class EarthquakeGUI: public ProblemHandler {
    public:
        EarthquakeGUI(GWindow& window);

        void actionPerformed(GObservable* source) override;
        void windowResized(GWindow& window) override;

    protected:
        void repaint(GWindow& window) override;

    private:
        /* Graphics components. */
        Temporary<GButton> mHour;
        Temporary<GButton> mDay;
        Temporary<GButton> mWeek;
        Temporary<GButton> mMonth;
        Temporary<GLabel>  mStatusLine;

        /* Bounds for our display. */
        GRectangle mBounds;
        GRectangle mImageBounds;   // Space for the image; used to place earthquakes
        GRectangle mTextBounds;    // Space for the text description
        GRectangle mLegendBounds;  // Space for the legend

        /* Actively-displayed earthquakes. */
        Vector<Earthquake> mEarthquakes;

        /* Background image. */
        shared_ptr<GImage> mBackground;

        /* Descriptive text render. */
        std::shared_ptr<TextRender> mDescription;

        /* Loads earthquakes from the specified JSON endpoint. */
        void showEarthquakesFrom(const std::string& url);

        /* Recomputes the bounds of the window and graphics items based on the current
         * window size.
         */
        void recomputeBounds(GWindow& window);
    };

    EarthquakeGUI::EarthquakeGUI(GWindow& window) :
        mHour (new GButton("Past Hour"),  window, "EAST"),
        mDay  (new GButton("Past Day"),   window, "EAST"),
        mWeek (new GButton("Past Week"),  window, "EAST"),
        mMonth(new GButton("Past Month"), window, "EAST"),
        mStatusLine(new GLabel(" "), window, "SOUTH") {

        recomputeBounds(window);
    }

    void EarthquakeGUI::recomputeBounds(GWindow& window) {
        mBounds = boundsFor(window);
        mBackground = make_shared<GImage>(kWorldFile);

        /* Scale the background to fill the window as much as possible. */
        double scale = mBounds.getWidth() / mBackground->getWidth();

        /* Position and resize the background. */

        /* TODO: THIS IS WRONG! This is patching over an issue where GImage scaling impacts the
         * translation in addition to the width/height. Once this is fixed, remove the "/ scale"
         * bits from here.
         */
        mBackground->setLocation(mBounds.getX() / scale, mBounds.getY() / scale);

        /* Change the scale, undoing the past scaling operation. */
        mBackground->scale(scale);

        /* Reserve that area for the background. */
        mImageBounds = {
            mBounds.getX(), mBounds.getY(),
            mBackground->getWidth() * scale, // <-- TODO: Change once scaling issue is resolved
            mBackground->getHeight() * scale // <-- TODO: Change once scaling issue is resolved
        };

        /* Reserve space for the chart. We need a certain fixed minimum number of pixels
         * vertically for this, regardless of height, or the chart can't render. We'll therefore
         * work from the bottom up.
         */
        double legendHeight = max(kMinLegendHeight, (mBounds.getHeight() - mImageBounds.getHeight()) / 2.0);
        mLegendBounds = {
            mBounds.getX(), mBounds.getHeight() - legendHeight,
            mBounds.getWidth(), legendHeight
        };

        /* Our text goes above this, and it too has a minimum height. To make it easier to read,
         * we'll give it the full width of the window, minus some small padding.
         */
        double textHeight = max(kMinTextHeight, (mBounds.getHeight() - mImageBounds.getHeight()) / 2.0);
        mTextBounds = {
            kTextPadding, mBounds.getY() + mBounds.getHeight() - legendHeight - textHeight + kTextPadding,
            window.getCanvasWidth() - 2 * kTextPadding, textHeight - 2 * kTextPadding
        };

        /* Reserve the remaining area for the client. */
        mDescription = TextRender::construct(kDescription, mTextBounds, kDescriptionColor, kDescriptionFont);
    }

    void EarthquakeGUI::windowResized(GWindow& window) {
        recomputeBounds(window);
        requestRepaint();
    }

    void EarthquakeGUI::actionPerformed(GObservable* source) {
        if (source == mHour) {
            showEarthquakesFrom("https://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/all_hour.geojson");
        } else if (source == mDay) {
            showEarthquakesFrom("https://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/all_day.geojson");
        } else if (source == mWeek) {
            showEarthquakesFrom("https://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/all_week.geojson");
        } else if (source == mMonth) {
            showEarthquakesFrom("https://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/all_month.geojson");
        }
    }

    void EarthquakeGUI::repaint(GWindow& window) {
        /* Redraw the background. */
        clearDisplay(window, kBackgroundColor);
        window.draw(*mBackground);

        /* Draw all earthquakes. */
        drawEarthquakes(window, mEarthquakes, mImageBounds);

        /* Render the explanatory text. */
        drawDescriptiveText(window, mEarthquakes, mDescription, mLegendBounds);
    }

    void EarthquakeGUI::showEarthquakesFrom(const string& url) {
        mEarthquakes = largestEarthquakesIn(earthquakesFrom(url, [this] (const string& message) {
            this->mStatusLine->setText(message);
        }), kNumQuakes);
        requestRepaint();
    }
}

shared_ptr<ProblemHandler> makeEarthquakeGUI(GWindow& window) {
    return make_shared<EarthquakeGUI>(window);
}
