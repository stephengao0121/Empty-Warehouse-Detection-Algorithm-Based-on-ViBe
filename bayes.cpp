#include "bayes.h"


Bayes::Bayes() {}

Bayes::~Bayes() {
    delete [] x;
    delete [] y;
    delete [] xy;
}

void Bayes::init() {
    x = new vector<int> (2, 1);
    y = new vector<int> (2, 1);
    xy = new vector<vector<int>> (2, vector<int> (2, 1));
}

float Bayes::get_px(int value) {
    return float(x->at(value)) / float(x->at(0) + x->at(1));
}

float Bayes::get_py(int value) {
    return float(y->at(value)) / float(y->at(0) + y->at(1));
}

float Bayes::get_pxy(int xvalue, int yvalue) {
    return float(xy->at(xvalue)[yvalue]) / float(xy->at(0)[yvalue] + xy->at(1)[yvalue]);
}

float Bayes::get_pyx(int yvalue, int xvalue) {
    return float(xy->at(xvalue)[yvalue]) / float(xy->at(xvalue)[0] + xy->at(xvalue)[1]);
}

void Bayes::update(int xvalue, int yvalue) {
    x->at(xvalue) ++;
    y->at(yvalue) ++;
    xy->at(xvalue)[yvalue] ++;
}
