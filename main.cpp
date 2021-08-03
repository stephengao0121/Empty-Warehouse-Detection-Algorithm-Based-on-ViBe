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

bool answer(int frame_num){
    return 0;
}

Mat fd(Mat &currentFrame, Mat &rawFrame){
    Mat currentFrameGray, frameDiff;
//    cv::cvtColor(currentFrame, currentFrameGray, COLOR_BGR2GRAY);
    //GaussianBlur(currentFrameGray, currentFrameGray, Size(21, 21), 0, 0);
    cv::absdiff(rawFrame, currentFrame, frameDiff);   //原始与当前做差
    cv::threshold(frameDiff, frameDiff, 70, 255, THRESH_BINARY);//全局固定阈值,这个阈值的选择计算以及面对光照的影响
    //cv::threshold(frameDiff, frameDiff, 0, 255, CV_THRESH_BINARY| CV_THRESH_OTSU);//otsu在场景中没有东西的时候效果很差，也许是阈值处理不合适。有东西时还可以
    //cv::adaptiveThreshold(frameDiff, frameDiff, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, -2);//效果不好
    //先膨胀后腐蚀排除小黑点，反过来是排除小亮点
    cv::erode(frameDiff, frameDiff, cv::Mat());//腐蚀 减少很多点
    cv::dilate(frameDiff, frameDiff, cv::Mat());//膨胀   //别人是相反的
    return frameDiff;
}

int main(int argc, char* argv[])
{
    Mat frame, gray, FGModel, raw_frame, FDModel;
    VideoCapture capture;
    capture = VideoCapture(R"(C:\Users\stephen.gao\Desktop\c\test01.avi)");
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

        /* 直方图均衡光照.*/
//        Mat imageRGB[3];
//        split(frame, imageRGB);
//        for (auto & i : imageRGB){
//            equalizeHist(i, i);
//        }
//        merge(imageRGB, 3, frame);

        /* gamma correction. (best)*/
        Mat X, I;
        frame.convertTo(X, CV_32FC1);
        float gamma = 1.5;
        pow(X, gamma, I);
        norm(I, frame);

        /* log 变换增强.*/
//        double temp = 255 / log(256);
//        Mat imglog(frame.size(), CV_32FC3);
//        for (int i = 0; i < frame.rows; i++){
//            for (int j = 0; j < frame.cols; j++){
//                imglog.at<Vec3f>(i, j)[0] = temp * log(1 + frame.at<Vec3b>(i, j)[0]);
//                imglog.at<Vec3f>(i, j)[1] = temp * log(1 + frame.at<Vec3b>(i, j)[1]);
//                imglog.at<Vec3f>(i, j)[2] = temp * log(1 + frame.at<Vec3b>(i, j)[2]);
//            }
//        }
//        normalize(imglog, imglog, 0, 255, NORM_MINMAX);
//        convertScaleAbs(imglog, frame);

        /* RGB归一化去除光照.*/
//        Mat src(frame.size(), CV_32FC3);
//        for (int i = 0; i < frame.rows; i++){
//            for (int j = 0; j < frame.cols; j++){
//                src.at<Vec3f>(i, j)[0] = 255 * (float)frame.at<Vec3b>(i, j)[0] / ((float)frame.at<Vec3b>(i, j)[0] + (float)frame.at<Vec3b>(i, j)[1] + (float)frame.at<Vec3b>(i, j)[2] + 0.01);
//                src.at<Vec3f>(i, j)[1] = 255 * (float)frame.at<Vec3b>(i, j)[1] / ((float)frame.at<Vec3b>(i, j)[0] + (float)frame.at<Vec3b>(i, j)[1] + (float)frame.at<Vec3b>(i, j)[2] + 0.01);
//                src.at<Vec3f>(i, j)[2] = 255 * (float)frame.at<Vec3b>(i, j)[2] / ((float)frame.at<Vec3b>(i, j)[0] + (float)frame.at<Vec3b>(i, j)[1] + (float)frame.at<Vec3b>(i, j)[2] + 0.01);
//            }
//        }
//        normalize(src, src, 0, 255, NORM_MINMAX);
//        convertScaleAbs(src, frame);

        cvtColor(frame(Rect(120, 0, 520, 480)), gray, COLOR_RGB2GRAY);
        if (frame_num == 1)
        {
            vibe.init(gray);
            /* Take first 3 frames to construct the model.*/
//            capture >> frame2;
//            capture >> frame3;
//            cvtColor(frame2(Rect(120, 0, 520, 480)), gray2, COLOR_RGB2GRAY);
//            cvtColor(frame3(Rect(120, 0, 520, 480)), gray3, COLOR_RGB2GRAY);
//            vibe.ProcessFirstFewFrames(gray, gray2, gray3);

            /* Take only the first frame to construct the model.*/
            vibe.ProcessFirstFrame(gray);

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

            /* Start Bayes decision after first 2000 frames. Before that, use ViBe || fd.*/
            if (frame_num <= 2000){
                indicator = vibe_indicator || fd_indicator;
            }
            else{
                vector<int> vec = {vibe_indicator, fd_indicator};
                indicator = NBClassifier.predict(&vec);
            }

            /* check tp, tn, fp, fn frames.*/
            vector<int> vec = {vibe_indicator, fd_indicator, validate_01(frame_num, stds, &fp, &fn, indicator)};  /* function to validate test01.avi.*/
//            vector<int> vec = {vibe_indicator, fd_indicator, validate_02(frame_num, stds, &fp, &fn, indicator)};  /* function to validate test02.avi.*/
//            vector<int> vec = {vibe_indicator, fd_indicator, validate_03(frame_num, stds, &fp, &fn, indicator)};  /* function to validate test03.avi.*/
            NBClassifier.fit(&vec);
        }

        /* Used to check problematic frames.*/
//        if (frame_num >= 9050 && frame_num <= 10000) {
//            cout << "required frame reached." << endl;
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
        if (waitKey(1) == 27) {
            break;
        }

        frame_num ++;
    }

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
