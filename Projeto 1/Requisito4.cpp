#include <opencv2/highgui.hpp>
#include <iostream>

cv::Mat g_frame;
cv::Vec3b g_selected_pixel;
bool g_is_pixel_selected = false;


/* Change the pixels from g_frame to pure red if the euclidean distance between
 * the pixel and the g_selected_pixel is smaller than 13.
 */
void changePixelsToRed()
{
    g_frame.forEach<cv::Vec3b>
    (
        [] (cv::Vec3b &pixel, const int * position)
        {
            int distance = sqrt(pow(pixel.val[0] - g_selected_pixel[0], 2) +
                                pow(pixel.val[1] - g_selected_pixel[1], 2) +
                                pow(pixel.val[2] - g_selected_pixel[2], 2));

            if (distance < 13)
            {
                pixel.val[0] = 0;
                pixel.val[1] = 0;
                pixel.val[2] = 255;
            }
        }
    );
}


void mouseCallbackFunc(int event, int x, int y, int, void *)
{
    if (event != cv::EVENT_LBUTTONDOWN)
        return;

    g_selected_pixel = g_frame.at<cv::Vec3b>(y, x);
    g_is_pixel_selected = true;

    std::cout << "O valor RGB do pixel (x=" << x << ", y=" << y << ") é" << std::endl
                << "R = " << (uint) g_selected_pixel.val[2] << std::endl
                << "G = " << (uint) g_selected_pixel.val[1] << std::endl
                << "B = " << (uint) g_selected_pixel.val[0] << std::endl << std::endl;
}


int main(int argc, char** argv)
{
    cv::VideoCapture cap(0);

    if (!cap.isOpened())
    {
        std::cout << "Não foi possível abrir a câmera." << std::endl;
        return -1;
    }

    cv::namedWindow("Requisito 4", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Requisito 4", mouseCallbackFunc);

    do
    {
        cap >> g_frame;

        if (g_frame.empty())
            break;

        if (g_is_pixel_selected)
            changePixelsToRed();

        imshow("Requisito 4", g_frame);

    } while ((char) cv::waitKey(33) < 0);

    return 0;
}
