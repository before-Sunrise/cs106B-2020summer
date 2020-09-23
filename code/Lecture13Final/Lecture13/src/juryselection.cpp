#include <iostream>
#include "vector.h"
#include "testing/SimpleTest.h"

using namespace std;

struct juror {
    string name;
    int bias;
};

void displayJury(Vector<juror>& jury){
    for (juror j: jury) {
        cout << "(" << j.name << " " << j.bias << ") ";
    }
    cout << endl;
}

void findAllUnbiasedJuriesHelperV2(Vector<juror>& allCandidates, Vector<juror>& currentJury, int currentBias, int index){
    if (index == allCandidates.size()){
        if (currentBias == 0){
            displayJury(currentJury);
        }
    } else {
        juror currentCandidate = allCandidates[index];

        /* Case 1: Do not include current candidate. */
        findAllUnbiasedJuriesHelperV2(allCandidates, currentJury, currentBias, index + 1); // Do not include the current candidate

        /* Case 2: Do include current candidate. */
        currentJury.add(currentCandidate);
        findAllUnbiasedJuriesHelperV2(allCandidates, currentJury, currentBias + currentCandidate.bias, index + 1);
        currentJury.remove(currentJury.size() - 1);
    }
}

void findAllUnbiasedJuriesHelper(Vector<juror>& allCandidates, Vector<juror>& currentJury, int currentBias){
    if (allCandidates.isEmpty()){
        if (currentBias == 0){
            displayJury(currentJury);
        }
    } else {
        juror currentCandidate = allCandidates[0];
        allCandidates.remove(0);

        /* Case 1: Do not include current candidate. */
        findAllUnbiasedJuriesHelper(allCandidates, currentJury, currentBias); // Do not include the current candidate

        /* Case 2: Do include current candidate. */
        currentJury.add(currentCandidate);
        findAllUnbiasedJuriesHelper(allCandidates, currentJury, currentBias + currentCandidate.bias);
        currentJury.remove(currentJury.size() - 1);

        // undo changes
        allCandidates.insert(0, currentCandidate);
    }
}

void findAllUnbiasedJuries(Vector<juror>& allCandidates){
    Vector<juror> jury;
//    findAllUnbiasedJuriesHelper(allCandidates, jury, 0);
    findAllUnbiasedJuriesHelperV2(allCandidates, jury, 0, 0);

}


PROVIDED_TEST("Show jury selection process"){
    cout << endl;
    Vector<juror> allCandidates = {{"Nick", -3}, {"Kylie", 3}, {"Trip", 0}};
    findAllUnbiasedJuries(allCandidates);
}
