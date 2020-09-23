#ifndef SONGPAIRS
#define SONGPAIRS
#include <string>
#include "vector.h"
using namespace std;

class Songpair {
public:
    Songpair(int f1, int f2, int timeDelta, int endTime, string artist, string songName);
    ~Songpair();
    int getF1();
    int getF2();
    int getTimeDelta();
    int getEndTime();
    void save();
    string getTitle();
private:
    int f1;
    int f2;
    int timeDelta;
    int endTime;
    string artist;
    string songName;
};

Vector<Songpair *> getVotes(int f1, int f2, int timeDelta);

#endif // SONGPAIRS

