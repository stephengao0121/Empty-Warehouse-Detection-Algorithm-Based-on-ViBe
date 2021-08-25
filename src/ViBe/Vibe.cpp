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
    // Dynamically Assign 3-D Array.
    // sample[img.rows][img.cols][num_samples] is a 3-D Array which includes all pixels' samples.
    samples = new unsigned char **[img.rows];
    for (int i = 0; i < img.rows; i++)
	{
        samples[i] = new uchar *[img.cols];
        for (int j = 0; j < img.cols; j++)
		{
            // the '+ 1' in 'num_samples + 1', it's used to count times of this pixel regarded as foreground pixel.
            samples[i][j] =new uchar [num_samples + 1];
            for (int k = 0; k < num_samples + 1; k++)
			{
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
                // Randomly pick up num_samples pixel in neighbourhood to construct the model
                int random;
                random = rng.uniform(0, 9); row = i + c_yoff[random];
                random = rng.uniform(0, 9); col = j + c_xoff[random];

                // Protect Pixel from Crossing the border
                if (row < 0)  row = 0;
                if (row >= img.rows)  row = img.rows - 1;
                if (col < 0)  col = 0;
                if (col >= img.cols)   col = img.cols - 1;

                // Set random pixel's Value for Sample Library.
                samples[i][j][k] = img.at<uchar>(row, col);
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
                // Set Foreground Model's pixel as 0
                FGModel.at<uchar>(i, j) = 0;
            }
            else
            {
                // Set Foreground Model's pixel as 255
                FGModel.at<uchar>(i, j) = 255;
            }

            if (matches >= num_min_matches)
            {
                // This pixel is already regarded as Background Pixel, then it has possibility of 1/φ to Run its model sample's value.
                int random = rng.uniform(0, random_sample);
                if (random == 0)
                {
                    random = rng.uniform(0, num_samples);
                    samples[i][j][random] = img.at<uchar>(i, j);
                }

                // At the same time, it has possibility of 1/φ to Run its neighborhood point's sample value.
                random = rng.uniform(0, random_sample);
                if (random == 0)
                {
                    int row, col;
                    random = rng.uniform(0, 9); row = i + c_yoff[random];
                    random = rng.uniform(0, 9); col = j + c_xoff[random];

                    // Protect Pixel from Crossing the border
                    if (row < 0) row = 0;
                    if (row >= img.rows)  row = img.rows - 1;
                    if (col < 0) col = 0;
                    if (col >= img.cols) col = img.cols - 1;

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
