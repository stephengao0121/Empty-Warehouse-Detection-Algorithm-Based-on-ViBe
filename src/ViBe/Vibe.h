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

#include <iostream>
#include <cstdio>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

// the Default Number of pixel's samples
#define DEFAULT_NUM_SAMPLES  20

// the Default Match Number of make pixel as Background
#define DEFAULT_MIN_MATCHES  2

// the Default Radius of pixel value
#define DEFAULT_RADIUS  20

// the Default the probability of random sample
#define DEFAULT_RANDOM_SAMPLE  20

class ViBe
{
public:
    ViBe(int num_sam = DEFAULT_NUM_SAMPLES,
         int min_match = DEFAULT_MIN_MATCHES,
         int r = DEFAULT_RADIUS,
         int rand_sam = DEFAULT_RANDOM_SAMPLE);
    ~ViBe(void);

    // Init Background Model.
    void init(Mat img);

    // Process First Frame of Video Query
    void ProcessFirstFrame(Mat img);

    // Run the ViBe Algorithm: Extract Foreground Areas & Update Background Model Sample Library.
    void Run(Mat img);

    // get Foreground Model Binary Image.
    Mat getFGModel();

    // Delete Sample Library.
    void deleteSamples();

    // x's neighborhood points
    int c_xoff[9];

    // y's neighborhood points
    int c_yoff[9];

private:
    // Sample Library, size = img.rows * img.cols *  DEFAULT_NUM_SAMPLES
    unsigned char ***samples;

    // Foreground Model Binary Image
    Mat FGModel;

    // Number of pixel's samples
    int num_samples;

    // Match Number of make pixel as Background
    int num_min_matches;

    // Radius of pixel value
    int radius;

    // the probability of random sample
    int random_sample;
};

