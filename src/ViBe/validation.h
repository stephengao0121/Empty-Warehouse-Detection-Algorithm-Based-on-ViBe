
#ifndef C_VALIDATION_H
#define C_VALIDATION_H

#include <iostream>
#include <vector>

using namespace std;

bool validate_01(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator = false);
bool validate_02(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator = false);
bool validate_03(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator = false);

bool door_close_01(int num);
bool validate_door_01(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator = false);
bool door_close_02(int num);
bool validate_door_02(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator = false);
bool door_close_03(int num);
bool validate_door_03(int num, int *arr, vector<int> *fp, vector<int> *fn, bool indicator = false);

#endif //C_VALIDATION_H
