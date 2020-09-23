#include <iostream>
#include "console.h"
#include "strlib.h"
#include "fractals.h"
#include "testing/SimpleTest.h"
using namespace std;

int main() {
    if (runSimpleTests(NO_TESTS)){
        return 0;
    }
    runCantorDemo();
    runSnowflakeDemo();
    runSierpinskiDemo();
    return 0;
}

/* This function takes in a string as a parameter and returns a
 * boolean value indicating whether or not the specified string is
 * a palindrome. A palindrome is defined as a string that reads the
 * same both forwards and backwards.
 *
 * This function is implemented recursively.
 */
bool isPalindrome(string s) {
    if (s.length() < 2){
        return true;
    } else {
        if (s[0] != s[s.length() - 1]) {
            return false;
        }
        return isPalindrome(s.substr(1, s.length() - 2));
    }
}

PROVIDED_TEST("Lecture slide examples for isPalindrome"){
    EXPECT(isPalindrome("level"));
    EXPECT(isPalindrome("racecar"));
    EXPECT(isPalindrome("step on no pets"));
    EXPECT(!isPalindrome("high"));
    EXPECT(!isPalindrome("hi"));
    EXPECT(!isPalindrome("palindrome"));
    EXPECT(isPalindrome("A"));
    EXPECT(isPalindrome(""));
}

