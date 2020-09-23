#include <fstream>
#include <iostream>
#include <iomanip>
#include "console.h"
#include "timer.h"
#include "hashset.h"
#include "lexicon.h"
#include "queue.h"
#include "set.h"
#include "vector.h"
#include "grid.h"
#include "filelib.h"
#include "gwindow.h"
#include "gobjects.h"
#include "simpio.h"
#include "ghanoi.h"
#include "prototypes.h"

using namespace std;

// Constant for total number of disks
static const int N = 5;

/* Recursive algorithm:
 *  - Base case: Move single disk to destination
 *  - Recursive case: The goal is to move the largest disk to the
 *                    destination post. To do this, first transfer all
 *                    disks above the largest to the auxiliary post.
 *                    Once those are out of the way, move the largest
 *                    disk to destination post. Repeat this process for
 *                    the next largest disk, swapping aux and src since
 *                    the remaining disks are now stacked on the auxiliary
 *                    post.
 *
 */
void findSolution(int n, char source, char dest, char aux) {
    /* TODO: Implement the recursive solution to the Towers of Hanoi! */
    if (n == 1) {   // base case
        moveSingleDisk(source, dest);
    } else {        // recursive case
        findSolution(n - 1, source, aux, dest); // move top n -1 disks
                                                // out of the way

        moveSingleDisk(source, dest);           // move largest disk to dest

        findSolution(n - 1, aux, dest, source); // move the same n-1 disks
                                                // to the destination now
    }
}

/* Iterative algorithm:
 *  1. Calculate the total number of moves required for n disks.
    2. If n is even, swap destination and auxiliary poles.
    3. for i = 1 to total number of moves:
        if i % 3 == 1:
            perform legal movement between source and destination
        if i % 3 == 2:
            perform legal movement between source and auxiliary
         if i % 3 == 0:
            perform legal movement between auxilliary and destination

   (NOTE: Don't worry if this code doesn't make sense! The main takeaway
    is that the recursive solution is much more elegant + intuitive!)
 */
void findSolutionIterative(int n, char source, char dest, char aux) {
    int numMoves = pow(2, n) - 1; // total number of moves necessary

    // if number of disks is even, swap dest and aux posts
    if (n % 2 == 0) {
        char temp = dest;
        dest = aux;
        aux = temp;
    }

    Stack<int> srcStack;
    for (int i = n; i > 0; i--) {
        srcStack.push(i);
    }
    cout << srcStack << endl;
    Stack<int> destStack;
    Stack<int> auxStack;

    // Determine next move based on how many moves have been made so far
    for (int i = 1; i <= numMoves; i++) {
        switch (i % 3) {
            case 1:
                if (srcStack.isEmpty() ||
                        (!destStack.isEmpty() && srcStack.peek() > destStack.peek())) {
                    srcStack.push(destStack.pop());
                    moveSingleDisk(dest, source);
                } else {
                    destStack.push(srcStack.pop());
                    moveSingleDisk(source, dest);
                }
                break;
            case 2:
                if (srcStack.isEmpty() ||
                        (!auxStack.isEmpty() && srcStack.peek() > auxStack.peek())) {
                    srcStack.push(auxStack.pop());
                    moveSingleDisk(aux, source);
                } else {
                    auxStack.push(srcStack.pop());
                    moveSingleDisk(source, aux);
                }
                break;
            case 0:
                if (destStack.isEmpty() ||
                        (!auxStack.isEmpty() && destStack.peek() > auxStack.peek())) {
                    destStack.push(auxStack.pop());
                    moveSingleDisk(aux, dest);
                } else {
                    auxStack.push(destStack.pop());
                    moveSingleDisk(dest, aux);
                }
                break;
        }
    }
}

void runHanoiDemo() {
    cout << "Towers of Hanoi" << endl;
    initHanoiDisplay(N);
    findSolution(N, 'a', 'c', 'b');
//    findSolutionIterative(N, 'a', 'c', 'b');
}

