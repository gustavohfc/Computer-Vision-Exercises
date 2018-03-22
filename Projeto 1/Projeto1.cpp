#include <opencv2/highgui.hpp>
#include <iostream>
#include <cmath>

cv::Mat frame;
cv::Vec3b selected_pixel;
bool changePixelsToRed = false;


void updateFrame()
{
    if(changePixelsToRed)
    {
        frame.forEach<cv::Vec3b>
        (
            [] (cv::Vec3b &pixel, const int * position)
            {
                int distance = sqrt(pow(pixel.val[0] - selected_pixel[0], 2) +
                                    pow(pixel.val[1] - selected_pixel[1], 2) +
                                    pow(pixel.val[2] - selected_pixel[2], 2));

                if(distance < 13)
                {
                    pixel.val[0] = 0;
                    pixel.val[1] = 0;
                    pixel.val[2] = 255;
                }
            }
        );
    }

    cv::imshow("Projeto 1", frame);
}


void mouseCallbackFunc(int event, int x, int y, int, void *)
{
    if(event != cv::EVENT_LBUTTONDOWN)
        return;

    if(frame.channels() == 1)
    {
        selected_pixel.val[0] = frame.at<char>(y, x);
        selected_pixel.val[1] = 0;
        selected_pixel.val[2] = 0;
        
        std::cout << "Intensidade do pixel (x=" << x << ", y=" << y << ") = "
                  << (uint) selected_pixel.val[0] << std::endl << std::endl;
    }
    else if(frame.channels() == 3)
    {
        selected_pixel = frame.at<cv::Vec3b>(y, x);

        std::cout << "O valor RGB do pixel (x=" << x << ", y=" << y << ") é" << std::endl
                  << "R = " << (uint) selected_pixel.val[2] << std::endl
                  << "G = " << (uint) selected_pixel.val[1] << std::endl
                  << "B = " << (uint) selected_pixel.val[0] << std::endl << std::endl;
    }

    changePixelsToRed = true;
    updateFrame();
}


int main ()
{
    cv::namedWindow("Projeto 1", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Projeto 1", mouseCallbackFunc);

    cv::VideoCapture cap;
    cap.open(0);

    if(!cap.isOpened())
    {
        std::cout << "Não foi possível abrir a câmera." << std::endl;
        return -1;
    }

    while(true)
    {
        cap >> frame;

        if(frame.empty())
            break;

        updateFrame();

        if((char) cv::waitKey(33) >= 0)
            break;
    }

    return 0;
}
