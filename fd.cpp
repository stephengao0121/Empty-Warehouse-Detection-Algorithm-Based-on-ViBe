#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int fd(){
    cv::cvtColor(currentFrame, currentFrameGray, CV_BGR2GRAY);
    //GaussianBlur(currentFrameGray, currentFrameGray, Size(21, 21), 0, 0);
    cv::absdiff(rawFrame, currentFrameGray, frameDiff);   //原始与当前做差
    cv::threshold(frameDiff, frameDiff, 70, 255, CV_THRESH_BINARY);//全局固定阈值,这个阈值的选择计算以及面对光照的影响
    //cv::threshold(frameDiff, frameDiff, 0, 255, CV_THRESH_BINARY| CV_THRESH_OTSU);//otsu在场景中没有东西的时候效果很差，也许是阈值处理不合适。有东西时还可以
    //cv::adaptiveThreshold(frameDiff, frameDiff, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, -2);//效果不好
    //先膨胀后腐蚀排除小黑点，反过来是排除小亮点
    cv::erode(frameDiff, frameDiff, cv::Mat());//腐蚀 减少很多点
    cv::dilate(frameDiff, frameDiff, cv::Mat());//膨胀   //别人是相反的
}

