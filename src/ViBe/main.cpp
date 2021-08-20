/*=================================================================
 * Extract Background & Foreground Model by ViBe Algorithm using OpenCV Library.
 *
 * Copyright (C) 2017 Chandler Geng. All rights reserved.
 *
 *     This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *     You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 59
 * Temple Place, Suite 330, Boston, MA 02111-1307 USA
===================================================================
*/

#include "Vibe.h"
#include "validation.h"


bool white_sum(Mat *img, int threshold){
    int counter= 0;
    Mat_<uchar>::iterator it = img->begin<uchar>();
    Mat_<uchar>::iterator end = img->end<uchar>();
    for(; it != end; it++){
        if (*it == 255) counter ++;
    }
    if (counter >= threshold) return true;
    else return false;
}

void norm(const Mat& src, Mat& dst){
    switch(src.channels()){
        case 1:
            normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
            break;
        case 3:
            normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
            break;
        default:
            src.copyTo(dst);
            break;
    }
}

int main(int argc, char* argv[])
{
    Mat frame, gray, FGModel;
    VideoCapture capture, capture_bg;
    capture = VideoCapture(R"(C:\Users\stephen.gao\Desktop\c\door_test01.mp4)");
    if(!capture.isOpened()) {
        cout << "ERROR: Didn't find this video!" << endl;
        return 0;
    }

    if (!capture.isOpened())
    {
        cout << "No camera or video input!" << endl;
        return -1;
    }

    ViBe vibe;
    bool indicator = false;
    int frame_num = 1;
    int stds[4]{0, 0, 0, 0};
    vector<int> fp, fn;

    while (1)
    {
        capture >> frame;
        if (frame.empty())
            continue;

        /* gamma correction. (best)*/
        Mat X, I;
        frame.convertTo(X, CV_32FC1);
        float gamma = 1.5;
        pow(X, gamma, I);
        norm(I, frame);

        cvtColor(frame(Rect(250, 0, 1030, 720)), gray, COLOR_RGB2GRAY);
        if (frame_num == 1)
        {
            vibe.init(gray);
            /* Take only the first frame to construct the model.*/
            vibe.ProcessFirstFrame(gray);
            cout << "Training ViBe Success." << endl;
        }
        else
        {
            vibe.Run(gray);
            FGModel = vibe.getFGModel();
            morphologyEx(FGModel, FGModel, MORPH_OPEN, Mat());
            imshow("FGModel", FGModel);
            imshow("input", frame(Rect(250, 0, 1030, 720)));

            /* Vibe decision.*/
            for (int i = 0; i < 960; i += 70) {
                indicator = false;
                for (int j = 0; j < 650; j += 70) {
                    Mat square = FGModel(Rect(i, j, 70, 70));
                    if (white_sum(&square, 100)) {
                        indicator = true;
                        break;
                    }
                }
                if (indicator) break;
            }
//            if (frame_num >= 816){
//                cout << "The decision is " << indicator << endl;
//            }

            /* check tp, tn, fp, fn frames, and form a vector of results from both algorithms.*/
//            validate_01(frame_num, stds, &fp, &fn, indicator);  /* function to validate test01.avi.*/
//            validate_02(frame_num, stds, &fp, &fn, indicator);  /* function to validate test02.avi.*/
//            validate_03(frame_num, stds, &fp, &fn, indicator);  /* function to validate test03.avi.*/
//            validate_04(frame_num, stds, &fp, &fn, indicator);  /* function to validate test04.mp4.*/
//            validate_05(frame_num, stds, &fp, &fn, indicator);  /* function to validate test05.mp4.*/
//            validate_06(frame_num, stds, &fp, &fn, indicator);  /* function to validate test06.mp4.*/
//            validate_07(frame_num, stds, &fp, &fn, indicator);  /* function to validate test06.mp4.*/
            if (door_close_01(frame_num)){
                validate_door_01(frame_num, stds, &fp, &fn, indicator);
            }
//            if (door_close_02(frame_num)){
//                validate_door_02(frame_num, stds, &fp, &fn, indicator);
//            }
        }

//        if (frame_num >= 4062 && frame_num <= 5000){
//            cout << "This is the " << frame_num << "th frame" << endl;
//            if (waitKey(0) == 27) {
//                frame_num ++;
//                continue;
//            }
//        }

        /* Final termination condition.*/
        if (frame_num == capture.get(7)) {
            destroyAllWindows();
            break;
        }

        /* Terminate anytime when esc is hit.*/
        if (waitKey(10) == 27) {
            break;
        }

        frame_num ++;
    }

    /* Visualize tp, tn, fp, fn numbers and positions.*/
    cout << "The TP is " << stds[0] << endl;
    cout << "The TN is " << stds[1] << endl;
    cout << "The FP is " << stds[2] << endl;
    cout << "The FN is " << stds[3] << endl;
    if (!fn.empty()) {
        for (int &it : fn) {
            cout << "FN frame number is " << it << endl;
        }
    }
    if (!fp.empty()) {
        for (int &it : fp) {
            cout << "FP frame number is " << it << endl;
        }
    }

    return 0;
}
