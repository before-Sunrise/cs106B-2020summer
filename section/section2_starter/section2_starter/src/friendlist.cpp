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
#include "map.h"
#include "vector.h"
#include "filelib.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
using namespace std;

/*
 * Friend List (Code Write)
 * ----------------------------------
 * Write a function named friendList that takes in a file name and reads
 * friend relationships from a file and writes them to a Map.
 * friendList should return the populated Map. Friendships are bi-directional.
 * The file contains one friend relationship per line, with names separated
 * by a single space. You do not have to worry about malformed entries.
 */

Map<string, Vector<string>> friendList(string filename) {
    ifstream input;
    Map<string, Vector<string>> friendList;
    if(!openFile(input, filename)){
        cout << "wrong filename";
    }
    string line;
    while(getline(input, line)){
        stringstream ss(line);
        string first, last;
        ss >> first >> last;
        friendList[first].add(last);
        friendList[last].add(first);
    }
    return friendList;
}

/*
Map<string, Vector<string>> friendList(string filename) {
    ifstream in;
    Vector<string> lines;

    if (openFile(in, filename)) {
        readEntireFile(in, lines);
    }

    Map<string, Vector<string> > friends;
    for (string line: lines) {
        Vector<string> people = stringSplit(line, " ");
        string s1 = people[0];
        string s2 = people[1];
        friends[s1] += s2;
        friends[s2] += s1;
    }
    return friends;
}
*/
/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("provided test case from handout") {
    Map<string, Vector<string>> soln;
    soln["Trip"] = {"Kylie", "Nick"};
    soln["Kylie"] = {"Trip"};
    soln["Nick"] = {"Trip"};
    EXPECT_EQUAL(soln, friendList("res/buddies.txt"));
}
