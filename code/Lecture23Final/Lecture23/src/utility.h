/*
 * This file contains supporting code that we used to help
 * us with sorting.

 * We did not write this code in class.
 */

#pragma once

#include "vector.h"

// function prototype declarations
void fillRandomIntVector(Vector<int>& v, int length);
void fillSortedIntVector(Vector<int>& v, int length);
bool isSorted(const Vector<int>& v);
void swap(Vector<int>& v, int i, int j);
