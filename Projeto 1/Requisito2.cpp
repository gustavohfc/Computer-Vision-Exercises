#include <opencv2/highgui.hpp>
#include <iostream>

cv::Mat g_image;
cv::Vec3b g_selected_pixel;


/* Change the pixels from g_image to pure red if the euclidean distance between
 * the pixel and the g_selected_pixel is smaller than 13.
 */
void changePixelsToRed()
{
    g_image.forEach<cv::Vec3b>
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

    g_selected_pixel = g_image.at<cv::Vec3b>(y, x);

    std::cout << "O valor RGB do pixel (x=" << x << ", y=" << y << ") é" << std::endl
                << "R = " << (uint) g_selected_pixel.val[2] << std::endl
                << "G = " << (uint) g_selected_pixel.val[1] << std::endl
                << "B = " << (uint) g_selected_pixel.val[0] << std::endl << std::endl;

    changePixelsToRed();
    cv::imshow("Requisito 2", g_image);
}


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "O programa deve receber exatamente 1 argumento, correspondendo"
                  << " ao caminho da imagem." << std::endl;
        return -1;
    }

    g_image = cv::imread(argv[1], cv::IMREAD_COLOR);

    if (!g_image.data)
    {
        std::cout << "Não foi possível abrir a imagem." << std::endl;
        return -1;
    }

    cv::namedWindow("Requisito 2", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Requisito 2", mouseCallbackFunc);

    cv::imshow("Requisito 2", g_image);

    cv::waitKey(0);

    return 0;
}
