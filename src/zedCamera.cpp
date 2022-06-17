#include <iostream>
#include <cstdio>
#include <sl/Camera.hpp>
#include "zedCamera.h"

using namespace sl;

namespace zv
{

// is camera connected?
bool zedCamera::isConnected()
{
    return m_camera.isOpened();
}

// get and set camera intrinsic matrix
cv::Mat zedCamera::getIntrinsicMatrix()
{
    return m_intrinsic_matrix.clone();
}

void zedCamera::setIntrinsicMatrix(const cv::Mat& iMat)
{
    iMat.copyTo(m_intrinsic_matrix);
}

// get and set camera distortion coeffs
cv::Mat zedCamera::getDistortionCoeffs()
{
    return m_distortion_coeffs.clone();
}

void zedCamera::setDistortionCoeffs(const cv::Mat& dMat)
{
    dMat.copyTo(m_distortion_coeffs);
}

// get camera info
int zedCamera::getCameraInfo(CameraInfo& info)
{
    if (!m_camera.isOpened())
    {
        std::cerr << "zedCamera::getCameraInfo: ** ERROR **: camera not open" << std::endl;
        return -1;
    }
    info.make = "stereolabs";
    switch (m_cameraInformation.camera_model)
    {
    case sl::MODEL::ZED:
        info.model = "Zed";
        break;
    case sl::MODEL::ZED_M:
        info.model = "Zed_M";
        break;
    case sl::MODEL::ZED2:
        info.model = "Zed2";
        break;
    case sl::MODEL::ZED2i:
        info.model = "Zed2i";
        break;
    default:
        std::cerr << "zedCamera::getCameraInfo: ** ERROR **: unknown camera model "
                  << m_cameraInformation.camera_model << std::endl;
        return -1;
        break;
    }
    unsigned int fw = m_cameraInformation.camera_configuration.firmware_version;
    info.firmware = std::to_string(fw);
    info.frame_width = m_cameraInformation.camera_resolution.width;
    info.frame_height = m_cameraInformation.camera_resolution.height;
    return 0;
}

// save/load camera parameters (yml format)
bool zedCamera::saveParams(const std::string fileName)
{
    std::cerr << "zedCamera::saveParams: ** ERROR **: NOT IMPLEMENTED YET" << std::endl;
    return false;
}

bool zedCamera::loadParams(const std::string fileName)
{
    std::cerr << "zedCamera::loadParams: ** ERROR **: NOT IMPLEMENTED YET" << std::endl;
    return false;
}

// open
// open the camera for capturing at a specified resolution and FPS
// Return non-zero on error.
int zedCamera::open(ResolutionFPS resFPS)
{

    if (m_camera.isOpened())
    {
        std::cerr << "zedCamera::open: ** WARN **: camera is already open. Closing camera and "
                     "re-opening with new ResolutionFPS settings."
                  << std::endl;
        m_camera.close();
    }

    sl::RESOLUTION res;
    int fps;

    switch (resFPS)
    {
    case ResolutionFPS::HD1080_15:
        res = RESOLUTION::HD1080;
        fps = 15;
        break;
    case ResolutionFPS::HD1080_30:
        res = RESOLUTION::HD1080;
        fps = 30;
        break;
    case ResolutionFPS::HD720_15:
        res = RESOLUTION::HD720;
        fps = 15;
        break;
    case ResolutionFPS::HD720_30:
        res = RESOLUTION::HD720;
        fps = 30;
        break;
    case ResolutionFPS::HD720_60:
        res = RESOLUTION::HD720;
        fps = 60;
        break;
    }
    // std::cout << "zedCamera::open: opening with fps=" << fps << ", res=" << res << std::endl;

    // Set configuration parameters
    InitParameters init_params;
    init_params.camera_resolution = res;
    init_params.camera_fps = fps;

    return _open(init_params);
}

// open
// open the camera for capturing and load InitParameters from given file.
// Return non-zero on error.
int zedCamera::open(const std::string& init_params_file_name)
{
    if (init_params_file_name.empty())
    {
        std::cerr << "zedCamera::open: ** ERROR **: Empty InitParameters filename." << std::endl;
        return -1;
    }

    if (m_camera.isOpened())
    {
        std::cerr << "zedCamera::open: ** WARN **: camera is already open. Closing camera and "
                     "re-opening with new InitParameters from the settings file: "
                  << init_params_file_name << std::endl;
        m_camera.close();
    }

    // Set configuration parameters
    InitParameters init_params;
    if (!init_params.load(sl::String(init_params_file_name.c_str())))
    {
        std::cerr << "zedCamera::open: ** ERROR **: failed loading init parameters from given file:"
                  << init_params_file_name << std::endl;
        m_camera.close();
        return -1;
    }

    return _open(init_params);
}

// open
// open the camera for capturing with specified InitParameters and update
// intrinsic matrix and distortion coefficient vector
// Return non-zero on error.
int zedCamera::_open(InitParameters& init_params)
{

    // open the camera
    ERROR_CODE err = m_camera.open(init_params);
    if (err != ERROR_CODE::SUCCESS)
    {
        std::cerr << "zedCamera::open: ** ERROR **: failed to open camera: " << toString(err)
                  << std::endl;
        m_camera.close();
        return -1;
    }

    // grab the camera information
    m_cameraInformation = m_camera.getCameraInformation();

    // Set the Roi for automatic gain and exposure time adjustment
    int roi_x = 0;
    int roi_y = m_cameraInformation.camera_resolution.height * Y_START;
    int roi_w = m_cameraInformation.camera_resolution.width;
    int roi_h = m_cameraInformation.camera_resolution.height * (1 - Y_START);
    Rect rect = Rect(roi_x, roi_y, roi_w, roi_h);
    err = m_camera.setCameraSettings(VIDEO_SETTINGS::AEC_AGC_ROI, rect);
    if (err != ERROR_CODE::SUCCESS)
    {
        std::cerr << "zedCamera::open: ** ERROR **: failed to set AEC_AGC_ROI for the camera: "
                  << toString(err) << std::endl;
    }
    // else
    //{
    //    std::cout << "zedCamera::open: ** INFO **: successfully set AEC_AGC_ROI for the camera: "
    //              << std::endl;
    //}

    // populate intrinsic matrix
    // by reading from the camera info
    m_intrinsic_matrix = cv::Mat::zeros(3, 3, CV_64F);
    m_intrinsic_matrix(0, 0) =
        m_cameraInformation.camera_configuration.calibration_parameters.left_cam.fx;
    m_intrinsic_matrix(0, 2) =
        m_cameraInformation.camera_configuration.calibration_parameters.left_cam.cx;
    m_intrinsic_matrix(1, 1) =
        m_cameraInformation.camera_configuration.calibration_parameters.left_cam.fy;
    m_intrinsic_matrix(1, 2) =
        m_cameraInformation.camera_configuration.calibration_parameters.left_cam.cy;
    m_intrinsic_matrix(2, 2) = 1.0;

    // populate dist coeffs
    // [ k1, k2, p1, p2, k3, k4, k5, k6, s1, s2, s3, s4]
    m_distortion_coeffs = cv::Mat::zeros(12, 1, CV_64F);
    for (int i = 0; i < 12; i++)
    {
        m_distortion_coeffs.at<double>(i, 0) =
            m_cameraInformation.camera_configuration.calibration_parameters.left_cam.disto[i];
    }

    return 0;
}

// close
void zedCamera::close()
{
    // std::cout << "zedCamera::close: closing camera" << std::endl;
    m_camera.close();
}

// reset
// reset camera to default state
int zedCamera::reset()
{
    return 0;
}

// getFrame
// Capture the latest camera frame into an OpenCV matrix.
// Return non-zero on error.
int zedCamera::getFrame(cv::Mat& frame)
{
    ERROR_CODE err = m_camera.grab();
    if (err == ERROR_CODE::SUCCESS)
    {
        sl::Mat im;
        ERROR_CODE err2 = m_camera.retrieveImage(im, VIEW::LEFT);
        if (err2 != ERROR_CODE::SUCCESS)
        {
            std::cerr << "zedCamera::getFrame: ** ERROR **: " << toString(err2) << std::endl;
            return -1;
        }
        cv::Mat cvim = slMat2cvMat(im);
        frame = cvim.clone();
    }
    else
    {
        std::cerr << "zedCamera::getFrame: ** ERROR **: " << toString(err) << std::endl;
        return -1;
    }
    return 0;
}

//_______________________________________________
// Helpers

// dump stereolabs image properties to stdout
void zedCamera::dumpImageProps(sl::Mat& im)
{
    std::cout << "image props: "
              << "w=" << im.getWidth() << ", h=" << im.getHeight() << ", c=" << im.getChannels()
              << ", dt=" << im.getDataType() << ", mt=" << im.getMemoryType() << std::endl;
}

// "convert" a stereolabs Mat into a cvMat
// NOTE: the returned cvMat is shallow and points to the
// same data as the as the input Mat.
cv::Mat zedCamera::slMat2cvMat(Mat& input)
{
    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat
    // (getPtr<T>()) cv::Mat and sl::Mat will share a single memory structure
    return cv::Mat(input.getHeight(), input.getWidth(), getOCVtype(input.getDataType()),
                   input.getPtr<sl::uchar1>(MEM::CPU), input.getStepBytes(sl::MEM::CPU));
}

// mapping between MAT_TYPE and CV_TYPE
int zedCamera::getOCVtype(sl::MAT_TYPE type)
{
    int cv_type = -1;
    switch (type)
    {
    case MAT_TYPE::F32_C1:
        cv_type = CV_32FC1;
        break;
    case MAT_TYPE::F32_C2:
        cv_type = CV_32FC2;
        break;
    case MAT_TYPE::F32_C3:
        cv_type = CV_32FC3;
        break;
    case MAT_TYPE::F32_C4:
        cv_type = CV_32FC4;
        break;
    case MAT_TYPE::U8_C1:
        cv_type = CV_8UC1;
        break;
    case MAT_TYPE::U8_C2:
        cv_type = CV_8UC2;
        break;
    case MAT_TYPE::U8_C3:
        cv_type = CV_8UC3;
        break;
    case MAT_TYPE::U8_C4:
        cv_type = CV_8UC4;
        break;
    default:
        break;
    }
    return cv_type;
}

} // namespace zv
