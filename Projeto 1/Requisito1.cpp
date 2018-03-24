#include <opencv2/highgui.hpp>
#include <iostream>

cv::Mat g_image;
cv::Vec3b g_selected_pixel;


void mouseCallbackFunc(int event, int x, int y, int, void *)
{
    if (event != cv::EVENT_LBUTTONDOWN)
        return;

    // Check if the image color is BGR or grayscale
    if (g_image.channels() == 1)
    {
        g_selected_pixel.val[0] = g_image.at<uchar>(y, x);
        g_selected_pixel.val[1] = 0;
        g_selected_pixel.val[2] = 0;
        
        std::cout << "Intensidade do pixel (x=" << x << ", y=" << y << ") = "
                  << (uint) g_selected_pixel.val[0] << std::endl << std::endl;
    }
    else if (g_image.channels() == 3)
    {
        g_selected_pixel = g_image.at<cv::Vec3b>(y, x);

        std::cout << "O valor RGB do pixel (x=" << x << ", y=" << y << ") é" << std::endl
                  << "R = " << (uint) g_selected_pixel.val[2] << std::endl
                  << "G = " << (uint) g_selected_pixel.val[1] << std::endl
                  << "B = " << (uint) g_selected_pixel.val[0] << std::endl << std::endl;
    }
}


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "O programa deve receber exatamente 1 argumento, correspondendo"
                  << " ao caminho da imagem." << std::endl;
        return -1;
    }

    g_image = cv::imread(argv[1], cv::IMREAD_ANYCOLOR);

    if (!g_image.data)
    {
        std::cout << "Não foi possível abrir a imagem." << std::endl;
        return -1;
    }

    cv::namedWindow("Requisito 1", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("Requisito 1", mouseCallbackFunc);

    cv::imshow("Requisito 1", g_image);

    cv::waitKey(0);

    return 0;
}
