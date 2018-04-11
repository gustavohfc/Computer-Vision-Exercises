// Common functions

#ifndef COMMON_H
#define COMMON_H

void mouseCallbackFunc(int event, int x, int y, int flags, void *data);

void drawInfo(const cv::Mat& image, cv::Point p1, cv::Point p2, int distance, const char *distance_unit);

#endif  // COMMON_H