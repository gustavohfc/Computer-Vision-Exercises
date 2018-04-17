#include <iostream>
#include <string>

#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

#include "common.h"

void f(const cv::Point image_point, const cv::Mat homography, cv::Point2f& plane_point)
{
    cv::Mat image_point_homogeneous(1, 3, CV_64F);
    image_point_homogeneous.at<double>(0, 0) = image_point.x;
    image_point_homogeneous.at<double>(0, 1) = image_point.y;
    image_point_homogeneous.at<double>(0, 2) = 1;

    cv::Mat plane_point_homogeneous = homography.inv() * image_point_homogeneous.t();

    plane_point.x = plane_point_homogeneous.at<double>(0, 0) / plane_point_homogeneous.at<double>(0, 2);
    plane_point.y = plane_point_homogeneous.at<double>(0, 1) / plane_point_homogeneous.at<double>(0, 2);
}


void virtualRuler(const char* window_name, cv::Mat frame, const cv::Mat homography,
    const cv::Vec<cv::Point, 2> line_points_image)
{
    cv::Vec<cv::Point2f, 2> line_points_plane;

    f(line_points_image[0], homography, line_points_plane[0]);
    f(line_points_image[1], homography, line_points_plane[1]);

    drawInfo(frame, line_points_image[0], line_points_image[1],
        cv::norm(line_points_plane[0] - line_points_plane[1]), "mm");

    cv::imshow(window_name, frame);
}


int main(int argc, char** argv)
{
    cv::Mat raw_frame, undistorted_frame, camera_matrix, dist_coeffs, rvec, tvec;
    cv::Mat R, RT, homography;
    cv::Vec<cv::Point, 2> line_points_raw { { -1, -1 }, {-1 , -1} };
    cv::Vec<cv::Point, 2> line_points_undistorted { { -1, -1 }, {-1 , -1} };
    char* extrinsics_file_name;

    if (argc != 2)
    {
        std::cout << "Uso: ./Requisito3 arquivo_de_extrinsecos" << std::endl;
        return -1;
    }
    extrinsics_file_name = argv[1];

    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cout << "Não foi possível abrir a câmera." << std::endl;
        return -1;
    }

    readParameter("intrinsics.xml", "camera_matrix", camera_matrix);
    if (camera_matrix.empty())
        return -1;

    readParameter("distortion.xml", "dist_coeffs", dist_coeffs);
    if (dist_coeffs.empty())
        return -1;

    readParameter(extrinsics_file_name, "rvec", rvec);
    readParameter(extrinsics_file_name, "tvec", tvec);
    if (rvec.empty() || tvec.empty())
        return -1;

    cv::namedWindow("raw", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("raw", mouseCallbackFunc, &line_points_raw);

    cv::namedWindow("undistorted", cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback("undistorted", mouseCallbackFunc, &line_points_undistorted);

    // H = K [R1, R2, t]
    cv::Rodrigues(rvec, R);
    cv::hconcat(R.colRange(0, 2), tvec, RT);
    homography = camera_matrix * RT;

    do
    {
        cap >> raw_frame;

        if (raw_frame.empty())
            break;

        cv::undistort(raw_frame, undistorted_frame, camera_matrix, dist_coeffs);

        virtualRuler("raw", raw_frame, homography, line_points_raw);
        virtualRuler("undistorted", undistorted_frame, homography, line_points_undistorted);

    } while ((char) cv::waitKey(33) < 0);

    return 0;
}