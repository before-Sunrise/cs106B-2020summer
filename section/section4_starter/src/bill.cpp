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
#include "testing/TestDriver.h"
#include "error.h"
#include "set.h"
using namespace std;

/*
 * Splitting the Bill (Code Writing)
 * --------------------------------------------------------
 * Your task is to write a function that takes as input a
 * total amount of money to pay (in dollars) and a set of
 * all the people who ordered something, then lists off every
 * possible way you could split the bill, assuming everyone
 * pays a whole number of dollars.
 */

void listPossiblePaymentsHelp(int total, Set<string>& people, Map<string, int>& result){
    if(people.size() == 1){ //此时递归到了base case
        result[people.first()] = total;
        cout << result << endl;
        result.remove(people.first());

    }else{
        string now = people.first();
        people -= now;
        for(int i = 0; i <= total; i++){
            result[now] = i;
            listPossiblePaymentsHelp(total - i, people, result);
            result.remove(now);
        }
        people += now;
    }

}

void listPossiblePayments(int total, Set<string>& people) {
    if(total == 0 || people.isEmpty()){
        error("input is not valid");
    }else{
        Map<string, int> result;
        listPossiblePaymentsHelp(total, people, result);
    }

}

// official solution
/*
The insight that we used in our solution is that the first person has to pay some amount of money.
We can’t say for certain how much it will be, but we know that it’s going to have to be some amount
of money that’s between zero and the full total. We can then try out every possible way of having them
pay that amount of money, which always leaves the remaining people to split up the part of the bill that
 the first person hasn’t paid.
*/

/*
 * Lists off all ways that the set of people can pay a certain total, assuming
 * that some number of people have already committed to a given set of payments.
 *
 * @param total The total amount to pay.
 * @param people Who needs to pay.
 * @param payments The payments that have been set up so far.
 */
void listPossiblePaymentsRec(int total, Set<string>& people,
                Map<string, int>& payments) {
    /* Base case: if one person left, they have to pay the whole bill.*/
    if (people.size() == 1) {
        Map<string, int> finalPayments = payments;
        finalPayments[people.first()] = total;
        cout << finalPayments << endl;
    }
    /* Recursive case: The first person has to pay some amount between 0 and
    * the total amount. Try all of those possibilities.
    */
    else {
        for (int payment = 0; payment <= total; payment++) {
            /* Create a new assignment of people to payments in which
            * this first person pays this amount.
            */
            Map<string, int> updatedPayments = payments;
            updatedPayments[people.first()] = payment;
            Set<string> remainingPeople = people - people.first();
            listPossiblePaymentsRec(total - payment, remainingPeople,
                        updatedPayments);
        }
    }
}
/* * * * * Provided Tests Below This Point * * * * */

// This test macro allows you to see the text output from a program
// that print results to the screen

MANUAL_TEST("Provided Test: Examples from handout.") {
    Set<string> people = {"A", "B", "C"};
    listPossiblePayments(4, people);
    Map<string, int> payments;
    listPossiblePaymentsRec(4, people, payments);
}

