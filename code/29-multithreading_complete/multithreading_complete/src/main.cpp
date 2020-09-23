/**
 * File: section8.cpp
 * -------------------
 * Adapted from Jerry's "Introverts" example from CS110. Rage, Jerry.
 * This is a simple example program that introduces students to
 * multithreading. A function F is called KARRSIZE times, sleeping
 * each time. With a multithreading approach, this shouldn't take much
 * longer than the normal sleep time. Woo hoo!
 */
#include <iostream>
#include "console.h"
#include <thread>         // for C++11 thread support
#include <mutex>
#include "vector.h"
#include "timer.h"
#include "stdlib.h"
#include "random.h"

using namespace std;
static const int kArrSize = 10;
static const int kSleepTime = 1000; // in ms.

/* Don't worry about these function headers. */
static void setup (Timer& t);
static void stop (Timer &t);
static void task (int input, int id, mutex *m);
static Vector<int> getDataForTask();

/**
 * This program calls function TASK on KARRSIZE integers.
 * How hard can it be?
 */
int main () {
    Timer t; // Create and start a timer to time our operation!
    setup(t);

    /* Setup Vector of nums. */
    Vector<int> numsToProcess = getDataForTask();


    /* Your code here! */
    thread threadArr[kArrSize];
    mutex m;
    for (int i = 0; i < kArrSize; i++) {
        threadArr[i] = thread (task, numsToProcess[i], i, &m);
    }
    for (int i = 0; i < kArrSize; i++) {
        threadArr[i].join();
    }

    stop (t);
    return 0;
}



/**
 * Don't worry about this function. Just keeping
 * clutter out of main. Sets up the countdown
 * sequence.
 */
static void setup (Timer& t) {
    cout << "Setting the program up..." << endl;
    this_thread::sleep_for(chrono::milliseconds(3000));
    cout << "Let's process " << kArrSize << " numbers!" << endl;
    cout << "Starting in 3..." << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "2..." << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "1..." << endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << "GO!!" << endl;
    t.start();
}

/**
 * Stops the timer, printing out the appropriate
 * time in ms and seconds. Just trying to keep
 * boilerplate code out of main.
 */
static void stop (Timer &t) {
    t.stop();
    long ms = t.elapsed();
    cout << "All done! The total time spent working was "
         << ms << " milliseconds (roughly " << ms / 1000
         << " second" << ((ms / 1000 == 1) ? "" : "s") << "!)"<< endl;
}

/**
 * Thunk (routine) that processes a given number.
 * Pretend like it does something fancy and work-
 * intensive. It doesn't, it actually just sleeps,
 * but it's a pedagogical example.
 */
static void task (int input, int id, mutex *m) {
    input *= 2; // The work we do here doesn't really matter.
    /* sleep_for() to simulate an I/O */
    this_thread::sleep_for(chrono::milliseconds(kSleepTime));
    m->lock();
    cout << "Hello from worker " << id << endl;
    m->unlock();
}

/**
 * Returns a Vector<int> of some 'data' for the user to
 * process. Really just randomly generated numbers, but
 * it's totally possible to ping the user for these data.
 */
static Vector<int> getDataForTask() {
    Vector<int> toReturn;
    for (int i = 0; i < (int) kArrSize; i++) {
        toReturn.add(randomInteger(-20, 20));
    }
    return toReturn;
}




