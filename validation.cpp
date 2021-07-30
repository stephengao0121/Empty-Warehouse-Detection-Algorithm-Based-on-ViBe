#include "validation.h"

void validate_01(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator){
    if (indicator) {
        if ((num <= 94) || (num >= 2902 && num <= 2942) || (num >= 5320 && num <= 5393) ||
            (num >= 7902 && num <= 7908) || (num >= 8277 && num <= 8327) || (num >= 11749)) {
            arr[2]++;
            fp->push_back(num);
        }
        else {
            arr[0]++;
        }
    }
    else {
        if ((num <= 94) || (num >= 2902 && num <= 2942) || (num >= 5320 && num <= 5393) ||
            (num >= 7902 && num <= 7908) || (num >= 8277 && num <= 8327) || (num >= 11749)) {
            arr[1]++;
        }
        else {
            arr[3]++;
            fn->push_back(num);
        }
    }
}

void validate_02(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator){
    if (indicator) {
        if ((num <= 128) || (num >= 2130 && num <= 2153) || (num >= 3621 && num <= 3714) ||
            (num >= 5538 && num <= 5557) || (num >= 7376 && num <= 8866) || (num >= 9800)) {
            arr[2]++;
            fp->push_back(num);
        }
        else {
            arr[0]++;
        }
    }
    else {
        if ((num <= 128) || (num >= 2130 && num <= 2153) || (num >= 3621 && num <= 3714) ||
            (num >= 5538 && num <= 5557) || (num >= 7376 && num <= 8866) || (num >= 9800)) {
            arr[1] ++;
        }
        else {
            arr[3]++;
            fn->push_back(num);
        }
    }
}

void validate_03(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator){
    if (indicator) {
        if ((num <= 1147) || (num >= 1944 && num <= 2181) || (num >= 3080 && num <= 3138) ||
            (num >= 3489 && num <= 3658) || (num >= 4072 && num <= 4219) || (num >= 4437 && num <= 5015) ||
            (num >= 5546 && num <= 5704) || (num >= 6144)) {
            arr[2]++;
            fp->push_back(num);
        }
        else {
            arr[0]++;
        }
    }
    else {
        if ((num <= 1147) || (num >= 1944 && num <= 2181) || (num >= 3080 && num <= 3138) ||
            (num >= 3489 && num <= 3658) || (num >= 4072 && num <= 4219) || (num >= 4437 && num <= 5015) ||
            (num >= 5546 && num <= 5704) || (num >= 6144)) {
            arr[1] ++;
        }
        else {
            arr[3]++;
            fn->push_back(num);
        }
    }
}
