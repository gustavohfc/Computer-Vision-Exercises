#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "common.h"

/* Callback to mouse event.
 */
void mouseCallbackFunc(int event, int x, int y, int flags, void *data)
{
    static bool selecting_first_point = true;
    cv::Vec<cv::Point, 2>& line_points = *((cv::Vec<cv::Point, 2> *) data);

    if (event == cv::EVENT_LBUTTONDOWN)
    {
        if (selecting_first_point)
        {
            line_points[0] = cv::Point(x, y);
            line_points[1] = cv::Point(x, y);
            selecting_first_point = false;
        }
        else
        {
            line_points[1] = cv::Point(x, y);
            selecting_first_point = true;
        }
    }
    else if (event == cv::EVENT_MOUSEMOVE && !selecting_first_point)
    {
        // Make the line follow the mouse when the first point is already selected
        // but the second point isn't.
        line_points[1] = cv::Point(x, y);
    }
}


/* Draws a line and the distance information on the image.
 *
 * Parameters:
 *      image - Matrix of the image pixels.
 *      p1, p2 - Points to be connected with the line.
 *      distance - Value of the distance to be displayed.
 *      distance_unit - Measuring unit of the distance (Ex.: "mm", "pixels").
 */
void drawInfo(const cv::Mat& image, cv::Point p1, cv::Point p2, int distance, const char *distance_unit)
{
    if (p2.x == -1)
        return;

    // Draw the line
    cv::line(image, p1, p2, cv::Scalar(0, 0, 255), 5);

    // Draw the info text
    cv::Point text_orig(p2);
    cv::String text = cv::format(" D = %d %s", distance, distance_unit);
    int font_face = cv::FONT_HERSHEY_SIMPLEX;
    double font_scale = 1;
    int thickness = 2;

    cv::Size textSize = getTextSize(text, font_face, font_scale, thickness, nullptr);

    // Make sure that the text is inside the image
    if (text_orig.x + textSize.width > image.cols)
        text_orig.x = image.cols - textSize.width;
    if (text_orig.y - textSize.height < 0)
        text_orig.y = textSize.height;

    cv::putText(image, text, text_orig, font_face, font_scale, cv::Scalar(0, 255, 0), thickness);
}


/* Saves a cv::Mat in a xml file.
 *
 * Parameters:
 *      file_name - Name of the xml file.
 *      tag_name - Name of the xml tag.
 *      parameter - Value to be saved.
 */
void saveParameter(const char* file_name, const char* tag_name, const cv::Mat& parameter)
{
    cv::FileStorage fs(file_name, cv::FileStorage::WRITE);

    if (fs.isOpened())
    {
        fs << tag_name << parameter;
        std::cout << tag_name << " salvo em " << file_name << std::endl;
        fs.release();
    }
    else
    {
        std::cout << "Não foi possível salvar no arquivo " << file_name << std::endl;
    }
}


/* Reads a cv::Mat from a xml file.
 *
 * Parameters:
 *      file_name - Name of the xml file.
 *      tag_name - Name of the xml tag.
 *      parameter[out] - Value of the tag read from the file.
 */
void readParameter(const char* file_name, const char* tag_name, cv::Mat& parameter)
{
    cv::FileStorage fs(file_name, cv::FileStorage::READ);

    if (fs.isOpened())
    {
        fs[tag_name] >> parameter;
        fs.release();
    }
    else
    {
        std::cout << "Não foi possível abrir o arquivo " << file_name << std::endl;
    }
}
