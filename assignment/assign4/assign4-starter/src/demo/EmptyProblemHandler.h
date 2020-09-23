#pragma once

#include "ProblemHandler.h"

/* A problem handler that just prompts the user to pick a problem to solve. This
 * gets installed whenever there's nothing else to display.
 */
class EmptyProblemHandler: public ProblemHandler {
protected:
    void repaint(GWindow& window) override;
};

