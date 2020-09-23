#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "IntStack.h"
#include "simpio.h"
using namespace std;

int main() {
    if (runSimpleTests(SELECTED_TESTS)){
        return 0;
    }

    cout << "This simple program uses a linked-list based stack " << endl
         << "to reverse the input provided by a user!" << endl << endl;

    IntStack stack;
    while (true){
        int input = getInteger("Please enter a number (-1 to stop entering): ");
        if (input == -1) break;
        stack.push(input);
    }

    while (!stack.isEmpty()){
        cout << stack.pop() << endl;
    }

    return 0;
}




