#pragma once

/* Needed for fundamentalwarmup.cpp */
int factorial(int n);
double iterativePower(int base, int exp);
double power(int base, int exp);

/* Needed for balanced.cpp */
bool isBalanced(std::string str);
std::string operatorsOnly(std::string s);
bool checkOperators(std::string s);

/* Needed for sierpinski.cpp */
#include "gwindow.h"
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three);
void drawSierpinskiTriangle(GWindow& window,
                            GPoint one, GPoint two, GPoint three, 
                            int order);


/* Needed for merge.cpp */
#include "queue.h"

Queue<int> merge(Queue<int> a, Queue<int> b);
Queue<int> multiMerge(Vector<Queue<int>>& all);
Queue<int> recMultiMerge(Vector<Queue<int>>& all);

/* Needed for boggle.cpp */
#include "grid.h"
#include "lexicon.h"

int scoreBoard(Grid<char>& board, Lexicon& lex);

/* Needed for voting.cpp */
#include "vector.h"

Vector<int> computePowerIndexes(Vector<int>& blocks);
