#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "set.h"
using namespace std;
int fewestCoinsForHelper(int cents, Set<int>& coins, int sum, int time,string seq);
int fewestCoinsFor(int cents, Set<int>& coins);
int main() 
{
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    Set<int> coins = {1, 5, 10, 25, 50};
    int number = fewestCoinsFor(31, coins);
    cout << number << endl;
    return 0;
}
