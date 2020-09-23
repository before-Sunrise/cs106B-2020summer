#include <iostream>
#include "console.h"
#include "strlib.h"
#include "prototypes.h"
#include "set.h"
#include "testing/SimpleTest.h"
using namespace std;

int main() {
    if (runSimpleTests(SELECTED_TESTS)){
        return 0;
    }

    listPermutations("saki");
    Set<string> teachingTeam = {"Nick", "Kylie", "Trip"};
    listSubsets(teachingTeam);
//    printAllLargeShrinkableWords();
    return 0;
}
