#include <iostream>
#include "set.h"
#include "console.h"
using namespace std;


Set<Set<string>> combinationsRec(Set<string>& remaining, int k, Set<string>& chosen) {
    /* Base case 1: No more to pick? Then you're done. */
    if (k == 0) {
        return { chosen };
    }
    /* Base case 2: Can't find enough people? Sad times! */
    else if (remaining.size() < k) {
        return { };
    }
    /* Recursive case: Pick someone, then either include or exclude. */
    else {
        string elem = remaining.first();
        remaining = remaining - elem;

        /* Option 1: Exclude this person. */
        Set<Set<string>> without = combinationsRec(remaining, k, chosen);


        /* Option 2: Include this person. */
        chosen = chosen + elem;
        Set<Set<string>> with = combinationsRec(remaining, k - 1, chosen);

        chosen = chosen - elem;
        remaining = remaining + elem;

        return with + without;
    }
}


Set<Set<string>> combinationsOf(Set<string>& elems, int k) {
    Set<string> combinationTracker;
    return combinationsRec(elems, k, combinationTracker);
}

void combinationsDemo() {
    Set<string> usSupremeCourt = {
        "Thomas",
        "Ginsburg",
        "Breyer",
        "Roberts",
        "Alito",
        "Sotomayor",
        "Kagan",
        "Gorsuch",
        "Kavanaugh"
    };

    Set<Set<string>> combinations = combinationsOf(usSupremeCourt, 5);
    cout << "There are " << combinations.size() << " combinations. They are: " << endl;
    for (Set<string> combination: combinations) {
        cout << combination << endl;
    }
}
