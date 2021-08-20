#include "validation.h"

bool validate_01(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator){
    if (indicator) {
        if ((num <= 94) || (num >= 2902 && num <= 2942) || (num >= 5320 && num <= 5393) ||
            (num >= 7902 && num <= 7908) || (num >= 8277 && num <= 8327) || (num >= 11749)) {
            arr[2]++;
            fp->push_back(num);
            return false;
        }
        else {
            arr[0]++;
            return true;
        }
    }
    else {
        if ((num <= 94) || (num >= 2902 && num <= 2942) || (num >= 5320 && num <= 5393) ||
            (num >= 7902 && num <= 7908) || (num >= 8277 && num <= 8327) || (num >= 11749)) {
            arr[1]++;
            return false;
        }
        else {
            arr[3]++;
            fn->push_back(num);
            return true;
        }
    }
}

bool validate_02(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator){
    if (indicator) {
        if ((num <= 128) || (num >= 2130 && num <= 2153) || (num >= 3621 && num <= 3714) ||
            (num >= 5538 && num <= 5557) || (num >= 7376 && num <= 8866) || (num >= 9800)) {
            arr[2]++;
            fp->push_back(num);
            return false;
        }
        else {
            arr[0]++;
            return true;
        }
    }
    else {
        if ((num <= 128) || (num >= 2130 && num <= 2153) || (num >= 3621 && num <= 3714) ||
            (num >= 5538 && num <= 5557) || (num >= 7376 && num <= 8866) || (num >= 9800)) {
            arr[1] ++;
            return false;
        }
        else {
            arr[3]++;
            fn->push_back(num);
            return true;
        }
    }
}

bool validate_03(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator){
    if (indicator) {
        if ((num <= 1147) || (num >= 1944 && num <= 2181) || (num >= 3080 && num <= 3138) ||
            (num >= 3489 && num <= 3658) || (num >= 4072 && num <= 4219) || (num >= 4437 && num <= 5015) ||
            (num >= 5546 && num <= 5704) || (num >= 6144)) {
            arr[2]++;
            fp->push_back(num);
            return false;
        }
        else {
            arr[0]++;
            return true;
        }
    }
    else {
        if ((num <= 1147) || (num >= 1944 && num <= 2181) || (num >= 3080 && num <= 3138) ||
            (num >= 3489 && num <= 3658) || (num >= 4072 && num <= 4219) || (num >= 4437 && num <= 5015) ||
            (num >= 5546 && num <= 5704) || (num >= 6144)) {
            arr[1] ++;
            return false;
        }
        else {
            arr[3]++;
            fn->push_back(num);
            return true;
        }
    }
}

bool door_close_01(int num){
    if ((num <= 186) || (num >= 493 && num <= 702) || (num >= 917 && num <= 1096) || (num >= 1333 && num <= 1516) ||
    (num >= 1729 && num <= 2006) || (num >= 2273 && num <= 2488) || (num >= 2744 && num <= 2978) || (num >= 3248)){
        return true;
    }
    else{
        return false;
    }
}

bool validate_door_01(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator){
    if (indicator) {
        if ((num <= 186) || (num >= 1333 && num <= 1516) || (num >= 3248)) {
            arr[2]++;
            fp->push_back(num);
            return false;
        }
        else {
            arr[0]++;
            return true;
        }
    }
    else {
        if ((num <= 186) || (num >= 1333 && num <= 1516) || (num >= 3248)) {
            arr[1] ++;
            return false;
        }
        else {
            arr[3]++;
            fn->push_back(num);
            return true;
        }
    }
}

bool door_close_02(int num){
    if ((num <= 102) || (num >= 394 && num <= 668) || (num >= 937 && num <= 1123) || (num >= 1460 && num <= 1716) ||
    (num >= 2104 && num <= 2354) || (num >= 2621 && num <= 2804) || (num >= 3120 && num <= 3388) ||
    (num >= 3713 && num <= 4151) || (num >= 4462 && num <= 4584) || (num >= 4782)){
        return true;
    }
    else{
        return false;
    }
}

bool validate_door_02(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator){
    if (indicator) {
        if ((num <= 102) || (num >= 2621 && num <= 2804) || (num >= 4782)) {
            arr[2]++;
            fp->push_back(num);
            return false;
        }
        else {
            arr[0]++;
            return true;
        }
    }
    else {
        if ((num <= 102) || (num >= 2621 && num <= 2804) || (num >= 4782)) {
            arr[1] ++;
            return false;
        }
        else {
            arr[3]++;
            fn->push_back(num);
            return true;
        }
    }
}
