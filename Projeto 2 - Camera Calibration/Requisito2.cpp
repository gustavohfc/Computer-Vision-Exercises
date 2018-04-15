#include <iostream>
#include <chrono>

#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

#include "common.h"

// Square size in millimeter
const float square_size = 28.5;
const int n_captures = 25;
const std::chrono::milliseconds time_between_captures(1000);
const cv::Size board_size(8, 6);


bool calibrateIntrinsics(cv::VideoCapture& cap, cv::Mat& camera_matrix, cv::Mat& dist_coeffs)
{
    cv::Mat frame;
    auto last_captured_timestamp = std::chrono::system_clock::now();
    std::vector<std::vector<cv::Point2f>> image_points;

    cv::namedWindow("Calibrating intrinsics", cv::WINDOW_AUTOSIZE);

    do
    {
        cap >> frame;

        if (frame.empty())
            break;

        std::vector<cv::Point2f> chess_points;
        bool found = cv::findChessboardCorners(frame, board_size, chess_points);

        // Check the interval between captures
        auto timestamp = std::chrono::system_clock::now();
        if (found && (timestamp - last_captured_timestamp > time_between_captures))
        {
            last_captured_timestamp = timestamp;

            image_points.push_back(chess_points);

            std::cout << "Chessboard image captured ( " << image_points.size()
                      << " / " << n_captures << " )\n\n";
        }

        drawChessboardCorners(frame, board_size, chess_points, found);
        cv::imshow("Calibrating intrinsics", frame);

        if (image_points.size() == n_captures)
        {
            // Calculate the object points
            std::vector<std::vector<cv::Point3f>> object_points(1);
            for (int i = 0; i < board_size.height; i++)
                for (int j = 0; j < board_size.width; j++)
                    object_points.at(0).push_back(cv::Point3f(j * square_size, i * square_size, 0));

            object_points.resize(image_points.size(), object_points[0]);

            double rms = cv::calibrateCamera(object_points, image_points, frame.size(),
                                             camera_matrix, dist_coeffs, cv::noArray(), cv::noArray());

            std::cout << "Re-projection error: " << rms << std::endl;

            cv::destroyWindow("Calibrating intrinsics");
            return true;
        }

    } while ((char) cv::waitKey(33) < 0);

    return false;
}


void showUndistorted(cv::VideoCapture& cap, const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs)
{
    cv::Mat raw_frame, undistorted_frame;

    cv::Vec<cv::Point, 2> line_points_raw { { -1, -1 }, {-1 , -1} };
    cv::Vec<cv::Point, 2> line_points_undistorted { { -1, -1 }, {-1 , -1} };

    cv::namedWindow("raw", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("raw", mouseCallbackFunc, &line_points_raw);

    cv::namedWindow("undistorted", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("undistorted", mouseCallbackFunc, &line_points_undistorted);

    do
    {
        cap >> raw_frame;

        if (raw_frame.empty())
            break;

        cv::undistort(raw_frame, undistorted_frame, camera_matrix, dist_coeffs);

        drawInfo(raw_frame, line_points_raw[0], line_points_raw[1],
                cv::norm(line_points_raw[0] - line_points_raw[1]), "pixels");

        drawInfo(undistorted_frame, line_points_undistorted[0], line_points_undistorted[1],
                cv::norm(line_points_undistorted[0] - line_points_undistorted[1]), "pixels");
            
        cv::imshow("raw", raw_frame);
        cv::imshow("undistorted", undistorted_frame);

    } while ((char) cv::waitKey(33) < 0);
}


int main(int argc, char** argv)
{
    cv::Mat camera_matrix, dist_coeffs;

    cv::VideoCapture cap(0);

    if (!cap.isOpened())
    {
        std::cout << "Não foi possível abrir a câmera." << std::endl;
        return -1;
    }

    bool calibrated = calibrateIntrinsics(cap, camera_matrix, dist_coeffs);

    if (!calibrated)
        return -1;

    saveParameter("intrinsics.xml", "camera_matrix", camera_matrix);
    saveParameter("distortion.xml", "dist_coeffs", dist_coeffs);

    showUndistorted(cap, camera_matrix, dist_coeffs);

    return 0;
}
