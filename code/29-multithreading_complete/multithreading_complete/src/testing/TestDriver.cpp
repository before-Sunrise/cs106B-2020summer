/**
 * TestDriver.cpp
 *
 * @author Keith Schwarz
 * @version 2020/3/5
 *    Keith final revision from end of quarter 19-2
 */
#include "TestDriver.h"
#include "filelib.h"
#include <map>

using namespace std;

/* Why this type? Because we want to be able to
 *
 * 1. find all the tests in a certain group,
 * 2. have them be sorted by line number, and
 * 3. then have a name associated with each.
 */
map<TestKey, multimap<int, TestCase>>& gTestsMap() {
    static map<TestKey, multimap<int, TestCase>> result;
    return result;
}

void doFail(const string& message, size_t line, const char* filename) {
    throw TestFailedException(message, line, filename);
}
void doExpect(bool condition, const string &expression, size_t line, const char* filename) {
    if (!condition) {
        doFail(expression, line, filename);
    }
}

void reportFailure(const char* message, size_t line, const char* filename) {
    doFail(message, line, filename);
}

/* TestFailedException implementation. */
TestFailedException::TestFailedException(const string& message, size_t line, const char*)
    : logic_error("Line " + to_string(line) + ": " + string(message)) {

}

/* TestCaseAdder implementation. */
TestCaseAdder::TestCaseAdder(const TestKey& key, int line, const string& testname, const string &owner, std::function<void()> callback) {
    TestKey basename = getTail(key);
    TestCase tcase;
    tcase.testname = testname;
    tcase.line = line;
    tcase.filename = basename;
    tcase.owner = owner;
    tcase.callback = callback;
    gTestsMap()[basename].insert(make_pair(line, tcase));
}
