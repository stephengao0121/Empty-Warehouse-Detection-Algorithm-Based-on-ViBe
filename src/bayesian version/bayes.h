
#ifndef C_BAYES_H
#define C_BAYES_H

#include <iostream>
#include <vector>

using namespace std;

class NaiveBayes{
public:
    explicit NaiveBayes(int cond = 1);
    ~NaiveBayes() = default;
    void fit(vector<int> *);
    int predict(vector<int> *) const;
    vector<float>* get_var_probs() const;
    vector<vector<vector<float>> *>* get_cond_probs() const;
    vector<int> * get_variables();
    vector<vector<vector<int>> *> * get_conditions();
private:
    int cond_num;
    int var_num;
    vector<int> *variables;
    vector<vector<vector<int>> *> *conditions;
};

#endif //C_BAYES_H
