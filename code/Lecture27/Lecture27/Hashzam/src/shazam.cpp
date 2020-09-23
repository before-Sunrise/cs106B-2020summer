#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "console.h"
#include "random.h"
#include "simpio.h"
#include "strlib.h"
#include "microphone.h"
#include "gwindow.h"
#include <math.h>
#include "gbufferedimage.h"
#include "filelib.h"
#include "songpair.h"
#include "hashmap.h"
#include "util.h"
using namespace std;

const double SAMPLES_PER_SECOND = 85.6;
const bool VERBOSE = true;
const int MIN_VOTES = 5;


void welcome(GWindow *gw, GImage *logo) {
    cout << "Welcome!" << endl;
    int width = getScreenWidth() / 2;
    gw = new GWindow(width, getScreenHeight());
    gw->setLocation(0, 0);
    gw->setTitle("Shazam");
    logo = new GImage("shazam.png");
    logo->scale(width/logo->getWidth());
    gw->add(logo, 0, (getScreenHeight() - logo->getHeight() - 50)/2);
}

void searchWindowForVotes(HashMap<string, int> & voteMap, Grid<double> & constellation,
                          int anchor_r, int anchor_c) {
    int startR = anchor_r - 30;
    int endR = anchor_r + 30;
    int startC = anchor_c + 30;
    int endC = anchor_c + 90;
    for(int r = startR; r <= endR; r++) {
        for(int c = startC; c <= endC; c++) {
            if(r < 0 || r >= constellation.numRows()) {
                continue;
            }
            if(c >= constellation.numCols()) {
                continue;
            }
            if(constellation[r][c] == 1) {
                int deltaTime = c - anchor_c;

                Vector<Songpair *> votes = getVotes(anchor_r, r, deltaTime);
                for(int i = 0; i < votes.size(); i++) {
                    Songpair * sp = votes[i];
                    int startTime = (sp->getEndTime() - c) / SAMPLES_PER_SECOND;
                    string key = sp->getTitle() + ". " + integerToString(startTime) + "s from start.";
                    if(!voteMap.containsKey(key)) {
                        voteMap.put(key, 0);
                    }
                    voteMap.put(key, 1 + voteMap.get(key));
                }
            }
        }
    }
}

void findSong(Grid<double> & constellation) {
    // collect pairs of notes
    // create vote-map
    // find song with most votes

    HashMap<string, int> voteMap;
    for(int r = 0; r < constellation.numRows(); r++) {
        for(int c = 0; c < constellation.numCols(); c++) {
            if(constellation[r][c] == 1) {
                searchWindowForVotes(voteMap, constellation, r, c);
            }
        }
    }
    string argMax = "";
    int max = 0;
    Vector<string> candidates = voteMap.keys();
    for(int i = 0; i < candidates.size(); i++) {
        string key = candidates[i];
        int numVotes = voteMap.get(key);
        if(VERBOSE && numVotes >= MIN_VOTES) {
            cout << numVotes << "\t" << key << endl;
        }
        if(numVotes > max) {
            argMax = key;
            max = numVotes;
        }
    }
    if(max < MIN_VOTES) {
        cout << "Song not found!" << endl;
    } else {
        cout << "Song found" << endl;
        cout << argMax << endl;
    }
}

/*
 * Function: Shazam Song
 * Usage: shazamSong()
 * ------------
 * Listens to a sample of audio and identifies the Song that is playing.
 */
void shazamSong() {
    Vector<double> recordedSample = getMicrophoneSample(5);
    Grid<double> spectorgram = getSpectrogram(recordedSample);
    Grid<double> constellation = makeConstillation(spectorgram);
    if(VERBOSE) {
//         drawGrid(spectorgram);
        drawNotes(constellation);
    }
    cout << "Searching for song..." << endl;
    findSong(constellation);
}

/*
 * Function: Shazam Songs
 * Usage: shazamSongs()
 * ------------
 * Repeatedly listens to audio and identifies the Song that is playing.
 */
void shazamSongs() {
    while(true) {
        getLine("Press enter to Hashzam");
        shazamSong();
        cout << endl;
    }
}

/*
 * Function: Main
 * ------------
 * Runs shazam!
 */
int main() {
    GWindow *gw = nullptr;
    GImage *logo = nullptr;
    welcome(gw, logo);
    shazamSongs();
//    recordSong();
    delete(gw);
    delete(logo);

    return 0;
}
