
/*
 * Summer 2020 CS106B: Code adapted from a lecture example by Chris Gregg
 *
 * This program contains code to demonstrate pointers.
 *
 * Note 1: This program will compile with warnings due to uninitialized variables.
 * Note 2: This program will crash (purposefully) on some choices.
 */

#include <iostream>
#include "console.h"
#include "simpio.h"

using namespace std;


// function prototype declarations
int requestTest();
void addressExample();
void crash();
void sameVariablePointers();
void derefExample();
void pointerExample();

int main() {

    int testNum = requestTest();
    while (testNum != -1) {
        switch (testNum) {
        case 0: // address example
            addressExample();
            break;
        case 1:
            crash(); // will crash...
            break;
        case 2:
            sameVariablePointers();
            break;
        case 3:
            derefExample();
            break;
        case 4:
            pointerExample();
        }
        cout << endl;
        testNum = requestTest();
    }
    cout << "Goodbye!" << endl;
    
    return 0;
}

int requestTest() {
    int response = -2; // initial condition
    cout << "Pointer Examples" << endl;
    cout << "0. Pointers have addresses" << endl;
    cout << "1. Seg Fault!" << endl;
    cout << "2. Pointers to the same variable" << endl;
    cout << "3. Dereference example" << endl;
    cout << "4. Pointer example with crash at end." << endl;
    cout << endl;
    while (response < -1 or response > 4) {
        response = getInteger("Please choose an option (0-4), -1 to quit: ");
    }
    return response;
}

void addressExample() {
    string s;
    int i = 0;
    double sum = 0;

    cout << "Variables uninitialized" << endl
         << "    Address of s  :  "   << &s   << endl
         << "    Address of i  :  "   << &i   << endl
         << "    Address of sum:  "   << &sum << endl
         << "    Value of s    :  "   << s    << endl
         << "    Value of i    :  "   << i    << endl // will cause warning
         << "    Value of sum  :  "   << sum  << endl // will cause warning
         << endl;

    s   = "Bazinga";

    sum = 0.0;
    i   = 2;
    cout << "---------------------" << endl;
    cout << "Variables initialized" << endl
         << "    Address of s  :  " << &s   << endl
         << "    Address of i  :  " << &i   << endl
         << "    Address of sum:  " << &sum << endl
         << "    Value of s    :  " << s    << endl
         << "    Value of i    :  " << i    << endl
         << "    Value of sum  :  " << sum  << endl
         << endl;

    while (i < 1000) {
        sum += i;
        i *= 2;

        cout << "---------------------" << endl;
        cout << "Ready to do loop test again" << endl
             << "    Address of i  :  " << &i   << endl
             << "    Address of sum:  " << &sum << endl
             << "    Value of i    :  " << i    << endl
             << "    Value of sum  :  " << sum  << endl
             << endl;
    }

}

// This function will attempt to dereference nullptr, which will crash
// the program. :(
void crash() {
    string* sPtr = nullptr;
    string s = "hello";
    cout << *sPtr << endl;
}

void derefExample() {
    string* sPtr = nullptr;
    string s = "hello";
    sPtr = &s;
    *sPtr = "goodbye";
    cout << *sPtr << endl;
}

void pointerExample() {
    int      x;
    int*     p;  // declaration of a pointer to an int
    int*     q;  // another pointer to an int

    /**** STAGE 1 *****/

    cout << endl << "stage 1, initialization" << endl;
    x = 3;
    p = &x;  //the & operator finds the address of the variable

    cout << "  p: "  <<  p << endl;  // print the address of x
    cout << " *p: "  << *p << endl;  // print the value   of x;
    cout << "  x: "  <<  x << endl;  // print the value   of x;

    /**** STAGE 2 *****/
    cout << endl;
    cout << "stage 2, *p = *p - 1 " << endl;
    *p = *p - 1;

    cout << "  x: " <<  x << endl;   // value of x has decreased by 1
    cout << " *p: " << *p << endl;   // print the value of x thru p

    /**** STAGE 3 *****/
    cout << endl;
    cout << "stage 3, q = p " << endl;
    q = p;

    cout << "  q: " <<  q << endl;   // should be the same value as p
    cout << " *q: " << *q << endl;   // which points to x

    /**** STAGE 4 *****/
    cout << endl;
    cout << "stage 4, *q = *p - 1" << endl;
    *q = *p - 1;

    // *p and *q and x should all be the same thing
    // (which is now x = x - 1)
    cout << " *p: " << *p << endl;
    cout << " *q: " << *q << endl;
    cout << "  x: " <<  x << endl;

    /**** STAGE 5 *****/
    cout << endl;
    cout << "stage 6, p = (int*) 4" << endl;

    p = (int *) 4;                   // p assigned the location 0x04

    cout << "  p: " << p << endl;
    cout << "  q: " << q << endl;
    cout << "The data stored at location 4 is: " ;
    cout << *p << endl;              // this will cause a seg fault!
}

void sameVariablePointers() {
    string *sp1 = nullptr;
    string *sp2 = nullptr;
    string s = "hello";
    sp1 = &s;
    cout << *sp1 << endl;

    sp2 = sp1;
    cout << *sp2 << endl;

    *sp1 = "goodbye";
    cout << *sp1 << endl;
    cout << *sp2 << endl;
}





