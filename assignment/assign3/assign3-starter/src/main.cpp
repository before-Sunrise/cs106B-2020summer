#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "recursion.h"
using namespace std;

/* 
 * You are free to edit the main in any way that works
 * for your testing/debugging purposes.
 * We will supply our own main() during grading
 */

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    cout << "All done, exiting" << endl;
    return 0;
}


/* Do not remove or edit this test. It is here to to confirm that your code
 * conforms to the expected function prototypes need for grading.
 */
PROVIDED_TEST("Confirm function prototypes") {
    bool execute = false;
    if (execute) {
        string str;
        int i = 0;
        bool b;
        b = isBalanced(str);
        str = operatorsOnly(str);
        b = checkOperators(str);
        GPoint pt;
        GWindow win;
        drawSierpinskiTriangle(win, pt, pt, pt, i);
        Queue<int> q;
        Vector<Queue<int>> v;
        q = merge(q, q);
        q = multiMerge(v);
        q = recMultiMerge(v);
        Grid<char> g;
        Lexicon lex;
        i = scoreBoard(g, lex);
        Vector<int> blocks;
        blocks = computePowerIndexes(blocks);
    }
}
