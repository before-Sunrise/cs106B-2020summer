/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * 给定一个block投票数的vector和目标block的索引值target,block的投票总和为sumOfBlocks
 * 在枚举到第index个block且之前的投票总和为sum
 * 返回其critical vote数
 */

int computeCriticalVote(int target, Vector<int>& blocks, int sumOfBlocks, int index, int sum){
    if(index == target){ // 跳过索引为target的item
        index++;
    }
    if(sum > (sumOfBlocks / 2)){// 此时不需要再继续搜索，因为target不可能是包含现有coalition的coalition的critical vote
        return 0;
    }
    if(index == (blocks.size())){ //搜索完一个完整的coalition
        if(sum + blocks[target] > (sumOfBlocks / 2)){// 不加target，sum小于等于总和的一半，加了，大于总和的一半
            return 1; //target对于这一coalition是一个critical vote
        }else{
            return 0; //target对于这一coalition不是一个critical vote
        }
    }
    int resultOne = computeCriticalVote(target, blocks, sumOfBlocks, index + 1, sum);
    int resultTwo = computeCriticalVote(target, blocks, sumOfBlocks, index + 1, sum + blocks[index]);

    return resultOne + resultTwo;
}

Vector<int> computePowerIndexes(Vector<int>& blocks){
    int sumOfBlocks = 0, sumOfCriticalVote = 0;
    Vector<int> result(blocks.size(), 0);
    for(int block: blocks){
        sumOfBlocks += block;
    }

    for(int i = 0; i < blocks.size(); i++){
        int criticalVote = computeCriticalVote(i, blocks,sumOfBlocks, 0, 0);
        sumOfCriticalVote += criticalVote;
        result[i] = criticalVote;
    }

    for(int& vote: result){
        vote = vote * 1.0 / sumOfCriticalVote * 100;//避免整数运算的舍入导致结果全为0
    }
    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    /* This is a stress test, which tests the efficiency of your
     * recursive process. Your solution will need to avoid any expensive
     * copying or ADT operations (insertions/removals) in order to be able
     * to find the solution for this voting block setup.
     * 
     * On a standard computer system, an effieicent implementation that avoids
     * unnecessary work will take about 30 seconds - 1 minute to complete this 
     * test.
     */
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    /* Note: Due to rounding errors, the following values don't all add up to 100. */
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    // EXPECT_EQUAL(computePowerIndexes(blocks), expected);
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Time operation on different input size"){
    for(int i = 14; i <= 22; i += 2){
        Vector<int> blocks;
        for (int j = 0; j < i; j++) {
            blocks.add(randomInteger(1, 10));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}


