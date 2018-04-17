#include <iostream>
#include <string>

#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include "common.h"

// Square side size in millimeter
const float square_size = 28.5;

// Chessboard pattern (8x6)
const cv::Size board_size(8, 6);


/* Calibrate the extrinsics parameter of the camera.
 *
 * Parameters:
 *      cap - Input camera stream.
 *      camera_matrix - Matrix of the intrisics paremeters.
 *      dist_coeffs - Matrix of the distortion coefficients.
 *      rvec - Rotation vector.
 *      tvec - Translation vector.
 */
void calibrateExtrinsics(cv::VideoCapture& cap, const cv::Mat& camera_matrix, const cv::Mat& dist_coeffs,
    cv::Mat& rvec, cv::Mat& tvec)
{
    cv::Mat frame, draw_frame;
    std::vector<cv::Point2f> chess_points;

    cv::namedWindow("Calibrating extrinsics", cv::WINDOW_AUTOSIZE);

    while(true)
    {
        cap >> frame;

        if (frame.empty())
            break;

        bool found = cv::findChessboardCorners(frame, board_size, chess_points);

        // Display the frame with the chess points
        draw_frame = frame.clone();
        drawChessboardCorners(draw_frame, board_size, chess_points, found);
        cv::imshow("Calibrating extrinsics", draw_frame);

        // Break the loop when the user press any key and the chessboard was found
        if ((char) cv::waitKey(33) > 0)
        {
            if (!found)
            {
                std::cout << "Chessboard não encontrado." << std::endl;
                continue;
            }
            
            break;
        }
    }

    // Calculate the object points
    std::vector<cv::Point3f> object_points;
    for (int i = 0; i < board_size.height; i++)
        for (int j = 0; j < board_size.width; j++)
            object_points.push_back(cv::Point3f(j * square_size, i * square_size, 0));

    // Calculate the extrinsics
    cv::solvePnP(object_points, chess_points, camera_matrix, dist_coeffs, rvec, tvec);
}


int main(int argc, char** argv)
{
    cv::Mat camera_matrix, dist_coeffs, rvec, tvec;
    char* file_name;

    if (argc != 2)
    {
        std::cout << "\n\n  Uso: ./Requisito3 arquivo_de_saída\n\n";
        return -1;
    }

    file_name = argv[1];

    cv::VideoCapture cap(0);
    if (!cap.isOpened())
    {
        std::cout << "\n\nNão foi possível abrir a câmera.\n\n";
        return -1;
    }

    readParameter("intrinsics.xml", "camera_matrix", camera_matrix);
    if (camera_matrix.empty())
        return -1;

    readParameter("distortion.xml", "dist_coeffs", dist_coeffs);
    if (dist_coeffs.empty())
        return -1;

    calibrateExtrinsics(cap, camera_matrix, dist_coeffs, rvec, tvec);

    // Save the extrinsics parameter
    cv::FileStorage fs(file_name, cv::FileStorage::WRITE);

    if (fs.isOpened())
    {
        fs << "rvec" << rvec;
        fs << "tvec" << tvec;
        fs << "tvec_norm" << cv::norm(tvec);

        std::cout << "O resultado foi salvo no arquivo " << file_name << std::endl;
        fs.release();
    }
    else
    {
        std::cout << "Não foi possível salvar o resultado no arquivo " << file_name << std::endl;
        return -1;
    }

    return 0;
}