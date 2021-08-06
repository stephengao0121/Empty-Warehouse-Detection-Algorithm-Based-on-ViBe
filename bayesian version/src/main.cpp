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
#include "bayes.h"

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

Mat fd(Mat &currentFrame, Mat &rawFrame){
    Mat currentFrameGray, frameDiff;
    cv::absdiff(rawFrame, currentFrame, frameDiff);
    cv::threshold(frameDiff, frameDiff, 70, 255, THRESH_BINARY);
    cv::erode(frameDiff, frameDiff, cv::Mat());
    cv::dilate(frameDiff, frameDiff, cv::Mat());
    return frameDiff;
}

int main(int argc, char* argv[])
{
    Mat frame, gray, FGModel, raw_frame, FDModel;
    VideoCapture capture;
    capture = VideoCapture(R"(C:\Users\stephen.gao\Desktop\c\test03.avi)");
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
    bool vibe_indicator = false;
    bool fd_indicator = false;
    bool indicator = false;
    int frame_num = 1;
    int stds[4]{0, 0, 0, 0};
    vector<int> fp, fn;
    NaiveBayes NBClassifier(2);

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

        cvtColor(frame(Rect(120, 0, 520, 480)), gray, COLOR_RGB2GRAY);
        if (frame_num == 1)
        {
            vibe.init(gray);
            /* Take only the first frame to construct the model.*/
            vibe.ProcessFirstFrame(gray);
            /* Construct background frame of fd.*/
            gray.copyTo(raw_frame);
            cout << "Training ViBe Success." << endl;
            cout << "Frame Difference Background Construction Success." << endl;
        }
        else
        {
            vibe.Run(gray);
            FGModel = vibe.getFGModel();
            morphologyEx(FGModel, FGModel, MORPH_OPEN, Mat());
            FDModel = fd(gray, raw_frame);
            imshow("FGModel", FGModel);
            imshow("FDModel", FDModel);
            imshow("input", frame(Rect(120, 0, 520, 480)));

            /* Vibe decision.*/
            for (int i = 0; i < 480; i += 40) {
                vibe_indicator = false;
                for (int j = 0; j < 440; j += 40) {
                    Mat square = FGModel(Rect(i, j, 40, 40));
                    if (white_sum(&square, 100)) {
                        vibe_indicator = true;
                        break;
                    }
                }
                if (vibe_indicator) break;
            }

            /* FD decision.*/
            for (int i = 0; i < 480; i += 40) {
                fd_indicator = false;
                for (int j = 0; j < 440; j += 40) {
                    Mat square = FDModel(Rect(i, j, 40, 40));
                    if (white_sum(&square, 100)) {
                        fd_indicator = true;
                        break;
                    }
                }
                if (fd_indicator) break;
            }

            /* Start Bayes decision after first 2000 frames. Before that, use vibe || fd.*/
            if (frame_num <= 2000){
                indicator = vibe_indicator || fd_indicator;
            }
            else{
                vector<int> vec = {vibe_indicator, fd_indicator};
                indicator = NBClassifier.predict(&vec);
            }

            /* check tp, tn, fp, fn frames, and form a vector of results from both algorithms.*/
//            vector<int> vec = {vibe_indicator, fd_indicator, validate_01(frame_num, stds, &fp, &fn, indicator)};  /* function to validate test01.avi.*/
//            vector<int> vec = {vibe_indicator, fd_indicator, validate_02(frame_num, stds, &fp, &fn, indicator)};  /* function to validate test02.avi.*/
            vector<int> vec = {vibe_indicator, fd_indicator, validate_03(frame_num, stds, &fp, &fn, indicator)};  /* function to validate test03.avi.*/

            if (frame_num <= 5000) {
                NBClassifier.fit(&vec);
            }
        }

        /* Final termination condition.*/
        if (frame_num == capture.get(7)) {
            destroyAllWindows();
            break;
        }

        /* Terminate anytime when esc is hit.*/
        if (waitKey(5) == 27) {
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
