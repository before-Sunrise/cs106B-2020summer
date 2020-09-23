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

int getRGB(double value) {
    if(value == 1) return 0x00ff0000;
    value = value * value;
    int rgb = floor((1 - value) * 255) * 0x00010101;
    if(rgb < 0 || rgb > 0x00ffffff) {
        error("rbg out of range: " + realToString(value));
    }
    return rgb;
}



int randomColorUtil() {
    switch(randomInteger(0, 5)) {
    case 0: return 0x00ff0000;
    case 1: return 0x0000ff00;
    case 2: return 0x000000ff;
    case 3: return 0x0000ffff;
    case 4: return 0x00ffff00;
    case 5: return 0x00ff00ff;
    }
    return 0;
}

void searchWindow(Grid<double> & constellation,
                  int anchor_r, int anchor_c, string artist, string song) {
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
                Songpair pair(anchor_r, r, deltaTime, c, artist, song);
                pair.save();
            }
        }
    }
}

void collectPairs(Grid<double> & constellation, string artist, string song) {
    for(int r = 0; r < constellation.numRows(); r++) {
        cout<<"collect row " << r << endl;
        for(int c = 0; c < constellation.numCols(); c++) {
            if(constellation[r][c] == 1) {
                searchWindow(constellation, r, c, artist, song);
            }
        }
    }
}

void drawNotes(Grid<double> & grid) {
    int width = grid.numCols();
    int height = grid.numRows();
    GWindow *gw = new GWindow(width, height);
    gw->setTitle("Shazam");

    int noteRadius = 4;
    for(int c = noteRadius; c < width - noteRadius; c++) {
        for(int r = noteRadius; r < height - noteRadius; r++) {
            if(grid[r][c] == 1) {
                GOval * oval = new GOval(noteRadius * 2, noteRadius * 2, noteRadius, noteRadius);
                oval->setFilled(true);
                oval->setColor(randomColorUtil());
                gw->add(oval, c - noteRadius, r - noteRadius);
            }
        }
    }
}

bool testCandidate(Grid<double> & spectrum, int center_r, int center_c) {
    double value = spectrum[center_r][center_c];
    if(value < 0.5) return false;
    int halfSize = 10; // TODO:12
    int rMin = max(0, center_r - halfSize);
    int rMax = min(spectrum.numRows() - 1, center_r + halfSize);
    int cMin = max(0, center_c - halfSize);
    int cMax = min(spectrum.numCols() - 1, center_c + halfSize);
    for(int r = rMin; r < rMax; r++) {
        for(int c = cMin; c < cMax; c++) {
            if(r != center_r || c != center_c) {
                if(spectrum[r][c] >= value) {
                    return false;
                }
            }
        }
    }
    return true;
}

void drawGrid(Grid<double> & grid) {
    int width = grid.numCols();
    int height = grid.numRows();
    GWindow* gw = new GWindow(width, height);
    gw->setTitle("Shazam");

    GBufferedImage* img = new GBufferedImage(width, height, 0xffffff);
    for(int c = 0; c < width; c++) {
        for(int r = 0; r < height; r++) {
            img->setRGB(c, r, getRGB(grid[r][c]));
        }
    }
    gw->add(img, 0, 0);
    cout << "done" << endl;
}

void removeSong(string targetArtist, string targetSong) {
    for(int i = 0; i < 100000; i++) {
        if(i % 100 == 0) cout << i << endl;
        string path = "../Shazam/database/" + integerToString(i) + ".txt";
        if(!fileExists(path)) continue;
        string contents = readEntireFile(path);
        Vector<string> lines = stringSplit(contents, "\n");
        string newContents = "";
        for(int j = 0; j < lines.size(); j++) {
            string line = lines[j];
            Vector<string> cols = stringSplit(line, ",");
            bool artistMatch = equalsIgnoreCase(cols[4], targetArtist);
            bool nameMatch = equalsIgnoreCase(cols[5], targetSong);
            if(!(artistMatch && nameMatch)) {
                if(!equalsIgnoreCase(newContents,"")) {
                    newContents += "\n";
                }
                newContents += line;
            }
        }
        writeEntireFile(path, newContents);
    }
}

void recordSong() {
    string songName = getLine("song name: ");
    string artist = getLine("artist: ");
    int seconds = stringToInteger(getLine("seconds: "));
    Vector<double> recordedSample = getMicrophoneSample(seconds);

    cout << "Making spectogram..." << endl;
    Grid<double> spectorgram = getSpectrogram(recordedSample);

    cout << "Making constellation..." << endl;
    Grid<double> constellation = makeConstillation(spectorgram);

    cout << "Saving to database..." << endl;
    collectPairs(constellation, artist, songName);
}

Grid<double> makeConstillation(Grid<double> & spectrum) {
    Grid<double> constellation(spectrum.numRows(), spectrum.numCols());

    for(int r = 0; r < spectrum.numRows(); r++) {
        for(int c = 0; c < spectrum.numCols(); c++) {
            bool isCandidate = testCandidate(spectrum, r, c);
            constellation[r][c] = isCandidate ? 1 : 0;
        }
    }

    return constellation;
}
