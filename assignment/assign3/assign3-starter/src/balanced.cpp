/* 
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/* 
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
string operatorsOnly(string s) {
    if(s.empty()) return "";
    if(s[0] == '(' || s[0] == '[' || s[0] == '{'
            || s[0] == ')' || s[0] == ']' || s[0] == '}'){
        return s[0] + operatorsOnly(s.substr(1));
    }else{
        return operatorsOnly(s.substr(1));
    }
}

/* 
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool isBalancedHelp(char a, char b){
    if(a == '(' && b == ')' ||
       a == '[' && b == ']' ||
       a == '{' && b == '}'){
        return true;
    }
    return  false;
}

// check whether a string has paired Brackets
// return -1 if doesn't
// return index of the first Bracket if exists
int hasPairedBrackets(string s){
    //如果长度小于2，则不可能有配对的括号
    if(s.size() <= 1){
        return -1;
    }else{
        for(int i = 0; i < s.size() - 1; i++){
            if(s[i] == '(' && s[i+1] == ')' || s[i] == '{'
                        && s[i+1] == '}' || s[i] == '[' && s[i+1] == ']'){
                return i;
            }
        }

        return -1;
    }
}

bool checkOperators(string s) {
    // base case
    if(s.empty()){
        return true;
    }else{
        int index = hasPairedBrackets(s);

        if(index == -1){
            return false;
        }else{
            // recursive case, narrow the parameter's value to the base case situation
            return checkOperators(s.substr(0, index) + s.substr(index + 2));
        }
    }
}

/* 
 * This function assumes correct implementation of the previous 
 * two functions. It uses the operatorsOnly function to strip out
 * all characters from the provided string and then checks to see
 * whether the remaining operators are balanced by using the 
 * checkOperators function. All in all, this function is able to 
 * correctly determine whether a snippet of code has correctly
 * balanced bracketing operators. You should not need to modify this
 * function if the previous two functions have been implemented
 * correctly. 
 */
bool isBalanced(string str) {
    string ops = operatorsOnly(str);
    return checkOperators(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsOnly on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    string only = "(){([])(())}";
    EXPECT_EQUAL(operatorsOnly(example), only);
}


PROVIDED_TEST("checkOperators on example from writeup") {
    string only = "(){([])(())}";
    EXPECT(checkOperators(only));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on illegal examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

