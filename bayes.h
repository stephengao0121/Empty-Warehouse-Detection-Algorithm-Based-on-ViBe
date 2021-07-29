
#ifndef C_BAYES_H
#define C_BAYES_H

#include <iostream>
#include <vector>

using namespace std;

class Bayes{
public:
    Bayes();
    ~Bayes();
    void init();
    float get_pyx(int, int);
    float get_pxy(int, int);
    float get_px(int);
    float get_py(int);
    void update(int, int);
private:
    vector<int> *y, *x;  /* N(Y), N(X), Y, X = {0, 1}*/
    vector<vector<int>> *xy;  /* N(X|Y), Y, X = {0, 1}*/
};

#endif //C_BAYES_H
