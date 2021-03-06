/*
 * HomeBase.hpp
 *
 *  Created on: May 1, 2015
 *      Author: scott
 */

#ifndef HOMEBASE_HPP_
#define HOMEBASE_HPP_

#include "opencv2/core/core.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <iostream>
#include <ctype.h>
#include <vector>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <time.h>
#include <stdlib.h>

#define RES_WIDTH  808
#define RES_HEIGHT 608

#define ALIGN_THRESH 10
#define FWD 1
#define TURN_RIGHT 2
#define TURN_LEFT 3
#define FARAWAY 5000
#define CLOSE 2000
#define NEAR_DIST 1000

#define FRONT 1
#define BACK 2
#define CANT_FIND -12345
#define FRONT_ROWS 4
#define FRONT_COLS 6
#define BACK_ROWS 4
#define BACK_COLS 5
#define FRONT_SZIE 180.0
#define BACK_SZIE 200.0
#define TRAIN_DIST5 5000.0
#define TRAIN_DIST10 10000.0
#define TILT_THRESH 35

#define FRONT_TRAIN5 = "./Front_5m.png";
#define FRONT_TRAIN10 = "./Front_10m.png";
#define BACK_TRAIN5 = "./Back_5m.png";
#define BACK_TRAIN10 = "./Back_10m.png";

//vector<int> createCmd(int move, int val);
//vector<vector<int> > generateCommands(float theta, float D, float offset);
//void printCommands(vector<vector<int> > cmds);
//
//
//
//float dist(Point2f p1, Point2f p2);
//vector<Point2f> getCorners(vector<vector<Point2f> >imagePoints, Size boardSize);
//vector<float> getPixDist(vector<Point2f> corners);
//vector<float> computeFocal(vector<float> pixDist);
//float computeDistance(vector<float> pixDist, vector<float> focal);
//vector<float> trainDistance(Size boardSize);
//vector<float> detectBoard(VideoCapture &cap, Size boardSize, vector<float> focal);

#endif /* HOMEBASE_HPP_ */
