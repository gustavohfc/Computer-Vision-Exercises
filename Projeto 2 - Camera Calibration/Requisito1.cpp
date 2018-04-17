#include <iostream>
#include <opencv2/highgui.hpp>

#include "common.h"


int main(int argc, char** argv)
{
    cv::Mat frame;
    cv::Vec<cv::Point, 2> line_points { { -1, -1 }, {-1 , -1} };

    cv::VideoCapture cap(0);

    if (!cap.isOpened())
    {
        std::cout << "\n\nNão foi possível abrir a câmera.\n\n";
        return -1;
    }

    cv::namedWindow("Requisito 1", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Requisito 1", mouseCallbackFunc, &line_points);

    do
    {
        cap >> frame;

        drawInfo(frame, line_points[0], line_points[1],
                 cv::norm(line_points[0] - line_points[1]), "pixels");

        if (frame.empty())
            break;

        cv::imshow("Requisito 1", frame);

    } while ((char) cv::waitKey(33) < 0);

    return 0;
}
