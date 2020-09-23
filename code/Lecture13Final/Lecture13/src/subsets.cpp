#include <iostream>
#include "set.h"

using namespace std;

void listSubsetsHelper(Set<string>& remaining, Set<string>& chosen);

/* Code to list all subsets of a set. The first parameter
 * represents the elements we haven't yet considered. The second
 * parameter represents which integers we have already chosen.
 */
void listSubsetsHelper(Set<string>& remaining, Set<string>& chosen) {
    /* Base case: If there are no more decisions to be made, then we've
     * committed to a specific set.
     */
    if (remaining.isEmpty()) {
        cout << chosen << endl;
    }
    /* Recursive case: Pick an element. Then we either include that element
     * or exclude that element.
     */
    else {
        string elem = remaining.first();

        /* Indicate that this element has been considered by removing it
         * from the set of remaining options. */
        remaining = remaining - elem;

        /* Option 1: Do not add element to chosen */
        listSubsetsHelper(remaining, chosen);

        /* Option 2: Add element to chosen */
        chosen = chosen + elem;
        listSubsetsHelper(remaining, chosen);

        /* We must explicitly undo the changes we made to the data structures
         * so that we don't impact the state of the variables in other stack
         * frames.
         */
        chosen = chosen - elem;
        remaining = remaining + elem;
    }
}

void listSubsets(Set<string>& choices) {
    Set<string> subsetTracker;
    listSubsetsHelper(choices, subsetTracker);
}

