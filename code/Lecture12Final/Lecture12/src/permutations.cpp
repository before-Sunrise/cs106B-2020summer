#include "vector.h"
#include "testing/SimpleTest.h"
#include "random.h"

using namespace std;

/* Given a string of letters we haven't yet considered, along with the partial
 * string built up so far, prints out all permutations that can be made from
 * those characters.
 */
void listPermutationsHelper(string remaining, string soFar) {
    /* Base case: If we've already committed to every character, then print out
     * the permutation we made.
     */
    if (remaining == "") {
        cout << soFar << endl;
    }
    /* Recursive case: Some character must come next in the ordering. Try all
     * possible choices of what it can be.
     */
    else {
        for (int i = 0; i < remaining.size(); i++) {
            /* Form a new string by dropping out the character at this index.
             * Note that getting a substring from an index that's equal to the
             * length of the string is perfectly legal; it just gives back the
             * empty string.
             */
            char currentLetter = remaining[i];
            string rest = remaining.substr(0, i) + remaining.substr(i + 1);

            listPermutationsHelper(rest, soFar + currentLetter);
        }
    }
}

void listPermutations(string str) {
    listPermutationsHelper(str, "");
}

