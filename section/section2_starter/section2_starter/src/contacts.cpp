/*
 * CS106B Section Handout Test Harness: Section 1
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
#include "map.h"
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
using namespace std;

/*
 * findContact (debugging)
 * ----------------------------------
 * The function below attempts to look for PHONENUMBER in CONTACTS.
 * findContact returns the associated value (the name) if found, empty
 * string if not found.
 *
 * This implementation is BUGGY! You will need to step
 * through the debugger to figure out what's going on.
 */

string findContact(string phoneNumber, Map<string, string>& contacts) {
    // If PHONENUMBER does not exist in CONTACTS,
    // The Map access will return "".
    if (contacts[phoneNumber] == "") {
        return "";
    } else {
        return contacts[phoneNumber];
    }
}

//When you look up an element in a Map that doesn't exist, the Map will automatically insert that element into the map
//with the default value (0 or empty string, or some equivalent).
//This means that the size of your map increases if you try and access into the Map with a brand-new key.
// containsKey() solution,it doesn't auto-insert.
/*string findContact(string phoneNumber, Map<string, string>& contacts)
{
    return contacts.containsKey(phoneNumber) ? contacts[phoneNumber] : "";
}

// const solution,prevent  auto-insertion
string findContact(string phoneNumber, const Map<string, string>& contacts)
{
    return (contacts[phoneNumber] != "") ? contacts[phoneNumber] : "";
}
*/

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Verifies that findContact works on a simple example") {
    Map<string, string> contacts;
    contacts["1234567890"] = "Trip";
    contacts["0987654321"] = "Kylie";
    contacts["1111111111"] = "Nick";
    EXPECT_EQUAL(findContact("1234567890", contacts), "Trip");
    EXPECT_EQUAL(findContact("0000000000", contacts), "");
}

PROVIDED_TEST("Verifies that findContact works with many spurious lookups") {
    Map<string, string> contacts;
    contacts["1234567890"] = "Trip";
    EXPECT_EQUAL(findContact("857238493", contacts), "");
    EXPECT_EQUAL(findContact("13542", contacts), "");
    EXPECT_EQUAL(findContact("This isn't a phone number", contacts), "");
    EXPECT_EQUAL(findContact("New phone, who dis?", contacts), "");
    EXPECT_EQUAL(findContact("", contacts), "");
    EXPECT_EQUAL(findContact("1234567890", contacts), "Trip");
    EXPECT_EQUAL(findContact("Trip", contacts), "");
}
