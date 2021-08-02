#include "bayes.h"


NaiveBayes::NaiveBayes(int cond) {
    cond_num = cond;
    var_num = 1;
    variables = new vector<int>(2, 1);
    conditions = new vector<vector<vector<int>> *>(cond_num, nullptr);
    for (int i = 0; i < cond_num; i++){
        auto *vec = new vector<vector<int>>(2, vector<int>(2, 1));
        if (vec == nullptr) return;
        conditions->at(i) = vec;
    }
}

void NaiveBayes::fit(vector<vector<int>> *data) {
    for (auto i : *data){
        variables->at(i[2]) ++;
        conditions->at(0)->at(i[0])[i[2]] ++;
        conditions->at(1)->at(i[1])[i[2]] ++;
    }
}

int NaiveBayes::predict(vector<int> *data) const {
    auto *probs = new float [2];
    auto var_probs = get_var_probs();
    auto cond_probs = get_cond_probs();

    for (int i = 0; i < 1; i++){
        float a = 1;
        a *= var_probs->at(i);
        for (int j = 0; j < cond_num; j++){
            a *= cond_probs->at(j)->at(0)[data->at(i)];
            a *= cond_probs->at(j)->at(1)[data->at(i)];
        }
        probs[i] = a;
    }
    if (probs[0] > probs[1]) return 0;
    else return 1;
}

vector<float>* NaiveBayes::get_var_probs() const {
    auto *probs = new vector<float>(2, 0);
    probs->at(0) = float(variables->at(0)) / float(variables->at(0) + variables->at(1));
    probs->at(1) = float(variables->at(1)) / float(variables->at(0) + variables->at(1));
    return probs;
}

vector<vector<vector<float>> *>* NaiveBayes::get_cond_probs() const {
    auto *probs = new vector<vector<vector<float>> *>(cond_num, nullptr);
    for (int i = 0; i < cond_num; i++){
        auto *vec = new vector<vector<float>>(2, vector<float>(2, 0));
        if (vec == nullptr) return nullptr;
        vec->at(0)[0] = float(conditions->at(i)->at(0)[0]) / float(conditions->at(i)->at(0)[0] + conditions->at(i)->at(1)[0]);
        vec->at(0)[1] = float(conditions->at(i)->at(0)[1]) / float(conditions->at(i)->at(0)[1] + conditions->at(i)->at(1)[1]);
        vec->at(1)[0] = float(conditions->at(i)->at(1)[0]) / float(conditions->at(i)->at(0)[0] + conditions->at(i)->at(1)[0]);
        vec->at(1)[1] = float(conditions->at(i)->at(1)[1]) / float(conditions->at(i)->at(0)[1] + conditions->at(i)->at(1)[1]);
        probs->at(i) = vec;
    }

    return probs;
}


