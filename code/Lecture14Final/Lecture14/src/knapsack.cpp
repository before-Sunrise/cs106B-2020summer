#include <iostream>
#include "console.h"
#include "vector.h"
using namespace std;

struct BackpackItem {
    int survivalValue;  // You can assume this value will always >= 0
    int weight;         // You can assume this value will always >= 0
};

int fillBackpack(Vector<BackpackItem>& items, int targetWeight);
int fillBackpackHelper(Vector<BackpackItem>& items, int weight, int curValue, int index);

int fillBackpack(Vector<BackpackItem>& items, int targetWeight, Vector<BackpackItem>& bestBackpackArrangement);
int fillBackpackHelper(Vector<BackpackItem>& items, int weight, int curValue, int index, Vector<BackpackItem>& bestBackpackArrangement);


int fillBackpackHelper(Vector<BackpackItem>& items, int capacityRemaining, int curValue, int index){
    /* Base Case: If there is no more capacity in the backpack to hold things,
     * then we can no longer fit any more value in.
     */
    if (capacityRemaining < 0) {
        return 0;
    }
    /* Base Case: If we have run out of items to consider, then the best value we
     * can get is what we've built up so far.
     */
    else if (index == items.size()){
        return curValue;
    }else {
        /* Choose: Select an item to decide whether to bring along or not.
         */
        BackpackItem curItem = items[index];

        /* Explore: Try including the item and not including it, and keep track
         * of the best possible value in each case. */
        int bestValueWithout = fillBackpackHelper(items,
                                                  capacityRemaining,
                                                  curValue,
                                                  index + 1);

        int bestValueWith = fillBackpackHelper(items,
                                               capacityRemaining - curItem.weight,
                                               curValue + curItem.survivalValue,
                                               index + 1);

        /* Unchoose: No explicit unchoose necessary since no changes to data
         * structures have been made.
         */

        /* The final value we return is the best of the two options we tried. */
        return max(bestValueWith, bestValueWithout);
    }
}

int fillBackpack(Vector<BackpackItem>& items, int targetWeight){
    return fillBackpackHelper(items, targetWeight, 0, 0);
}


// 记录最好方案时选择的物品
int fillBackpackHelper(Vector<BackpackItem>& items, int capacityRemaining, int curValue, int index, Vector<BackpackItem>& bestBackpackArrangement){
    /* Base Case: If there is no more capacity in the backpack to hold things,
     * then we can no longer fit any more value in.
     */
    if (capacityRemaining < 0) {
        return 0;
    }
    /* Base Case: If we have run out of items to consider, then the best value we
     * can get is what we've built up so far.
     */
    else if (index == items.size()){
        return curValue;
    }else {
        /* Choose: Select an item to decide whether to bring along or not.
         */
        BackpackItem curItem = items[index];

        /* Explore: Try including the item and not including it, and keep track
         * of the best possible value in each case. */
        Vector<BackpackItem> bestBackpackWithout = bestBackpackArrangement;
        int bestValueWithout = fillBackpackHelper(items,
                                                  capacityRemaining,
                                                  curValue,
                                                  index + 1,
                                                  bestBackpackWithout);

        Vector<BackpackItem> bestBackpackWith = bestBackpackArrangement;
        bestBackpackWith.add(curItem);
        int bestValueWith = fillBackpackHelper(items,
                                               capacityRemaining - curItem.weight,
                                               curValue + curItem.survivalValue,
                                               index + 1,
                                               bestBackpackWith);

        /* Unchoose: No explicit unchoose necessary since no changes to data
         * structures have been made.
         */

        /* The final value we return is the best of the two options we tried. */
        int newBest = max(bestValueWith, bestValueWithout);

        /* Update the best solution accordingly. */
        if (newBest == bestValueWith) {
            bestBackpackArrangement = bestBackpackWith;
        } else {
            bestBackpackArrangement = bestBackpackWithout;
        }

        return newBest;
    }
}

int fillBackpack(Vector<BackpackItem>& items, int targetWeight, Vector<BackpackItem>& bestBackpackArrangement){
    return fillBackpackHelper(items, targetWeight, 0, 0, bestBackpackArrangement);
}

void knapsackDemo() {
    // solution: 44
    int targetWeight = 18;
    Vector<BackpackItem> items = {
        {12, 4},
        {10,6},
        {8, 5},
        {11, 7},
        {14, 3},
        {7, 1},
        {9, 6}
    };

    // solution: 67
    //        int targetWeight = 25;
    //        Vector<BackpackItem> items = {
    //                                    {5, 6},
    //                                    {20, 15},
    //                                    {3, 11},
    //                                    {50, 12},
    //                                    {5, 6},
    //                                    {4, 11},
    //                                    {15, 13},
    //                                    {12, 7},
    //                                    {6, 17},
    //                                    {7, 13}
    //                                   };

    // solution : 7
    //        int targetWeight = 5;
    //        Vector<BackpackItem> items = {
    //                                    {3, 2},
    //                                    {4, 3},
    //                                    {5, 4},
    //                                    {6, 5}
    //                                   };

    Vector<BackpackItem> backpack;

    cout << "Best solution has a value of: "
         << fillBackpack(items, targetWeight, backpack) << endl;
    cout << endl;

    cout << "Original objects: " << endl;
    for (BackpackItem item : items) {
        cout << "Survival Value: " << item.survivalValue << endl;
        cout << "Weight: " << item.weight << endl;
        cout << endl;
    }

    cout << "Target weight: " << targetWeight << endl << endl;

    cout << "Best backpack: " << endl;
    for (BackpackItem item: backpack) {
        cout << "Survival Value: " << item.survivalValue << endl;
        cout << "Weight: " << item.weight << endl;
        cout << endl;
    }
}
