/*
 * CS106B Section Handout Test Harness: Section 1
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
#include <fstream>
#include "filelib.h" // for readEntireFile()
#include "strlib.h" // for stringSplit and stringToInteger
#include "grid.h"
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
using namespace std;

/*
 * Sum Numbers (Code Write)
 * ----------------------------------
 * Write a program to read through a given file and sum
 * all of the numbers in the file. You can assume that numbers
 * will be composed entirely of numerical digits, optionally
 * preceded by a single negative sign.
 */
/*
official solution, which is more strict with boundary
bool isNumber(string s)
{
    // strip negative sign off negative numbers
    if (s.length() > 0 && s[0] == '-'){
        s = s.substr(1);
    }
    for (char ch : s)
        if (!isdigit(ch)) return false;
    return s.length() > 0;
}
*/
bool isNmuber(string token){
    if(token[0] == '-'){
        token = token.substr(1); //note string.substr return a copy of string's substring, so u must reassign
    }
    if(token.size() < 1) return false;
    for(char c:token){
        if(!isdigit(c)){
            return false;
        }
    }
    return true;

}

int sumNumbers(string filename){
    // TODO: Your code here
    int sum = 0;
    Vector<string> lines;
    ifstream myStream(filename.c_str());
    if(!myStream.is_open()){
        return 0;
    }
    readEntireFile(myStream,lines);
    for(string str:lines){
        Vector<string> v = stringSplit(str," ");
        for(string tkn:v){
            if(isNmuber(tkn)){
                sum += stringToInteger(tkn);
            }
        }
    }

    return sum;
}


/* * * * * Provided Tests Below This Point * * * * */
PROVIDED_TEST("given file in handout"){
    EXPECT_EQUAL(sumNumbers("res/numbers.txt"), 42);
}

PROVIDED_TEST("Nonexistent file should sum to zero") {
    EXPECT_EQUAL(sumNumbers("nonexistent_filename"), 0);
}

