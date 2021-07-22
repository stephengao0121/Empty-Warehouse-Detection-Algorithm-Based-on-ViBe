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


ViBe::ViBe(int num_sam, int min_match, int r, int rand_sam) {
    num_samples = num_sam;
    num_min_matches = min_match;
    radius = r;
    random_sample = rand_sam;
    int c_off[9] = {-1, 0, 1, -1, 1, -1, 0, 1, 0};
    for (int i = 0; i < 9; i++) {
        c_xoff[i] = c_yoff[i] = c_off[i];
    }
}

ViBe::~ViBe(void)
{
    deleteSamples();
}

void ViBe::init(Mat img)
{
    // 动态分配三维数组，samples[][][num_samples]存储前景被连续检测的次数
    // Dynamically Assign 3-D Array.
    // sample[img.rows][img.cols][num_samples] is a 3-D Array which includes all pixels' samples.
    samples = new unsigned char **[img.rows];
    for (int i = 0; i < img.rows; i++)
	{
        samples[i] = new uchar *[img.cols];
        for (int j = 0; j < img.cols; j++)
		{
            // 数组中，在num_samples之外多增的一个值，用于统计该像素点连续成为前景的次数；
            // the '+ 1' in 'num_samples + 1', it's used to count times of this pixel regarded as foreground pixel.
            samples[i][j] =new uchar [num_samples + 1];
            for (int k = 0; k < num_samples + 1; k++)
			{
                // 创建样本库时，所有样本全部初始化为0
                // All Samples init as 0 When Creating Sample Library.
                samples[i][j][k] = 0;
			}
		}
	}

    FGModel = Mat::zeros(img.size(),CV_8UC1);
}

void ViBe::ProcessFirstFrame(Mat img)
{
	RNG rng;
	int row, col;

    for(int i = 0; i < img.rows; i++)
	{
        for(int j = 0; j < img.cols; j++)
		{
            for(int k = 0 ; k < num_samples; k++)
			{
                // 随机选择num_samples个邻域像素点，构建背景模型
                // Randomly pick up num_samples pixel in neighbourhood to construct the model
                int random;
                random = rng.uniform(0, 9); row = i + c_yoff[random];
                random = rng.uniform(0, 9); col = j + c_xoff[random];

                // 防止选取的像素点越界
                // Protect Pixel from Crossing the border
                if (row < 0)  row = 0;
                if (row >= img.rows)  row = img.rows - 1;
                if (col < 0)  col = 0;
                if (col >= img.cols)   col = img.cols - 1;

                // 为样本库赋随机值
                // Set random pixel's Value for Sample Library.
                samples[i][j][k] = img.at<uchar>(row, col);
			}
		}
	}
}

void ViBe::ProcessFirstFewFrames(Mat img1, Mat img2, Mat img3)
{
    RNG rng;
    int row, col;

    for(int i = 0; i < img1.rows; i++)
    {
        for(int j = 0; j < img1.cols; j++)
        {
            for(int k = 0 ; k < num_samples; k++)
            {
                // 随机选择num_samples个邻域像素点，构建背景模型
                // Random pick up num_samples pixel in neighbourhood to construct the model
                int random;
                random = rng.uniform(0, 9); row = i + c_yoff[random];
                random = rng.uniform(0, 9); col = j + c_xoff[random];

                // 防止选取的像素点越界
                // Protect Pixel from Crossing the border
                if (row < 0)  row = 0;
                if (row >= img1.rows)  row = img1.rows - 1;
                if (col < 0)  col = 0;
                if (col >= img1.cols)   col = img1.cols - 1;

                // 为样本库赋三帧平均值
                // Set average pixel's Value for Sample Library.
                samples[i][j][k] = saturate_cast<uchar>((img1.at<uchar>(row, col)+ img2.at<uchar>(row, col) + img3.at<uchar>(row, col))/3);
            }
        }
    }
}

void ViBe::Run(Mat img)
{
    RNG rng;
    int k = 0, dist = 0, matches = 0;
    for(int i = 0; i < img.rows; i++)
	{
        for(int j = 0; j < img.cols; j++)
        {
            for(k = 0, matches = 0; matches < num_min_matches && k < num_samples; k++)
            {
                dist = abs(samples[i][j][k] - img.at<uchar>(i, j));
                if (dist < radius)
                    matches++;
            }
            if (matches >= num_min_matches)
            {
                // 已经认为是背景像素，故该像素的前景统计次数置0
                // This pixel has regard as a background pixel, so the count of this pixel's foreground statistic set as 0
//                samples[i][j][num_samples]=0;

                // 该像素点被的前景模型像素值置0
                // Set Foreground Model's pixel as 0
                FGModel.at<uchar>(i, j) = 0;
            }
            else
            {
                // 已经认为是前景像素，故该像素的前景统计次数+1
                // This pixel has regard as a foreground pixel, so the count of this pixel's foreground statistic plus 1
//                samples[i][j][num_samples]++;

                // 该像素点被的前景模型像素值置255
                // Set Foreground Model's pixel as 255
                FGModel.at<uchar>(i, j) = 255;

                // 如果某个像素点连续50次被检测为前景，则认为一块静止区域被误判为运动，将其更新为背景点
                // if this pixel is regarded as foreground for more than 50 times, then we regard this static area as dynamic area by mistake, and Run this pixel as background one.
//                if(samples[i][j][num_samples] > 50)
//                {
//                    int random = rng.uniform(0, num_samples);
//                    samples[i][j][random]=img.at<uchar>(i, j);
//                }
            }

            if (matches >= num_min_matches)
            {
                // 已经认为该像素是背景像素，那么它有 1 / φ 的概率去更新自己的模型样本值
                // This pixel is already regarded as Background Pixel, then it has possibility of 1/φ to Run its model sample's value.
                int random = rng.uniform(0, random_sample);
                if (random == 0)
                {
                    random = rng.uniform(0, num_samples);
                    samples[i][j][random] = img.at<uchar>(i, j);
                }

                // 同时也有 1 / φ 的概率去更新它的邻居点的模型样本值
                // At the same time, it has possibility of 1/φ to Run its neighborhood point's sample value.
                random = rng.uniform(0, random_sample);
                if (random == 0)
                {
                    int row, col;
                    random = rng.uniform(0, 9); row = i + c_yoff[random];
                    random = rng.uniform(0, 9); col = j + c_xoff[random];

                    // 防止选取的像素点越界
                    // Protect Pixel from Crossing the border
                    if (row < 0) row = 0;
                    if (row >= img.rows)  row = img.rows - 1;
                    if (col < 0) col = 0;
                    if (col >= img.cols) col = img.cols - 1;

                    // 为样本库赋随机值
                    // Set random pixel's Value for Sample Library
                    random = rng.uniform(0, num_samples);
                    samples[row][col][random] = img.at<uchar>(i, j);
                }
            }
        }
    }
}

Mat ViBe::getFGModel()
{
    return FGModel;
}

void ViBe::deleteSamples()
{
    delete samples;
}
