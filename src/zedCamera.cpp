#include <iostream>
#include <cstdio>
#include <sl/Camera.hpp>
#include "zedCamera.h"

using namespace sl;

namespace zv
{

// construct/destruct
zedCamera::zedCamera()
{
}

zedCamera::~zedCamera()
{
}

bool zedCamera::isConnected()
{
    return m_camera.isOpened();
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
    return 0;
}

// save/load camera parameters
// (yml format)
bool zedCamera::saveParams(const std::string fileName)
{
    return true;
    ;
}

bool zedCamera::loadParams(const std::string fileName)
{
    return true;
    ;
}

// open
// open the camera for capturing at a specified
// resolution and FPS
int zedCamera::open(ResolutionFPS resFPS)
{

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

    std::cout << "zedCamera::open: opening with fps=" << fps << ", res=" << res << std::endl;

    // Set configuration parameters
    InitParameters init_params;
    init_params.camera_resolution = res;
    init_params.camera_fps = fps;

    // open the camera
    ERROR_CODE err = m_camera.open(init_params);
    if (err != ERROR_CODE::SUCCESS)
    {
        std::cerr << "zedCamera::open: ** ERROR **: failed to open camera: " << toString(err)
                  << std::endl;
        m_camera.close();
        return -1;
    }
    std::cout << "zedCamera::open: open succeeded" << std::endl;
    std::cout << "zedCamera::open: reading camera information" << std::endl;
    m_cameraInformation = m_camera.getCameraInformation();
    sl::Resolution r = m_cameraInformation.camera_configuration.resolution;
    std::cout << "zedCamera::open: serial number " << m_cameraInformation.serial_number
              << std::endl;
    std::cout << "zedCamera::open: resolution " << r.width << "x" << r.height << std::endl;

    std::cout << "zedCamera::open: allocating image buffers" << std::endl;
    // m_sl_image = sl::Mat(m_cameraInformation.camera_configuration.resolution, MAT_TYPE::U8_C4);
    // m_cv_image = slMat2cvMat(m_sl_image);
    // dumpImageProps(m_sl_image);
    std::cout << "zedCamera::open: allocating image buffers" << std::endl;
    cv::Size s = m_cv_image.size();
    std::cout << "zedCamera::open: opencv w/h: " << s.width << "/" << s.height << std::endl;
    return 0;
}

// close
void zedCamera::close()
{
    std::cout << "zedCamera::close: closing camera" << std::endl;
    m_camera.close();
}

// reset
// reset camera to default state
int zedCamera::reset()
{
    return 0;
}

void zedCamera::dumpImageProps(sl::Mat& im)
{
    std::cout << "image props: "
              << "w=" << im.getWidth() << ", h=" << im.getHeight() << ", c=" << im.getChannels()
              << ", dt=" << im.getDataType() << ", mt=" << im.getMemoryType() << std::endl;
}

// getFrame
// get the latest camera frame
int zedCamera::getFrame(cv::Mat& frame)
{
    std::cout << "zedCamera::getImage: capturing image" << std::endl;
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
        auto timestamp = m_camera.getTimestamp(sl::TIME_REFERENCE::IMAGE);
        printf("zedCamera::getFrame: %d x %d  (ts: %llu)\n", im.getWidth(), im.getHeight(),
               timestamp);
        dumpImageProps(im);
        // m_cv_image.copyTo(frame);
        im.write("steve.jpg");
        cv::Mat cvim = slMat2cvMat(im);
        frame = cvim.clone();
    }
    else
    {
        std::cerr << "zedCamera::captureImage: ** ERROR **: " << toString(err) << std::endl;
        return -1;
    }
    return 0;
}

cv::Mat zedCamera::slMat2cvMat(Mat& input)
{
    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat
    // (getPtr<T>()) cv::Mat and sl::Mat will share a single memory structure
    return cv::Mat(input.getHeight(), input.getWidth(), getOCVtype(input.getDataType()),
                   input.getPtr<sl::uchar1>(MEM::CPU), input.getStepBytes(sl::MEM::CPU));
}

// Mapping between MAT_TYPE and CV_TYPE
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
