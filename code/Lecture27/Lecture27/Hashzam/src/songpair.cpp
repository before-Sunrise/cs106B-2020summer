#include "songpair.h"
#include <iostream>
#include <string>
#include "filelib.h"
#include "strlib.h"
using namespace std;

/*
 * Method: Hash
 * Usage: hash(f1, f2, timeDelta);
 * -----------------
 * Returns a number in the range [0 to 100,000). This function
 * is consistent: equal inputs always yield equal outputs. And it
 * has few collisions: unequal inputs yield unequal outputs as
 * often as possible. Uses the formula proposed by Joshua Bloch in
 * the book "Effective Java."
 */
int hashTimes(int f1, int f2, int timeDelta) {
    int p = 31;
    return (f1 + p * f2 + p * p * timeDelta) % 100000;
}

/*
 * Method: Get Path
 * Usage: getPath(f1, f2, timeDelta);
 * ----------------
 * Returns the path to the file where all songs-time records
 * (corresponding to f1 followed by f2 with timeDelta time in-
 * between) are stored.
 */
string getPath(int f1, int f2, int timeDelta) {
    int h = hashTimes(f1, f2, timeDelta);
    string fileName = integerToString(h) + ".txt";
    return "../Shazam/database/" + fileName;
}

/* Method: Get Votes
 * Usage: getVotes(f1, f2, timeDelta);
 * -----------------
 * This method returns all times in all songs where the note
 * f1 was followed by the note f2 with timeDelta time in-between.
 */
Vector<Songpair *> getVotes(int f1, int f2, int timeDelta) {
    string path = getPath(f1, f2, timeDelta);
    if(!fileExists(path)) {
        return Vector<Songpair *>();
    }
    string fileTxt = readEntireFile(path);
    Vector<string> lines = stringSplit(fileTxt, "\n");
    Vector<Songpair *> pairs;
    for(int i = 0; i < lines.size(); i++) {
        Vector<string> cols = stringSplit(lines[i], ",");
        int lineF1 = stringToInteger(cols[0]);
        int lineF2 = stringToInteger(cols[1]);
        int lineDelta = stringToInteger(cols[2]);
        if(f1 != lineF1 || f2 != lineF2 || timeDelta != lineDelta) {
            continue;
        }
        int endTime = stringToInteger(cols[3]);
        string artist = cols[4];
        string songName = cols[5];
        Songpair * sp = new Songpair(f1, f2, timeDelta, endTime, artist, songName);
        pairs.add(sp);
    }
    return pairs;
}

Songpair::Songpair(int f1, int f2, int timeDelta, int endTime, string artist, string songName) {
    this->f1 = f1;
    this->f2 = f2;
    this->timeDelta = timeDelta;
    this->artist = artist;
    this->songName = songName;
    this->endTime = endTime;
}

Songpair::~Songpair() {

}

int Songpair::getF1() {
    return f1;
}

int Songpair::getF2() {
    return f2;
}

int Songpair::getTimeDelta() {
    return timeDelta;
}

int Songpair::getEndTime() {
    return endTime;
}

string Songpair::getTitle() {
    return songName + " by " + artist;
}

void Songpair::save() {
    string path = getPath(f1, f2, timeDelta);
    string line = integerToString(f1) + "," + integerToString(f2) + "," + integerToString(timeDelta);
    line = line + "," + integerToString(endTime) + "," + artist + "," + songName;
    string toAppend = fileExists(path) ? "\n" + line : line;
    writeEntireFile(path, toAppend, true);
}
