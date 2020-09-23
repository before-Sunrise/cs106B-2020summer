#include "datapoint.h"
#include "testing/SimpleTest.h"
#include <string>
using namespace std;

/* The theme of this exercise will be building a grocery list of DataPoints,
 * where each point is an item that you want to buy at the store, with its
 * associated priority of how important it is to get.
 *
 * The DataPoint struct is defined in datapoint.h if you want to inspect the
 * declaration of the struct.
 */

PROVIDED_TEST("Test valid use of array, allocate/access/deallocate") {
    DataPoint* shoppingList = new DataPoint[6]; // allocate

    for (int i = 0; i < 3; i++) {
        shoppingList[i].name = "eggs"; // set struct field by field
        shoppingList[i].priority = 10 + i;
    }
    shoppingList[0].priority += 5;
    delete[] shoppingList; // deallocate
}

//PROVIDED_TEST("Test case that dereferences null pointer") {
//    /* Often we initialize pointers to a special value called nullptr to
//     * indicate a pointer that is not yet pointing to valid memory. While
//     * this value can be useful, dereferencing it can have dramatic
//     * consequences!
//     */
//    DataPoint* shoppingList = nullptr; // no memory allocated
//    string n = shoppingList[0].name; // executing this line dereferences nullptr
//}

//PROVIDED_TEST("Test case that accesses memory after it was deallocated") {
//    DataPoint* shoppingList = new DataPoint[4]; // allocate
//    shoppingList[0].name = "bread";
//    shoppingList[0].priority = 1000;
//    delete[] shoppingList; // shoppingList now holds address of memory that was deallocated
//    EXPECT_EQUAL(shoppingList[0].name, "bread"); // executing this line accesses deallocated memory
//}

//PROVIDED_TEST("Test case that deallocates same memory address twice") {
//    DataPoint* shoppingList = new DataPoint[3]; // allocate
//    DataPoint* ptr = shoppingList; // ptr has same memory address as shoppingList
//    delete[] shoppingList;  // deallocate memory once
//    delete[] ptr;           // executing this line deallocates same memory again
//}

PROVIDED_TEST("Test case that uses wrong form of delete operator") {
    DataPoint* shoppingList = new DataPoint[6]; // allocate using new[], must deallocate using delete[]
    shoppingList[0] = {"milk", 1};
    delete shoppingList;         // executing this line uses wrong form of delete
    DataPoint* single = new DataPoint;         // allocate using new, must deallocate using delete
    single->name = "cheese";
    delete[] single;             // executing this line uses wrong form of delete
}
