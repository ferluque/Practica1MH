//
// Created by Fernando on 30/03/2022.
//

#ifndef MDD_UTILS_H
#define MDD_UTILS_H
#include <vector>
using namespace std;
float avg(vector<float> v) {
    float sum = 0.0;
    for (float f: v)
        sum += f;
    return sum/v.size();
}

#endif //MDD_UTILS_H
