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

bool check_open_and_empty(int frame_num){
    if (frame_num == 94 || frame_num == 2942 || frame_num == 5393 || frame_num == 7908 || frame_num == 8327){
        return true;
    }
    return false;
}

int main(int argc, char* argv[])
{
    Mat frame, gray, FGModel, frame2;
    VideoCapture capture;
    capture = VideoCapture(R"(C:\Users\stephen.gao\Desktop\c\test02.avi)");
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
    bool count = true;
    bool indicator = false;
    int frame_num = 1;
    int *stds = new int[4]{0};
    vector<int> fp, fn;
    bool open = false;
    Bayes nemp_openemp;
    nemp_openemp.init();

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
        if (count)
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
            cout<<"Training ViBe Success."<<endl;
            count = false;
        }
        else
        {
            vibe.Run(gray);
            FGModel = vibe.getFGModel();
			morphologyEx(FGModel, FGModel, MORPH_OPEN, Mat());
            imshow("FGModel", FGModel);
            imshow("input", frame(Rect(120, 0, 520, 480)));

            /* Determining empty or not.*/
            for (int i = 0; i < 480; i += 40) {
                indicator = false;
                for (int j = 0; j < 440; j += 40) {
                    Mat square = FGModel(Rect(i, j, 40, 40));
                    if (white_sum(&square, 100)) {
                        indicator = true;
                        break;
                    }
                }
                if (indicator) break;
            }

            /* check tp, tn, fp, fn frames.*/
//            validate_01(frame_num, stds, &fp, &fn, indicator);  /* function to validate test01.avi.*/
            validate_02(frame_num, stds, &fp, &fn, indicator);  /* function to validate test02.avi.*/
//            validate_03(frame_num, stds, &fp, &fn, indicator);  /* function to validate test03.avi.*/


//            if (open && indicator){
//                nemp_openemp.update(1, 1);
//            }
        }
//        open = check_open_and_empty(frame_num);

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
        if (waitKey(5) == 27) {
            break;
        }

        frame_num ++;
    }

    cout << "The TP is " << stds[0] << endl;
    cout << "The TN is " << stds[1] << endl;
    cout << "The FP is " << stds[2] << endl;
    cout << "The FN is " << stds[3] << endl;
    for (int & it : fn){
        cout << "FN frame number is " << it << endl;
    }
    for (int & it : fp){
        cout << "FP frame number is " << it << endl;
    }
//    cout << nemp_openemp.get_pxy(1, 1) << endl;

    delete[] stds;

    return 0;
}
