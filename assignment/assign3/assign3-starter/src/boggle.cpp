/* 
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
#include "queue.h"
using namespace std;

/*  given word str, return the
 *  corresponding points it score
 */
int points(string str) {
    if(str.size() < 4){
        return 0;
    }else{
        return str.size() -3;
    }
    return 0;
}

bool inBoundary(const Grid<char>& board, GridLocation now){
    if(now.row >= 0 && now.row < board.height() && now.col >= 0 && now.col < board.width()){
        return true;
    }
    return false;
}

Queue<GridLocation> generateNextValidLocation(const Grid<char>& board, GridLocation now){
    Queue<GridLocation> ValidLocationQueue;
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if(i != 0 || j != 0){
                GridLocation choice{now.row + i, now.col + j};
                if(inBoundary(board, choice)){
                    ValidLocationQueue.enqueue(choice);
                }
            }
        }
    }

    return ValidLocationQueue;
}

bool validWord(string word, const Lexicon& lex){
    if(word.size() < 4 || (!lex.contains(word))){
        return false;
    }
    return true;
}
/* 给定一个位置now，到达now之前构建的string construction， 达到now之前已经找到的word的集合result
* 本次探索中已经访问过的矩阵visited，给定的棋盘board和字典lex，从位置now开始，继续寻找合法的word，并将其放入result集合*/
void scoreBoardHelp(GridLocation now, string construction, Set<string>& result, Grid<bool>& visited, const Grid<char>& board, const Lexicon& lex){
    if(validWord(construction, lex)){ //检查达到位置now前构建的string是否valid
        if(!result.contains(construction)){ //其实不需要这一步判断，因为使用的是set，但是为了代码阅读性
            result.add(construction);
        }
        // 此时仍需继续探索，因为可能存在以construction为前缀的合法word
    }

    // 如果当前位置超出了边界，不需要继续探索
    if(!inBoundary(board, now)){
        return;
    }

    //如果当前位置没有超出边界
    //如果当前位置在这一次的探索中已经访问过
    if(visited[now.row][now.col]){
        return; //直接结束本次探索，因为同一位置在构建一个单词时只能使用一次
    }

    // 如果前一步构建的单词在字典中没有以它为前缀的单词，不需要继续探索
    if(!lex.containsPrefix(construction)){
        return;
    }

    //此时访问当前位置
    visited[now.row][now.col] = true;
    construction += board[now.row][now.col];

    //往当前位置的相邻位置探索
    Queue<GridLocation> validLocations = generateNextValidLocation(board, now);
    while(!validLocations.isEmpty()){
        GridLocation nextLocation = validLocations.dequeue();
        scoreBoardHelp(nextLocation, construction, result, visited, board, lex);
    }

    //当之前已经构建了string construction，并继续从位置now出发的所有探索都结束时
    //释放位置now， 以供其他探索
    visited[now.row][now.col] = false;

}

/* 
 *
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    Grid<bool> visited;
    //默认是false；
    visited.resize(board.height(), board.width());

    Set<string> result;
    for(int i = 0; i < board.height(); i++){
        for(int j = 0; j < board.width(); j++){
            GridLocation cube{i, j};
            scoreBoardHelp(cube, "", result, visited, board, lex);
        }
    }
    int score = 0;
    for(string str: result){
        score += points(str);
    }

    return score;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}

STUDENT_TEST("check Point function"){
    string empty;
    EXPECT_EQUAL(points(empty), 0);

    string three{"fuc"};
    EXPECT_EQUAL(points(three), 0);

    string four{"fucc"};
    EXPECT_EQUAL(points(four), 1);
}

STUDENT_TEST("check inBoundary function"){
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    GridLocation leftUp{0, 0};
    GridLocation overLeft{0, -1};
    GridLocation right{0, 3};
    GridLocation overHeight{-1,2};
    EXPECT(inBoundary(board, leftUp));
    EXPECT(!inBoundary(board, overLeft));
    EXPECT(inBoundary(board, right));
    EXPECT(!inBoundary(board, overHeight));
}

STUDENT_TEST("check generateNextValidLocation"){
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(generateNextValidLocation(board, {0, 0}).size(), 3);
    EXPECT_EQUAL(generateNextValidLocation(board, {1, 0}).size(), 5);
    EXPECT_EQUAL(generateNextValidLocation(board, {1, 1}).size(), 8);
}


