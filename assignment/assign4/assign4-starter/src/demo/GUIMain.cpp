/**
 * GUIMain.cpp
 *
 * @author Keith Schwarz
 * @version 2020/3/5
 *    Keith final revision from end of quarter 19-2
 * minor changes for Spring 20202 juliez
 */
#include "ProblemHandler.h"
#include "EmptyProblemHandler.h"
#include "gwindow.h"
#include "gevents.h"
#include "gbutton.h"
#include "goptionpane.h"
#include "map.h"
#include <chrono>
using namespace std;

namespace {
    /* Constructor type: Given a GWindow, build a problem handler. */
    using Constructor = std::function<std::shared_ptr<ProblemHandler> (GWindow &)>;

    /* An entry in the top-level GUI control bar. */
    struct GUIOption {
        std::string name;
        Constructor constructor;
    };
}

/* Use X Macro Trick to define all the menu options. */
static vector<GUIOption> kGUIOptions = {
    #define MENU_OPTION(name, constructor) {                           \
        name, [] (GWindow& window) {                                   \
            std::shared_ptr<ProblemHandler> constructor(GWindow &);    \
            return constructor(window);                                \
        }                                                              \
    },
    #define RUN_TESTS_MENU_OPTION() MENU_OPTION("Run Tests", makeTestingGUI)
    #define WINDOW_TITLE(title)

    #include "GUIConfig.h"

    #undef WINDOW_TITLE
    #undef RUN_TESTS_MENU_OPTION
    #undef MENU_OPTION
};

/* Use X Macro Trick a second time to define the window title. */
static string kWindowTitle =
    #define MENU_OPTION(name, constructor)
    #define RUN_TESTS_MENU_OPTION()
    #define WINDOW_TITLE(title) title
    #include "GUIConfig.h"
    #undef WINDOW_TITLE
    #undef GUI_HANDLER
;

namespace {

    /* Window graphics constants. */
    const double kWindowWidth  = 800;
    const double kWindowHeight = 600;

    /* Milliseconds before a timer event is considered "stale." */
    const long kTimelyCutoff = 100;

    /* Type representing all state necessary to make the graphics work. */
    struct Graphics {
        GWindow window{kWindowWidth, kWindowHeight}; // The window
        shared_ptr<ProblemHandler> handler;          // Current task handler
        Map<GObservable*, Constructor> constructors; // Map from buttons to constructors.
    };

    /* Creates the graphics window and associated state. */
    Graphics* makeGraphics() {
        auto* result = new Graphics();

        /* Basic window setup. */
        result->window.setTitle(kWindowTitle);
        result->window.setExitOnClose(false);

        /* Basic graphics setup. */
        result->window.setRepaintImmediately(false);
        result->window.setCanvasSize(kWindowWidth, kWindowHeight);

        /* Default handler. */
        result->handler = make_shared<EmptyProblemHandler>();

        /* Problem handlers. */
        for (const auto& entry: kGUIOptions) {
            auto* button = new GButton(entry.name);
            result->window.addToRegion(button, "NORTH");

            result->constructors[button] = entry.constructor;
        }
        return result;
    }

    /* Sets the active problem. */
    void setProblem(Graphics* graphics, GObservable* source) {
        auto constructor = graphics->constructors.get(source);
        if (!constructor) {
            error("No constructor for that GObservable?");
        }

        /* We do have a handler. Dispose of whatever used to be here. */
        graphics->handler.reset();
        graphics->window.clearCanvas();

        /* Now, load whatever this problem is. */
        graphics->handler.reset();
        graphics->handler = constructor(graphics->window);
    }

    Graphics* theGraphics;
    bool theOptionsEnabled = true;
}

/* Enable/disable all demo buttons. */
void setDemoOptionsEnabled(bool isEnabled) {
    for (GObservable* option: theGraphics->constructors) {
        if (auto* button = dynamic_cast<GButton*>(option)) {
            button->setEnabled(isEnabled);
        }
    }
    theOptionsEnabled = isEnabled;
}

void runDemos() {
    theGraphics = makeGraphics();

    while (true) {
        /* Update the window (no-op if nothing needs to be redrawn.) */
        theGraphics->handler->draw(theGraphics->window);

        GEvent e = waitForEvent(MOUSE_EVENT | ACTION_EVENT | CHANGE_EVENT | TIMER_EVENT | WINDOW_EVENT);
        if (e.getEventClass() == ACTION_EVENT) {
            auto source = GActionEvent(e).getSource();

            /* We are responsible for the problem buttons. */
            if (theGraphics->constructors.containsKey(source)) {
                if (theOptionsEnabled) setProblem(theGraphics, source);
            }
            /* Any other event is the responsible of the problem handler. */
            else {
                theGraphics->handler->actionPerformed(source);
            }
        } else if (e.getEventClass() == CHANGE_EVENT) {
            theGraphics->handler->changeOccurredIn(GChangeEvent(e).getSource());
        } else if (e.getEventClass() == TIMER_EVENT) {
            /* If one of the handlers is responding slowly, it can cause a positive
             * feedback loop where timer events don't get pulled from the queue at the
             * rate at which they're generated, starving out higher-priority events.
             * To address this, if we pull a timer event out and it hasn't happened
             * sufficiently recently, we're going to assume we're running behind and
             * just swallow that event without processing it.
             */
            long now = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
            if (now - e.getTime() < kTimelyCutoff) {
                theGraphics->handler->timerFired();
            }
        } else if (e.getEventClass() == MOUSE_EVENT) {
            if (e.getEventType() == MOUSE_MOVED) {
                theGraphics->handler->mouseMoved(e.getX(), e.getY());
            } else if (e.getEventType() == MOUSE_PRESSED) {
                theGraphics->handler->mousePressed(e.getX(), e.getY());
            } else if (e.getEventType() == MOUSE_DRAGGED) {
                theGraphics->handler->mouseDragged(e.getX(), e.getY());
            }
        } else if (e.getEventClass() == WINDOW_EVENT) {
            if (e.getEventType() == WINDOW_MAXIMIZED ||
                e.getEventType() == WINDOW_RESIZED   ||
                e.getEventType() == WINDOW_RESTORED) {
            theGraphics->handler->windowResized(theGraphics->window);
            } else if (e.getEventType() == WINDOW_CLOSED) {
                return;
            }
        }
    }
}
