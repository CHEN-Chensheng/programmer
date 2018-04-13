//
// Created by johnny on 3/29/18.
//

#ifndef DATASTRUCTUREALGORITHMANALYSISINCPP_SORT_H
#define DATASTRUCTUREALGORITHMANALYSISINCPP_SORT_H
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class sort{
public:
    sort();
    ~sort();
    void insertionSort(vector<T> &a);
    void shellSort(vector<T> &a);
    void print(vector<T> &a);
};

#endif //DATASTRUCTUREALGORITHMANALYSISINCPP_SORT_H
