#include <iostream>
#include "console.h"
#include "pqclient.h"
#include "pqheap.h"
#include "pqsortedarray.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

// You are free to edit the main in any way that works
// for your testing/debugging purposes.
// We will supply our own main() during grading

int main()
{
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    cout << "All done, exiting" << endl;
    return 0;
}


// Do not remove or edit this test. It is here to to confirm that your code
// conforms to the expected function prototypes need for grading
PROVIDED_TEST("Confirm function prototypes") {
    bool execute = false;
    if (execute) {
        {
            PQSortedArray pq;
            pq.enqueue({ "", 1 });
            DataPoint n = pq.peek();
            n = pq.dequeue();
            int size = pq.size();
            pq.isEmpty();
            pq.clear();
        }
        Vector<DataPoint> v;
        pqSort(v);
        v = topK(cin, 1);
        {
            PQHeap pq;
            pq.enqueue({ "", 1 });
            DataPoint n = pq.peek();
            n = pq.dequeue();
            int size = pq.size();
            pq.isEmpty();
            pq.clear();
        }
    }
}
