/*
 * CS106B Section Handout Test Harness: Section 3
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
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "error.h"
#include "stack.h"
#include "set.h"
using namespace std;

/*
 * Make Chance (Code Writing)
 * --------------------------------------------------------
 * Your task is to write a function fewestCoinsFor that takes
 * as input a number of cents and a Set indicating the different
 * denominations of coins used in a country, then returns the
 * minimum number of coins required to make change for that total.
 */

//找零钱，用最少的硬币，硬币种类为coins，贪婪算法在某些情况并不适用，如test2

int fewestCoinsForHelper(int cents, Set<int>& coins, int sum, int time);

int fewestCoinsFor(int cents, Set<int>& coins) {
    int least = fewestCoinsForHelper(cents, coins, 0, 0);
    return least;
}

int fewestCoinsForHelper(int cents, Set<int>& coins, int sum, int time){
    if(sum == cents){
        return time;
    }
    if(sum > cents){
        return cents + 1; //所需硬币的最小数量一定大于cents + 1
    }
    int leastNumber = cents;
    for(int coin: coins){
        int leastNumverForThis = fewestCoinsForHelper(cents, coins, sum + coin, time + 1);
        if(leastNumverForThis < leastNumber){
            leastNumber = leastNumverForThis;
        }
    }
    return leastNumber;
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Provided Test: US Currency System") {
    Set<int> coins = {1, 5, 10, 25, 50};
    EXPECT_EQUAL(fewestCoinsFor(31, coins), 3);
}

PROVIDED_TEST("Provided Test: Recursia Currency System") {
    Set<int> coins = {1, 12, 14, 63};
    EXPECT_EQUAL(fewestCoinsFor(24, coins), 2);
}


/*
The idea behind this solution is the following: if we need to make change for zero cents,
the only (and, therefore, best!) option is to use 0 coins. Otherwise, we need to give back at least one coin.
What’s the first coin we should hand back? We don’t know which one it is, but we can say that it’s got to be
 one of the coins from our options and that that coin can’t be worth more than the total.
So we’ll try each of those options in turn, see which one ends up requiring the fewest coins for the remainder,
then go with that choice. The code for this is really elegant and is shown here:

 * Given a collection of denominations and an amount to give in change, returns
 * the minimum number of coins required to make change for it.
 *
 * @param cents How many cents we need to give back.
 * @param coins The set of coins we can use.
 * @return The minimum number of coins needed to make change.
 *
 */
/*
int fewestCoinsFor(int cents, Set<int>& coins) {
    // Base case: You need no coins to give change for no cents.
    if (cents == 0) {
        return 0;
    }
    // Recursive case: try each possible coin that doesn’t exceed the total as
    // as our first coin.
    else {
        int bestSoFar = cents + 1; // Can never need this many coins;
        for (int coin: coins) {
            // If this coin doesn’t exceed the total, try using it.
            if (coin <= cents) {
                bestSoFar = min(bestSoFar,
                    fewestCoinsFor(cents - coin, coins));
            }
        }
        return bestSoFar + 1; // For the coin we just used.
    }
}
*/
