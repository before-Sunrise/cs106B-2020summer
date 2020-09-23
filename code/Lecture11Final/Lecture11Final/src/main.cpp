#include <iostream>
#include "console.h"
#include "strlib.h"
#include "prototypes.h"
#include "testing/SimpleTest.h"
using namespace std;

int main() {
    if (runSimpleTests(NO_TESTS)){
        return 0;
    }

    //runHanoiDemo();
    runSequencesDemo();
    return 0;
}
