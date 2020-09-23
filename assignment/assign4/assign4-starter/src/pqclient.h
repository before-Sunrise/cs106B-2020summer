#pragma once

#include "datapoint.h"
#include "vector.h"
#include <istream>


/**
 * This function sorts a Vector of DataPoints by storing them
 * all in a PQueue and then extracting them all, which gives the
 * elements in sorted order.
 */
void pqSort(Vector<DataPoint>& v);


/**
 * Given a stream containing some number of DataPoints, returns the k elements from that
 * data stream that have the highest weight, sorted in descending order of weight.
 *
 * Note that the number k might be higher than the number of elements in the data stream.
 * If that happens, you should return all the elements of the data stream, sorted in
 * descending order by weight.
 *
 * If there are multiple data points that are tied for weight, you can break those ties
 * however you'd like.
 *
 * Be careful: the intent is that you should return the DataPoints with the HIGHEST weights,
 * which contrasts with what the priority queue normally does (it returns elements with the
 * lowest priorities). However, if you think about it, that might make your life a bit
 * easier... :-)
 *
 * This function should run in time O(n log k), where n is the total number of elements in
 * the stream and k is the parameter.
 *
 * @param stream A data stream containing a bunch of DataPoints.
 * @param k The number of elements to read.
 * @return The min{n, k} data points of the stream with the highest weight, sorted in descending
 *         order of weight, where n is the number of items in the stream.
 */
Vector<DataPoint> topK(std::istream& stream, int k);
