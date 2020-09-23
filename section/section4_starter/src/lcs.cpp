/*
 * CS106B Section Handout Test Harness: Section 3
 * ----------------------------------------------
 * These problems have been galvanized from years of
 * section handouts that have been worked on by numerous
 * instructors and TA's. Codified by Trip Master and Nick
 * Bowman for CS106B Spring 2020.
 *
 * A huge thank you to Keith Schwarz and Julie Zelenski
 * for creating an amazing testing harness!
 */

#include <iostream>
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "error.h"
using namespace std;

/*
 * Longest Common Subsequence (Code Writing)
 * --------------------------------------------------------
 * Write a recursive function named longestCommonSubsequence
 * that returns the longest common subsequence of two strings
 * passed as arguments. Recall that if a string is a
 * subsequence of another, each of its letters occurs in the
 * longer string in the same order, but not necessarily
 * consecutively.
 */



string longestCommonSubsequence(string s1, string s2) {
    if(s1.empty() || s2.empty()){
        return "";
    }else if(s1[0] == s2[0]){
        // 若两个字符串的第一个字符相等，则lcs一定是这个字符+之后字符的lcs
        return s1[0] + longestCommonSubsequence(s1.substr(1),
                             s2.substr(1));
    }else{
        // 若不相等，则lcs有两种可能
       string  choice1 = longestCommonSubsequence(s1.substr(1),
                                                  s2);
       string choice2 = longestCommonSubsequence(s1,
                                                 s2.substr(1));
       return choice1.size() > choice2.size() ? choice1 : choice2;
    }
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: First example from handout.") {
    EXPECT_EQUAL(longestCommonSubsequence("cs106a", "cs106b"), "cs106");
}

PROVIDED_TEST("Provided Test: Second example from handout.") {
    EXPECT_EQUAL(longestCommonSubsequence("nick", "julie"), "i");
}

PROVIDED_TEST("Provided Test: Third example from handout.") {
    EXPECT_EQUAL(longestCommonSubsequence("karel", "c++"), "");
}

PROVIDED_TEST("Provided Test: Fourth example from handout.") {
    EXPECT_EQUAL(longestCommonSubsequence("she sells", "seashells"), "sesells");
}
