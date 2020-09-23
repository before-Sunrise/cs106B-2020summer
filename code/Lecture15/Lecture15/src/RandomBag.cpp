#include "RandomBag.h"
#include "random.h"
#include "error.h"
using namespace std;

void RandomBag::add(int value) {
    elems += value;
}

int RandomBag::removeRandom() {
    /* We're using our own isEmpty() function. This is okay to do because we
     * told C++ that isEmpty is a function in the header file, so C++ knows
     * to expect it later on.
     */
    if (isEmpty()) {
        error("Aaaaaah!");
    }

    /* This is an inefficient implementation. Great question to ponder: how
     * fast is this code in big-O terms, and how can you make it run in time
     * O(1)?
     */
    int index  = randomInteger(0, size() - 1);
    int result = elems[index];
    elems.remove(index);

    return result;
}

/* We need to put the const here because, as we said in the header file, this
 * function doesn't change the size of the bag.
 */
int RandomBag::size() const {
    return elems.size();
}

bool RandomBag::isEmpty() const {
    return size() == 0;
}
