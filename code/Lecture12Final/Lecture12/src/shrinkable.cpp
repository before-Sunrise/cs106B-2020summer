#include <string>
#include "lexicon.h"
#include "testing/SimpleTest.h"
#include "prototypes.h"

using namespace std;

bool isShrinkable(Lexicon& lex, string word) {
    /* Base Case 1: Things must be words to be shrinkable words. */
    if (!lex.contains(word)) {
        return false;
    }
    /* Base Case 2: If you're a single letter, then yes, you're shrinkable. */
    if (word.length() == 1) {
        return true;
    }

    /* Recursive Case: Try removing each letter. */
    for (int i = 0; i < word.length(); i++) {
        string shrunken = word.substr(0, i) + word.substr(i + 1);
        if (isShrinkable(lex, shrunken)) {
            return true;
        }
    }

    /* Oh fiddlesticks. */
    return false;
}

void printAllLargeShrinkableWords() {
    Lexicon lex("res/EnglishWords.txt");

    for (string word: lex) {
        if (word.length() >= 9 && isShrinkable(lex, word)) {
            cout << word << endl;
        }
    }
}

PROVIDED_TEST("Example cases of isShrinkable function from slides"){
    Lexicon lex("res/EnglishWords.txt");
    EXPECT(isShrinkable(lex, "startling"));
    EXPECT(isShrinkable(lex, "cart"));
    EXPECT(!isShrinkable(lex, "cusp"));
}
