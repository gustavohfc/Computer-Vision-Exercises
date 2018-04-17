#include <iostream>
#include <string>

#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>

#include "common.h"


/* Converts a pointer coordinate in the image plane to a pointer in the chessboard plane.
 *
 * Parameters:
 *      image_point - Coordinate in the image plain.
 *      homography - Homography matrix generated from the intrisics and extrinsics parameters, H = K [r1, r2, t].
 *      plane_point[out] - Coordinate in the chessboard plane.
 */
void imageToChessboardPlane(const cv::Point image_point, const cv::Mat homography, cv::Point2f& plane_point)
{
    // Convert euclidean to homogeneus coordinates
    cv::Mat image_point_homogeneous(1, 3, CV_64F);
    image_point_homogeneous.at<double>(0, 0) = image_point.x;
    image_point_homogeneous.at<double>(0, 1) = image_point.y;
    image_point_homogeneous.at<double>(0, 2) = 1;

    // X = H^-1 * x
    cv::Mat plane_point_homogeneous = homography.inv() * image_point_homogeneous.t();

    // Convert homogeneus to euclidean coordinates
    plane_point.x = plane_point_homogeneous.at<double>(0, 0) / plane_point_homogeneous.at<double>(0, 2);
    plane_point.y = plane_point_homogeneous.at<double>(0, 1) / plane_point_homogeneous.at<double>(0, 2);
}


/* Shows a image and allows the user to make measurements of objects in the image or video.
 *
 * Parameters:
 *      window_name - Name of the window to display the image.
 *      frame - Image or next frame of the video to be displayed.
 *      homography - Homography matrix generated from the intrisics and extrinsics parameters, H = K [r1, r2, t].
 *      line_points_image - Points to calculate the distance.
 */
void virtualRuler(const char* window_name, cv::Mat frame, const cv::Mat homography,
    const cv::Vec<cv::Point, 2> line_points_image)
{
    cv::Vec<cv::Point2f, 2> line_points_plane;

    // Convert the two points to the chessboard plane.
    imageToChessboardPlane(line_points_image[0], homography, line_points_plane[0]);
    imageToChessboardPlane(line_points_image[1], homography, line_points_plane[1]);

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
        std::cout << "\n\n  Uso: ./Requisito3 arquivo_de_extrinsecos\n\n";
        return -1;
    }
    extrinsics_file_name = argv[1];

    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cout << "\n\nNão foi possível abrir a câmera.\n\n";
        return -1;
    }

    // Read the necessery parameters
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

    // Calculate the Homography matrix, H = K [R1, R2, t]
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