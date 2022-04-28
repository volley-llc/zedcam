#pragma once
#include <sl/Camera.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/cvconfig.h>

namespace zv
{

// convenience enum to specify
// resolution + framerate together
enum class ResolutionFPS
{
    HD1080_15,
    HD1080_30,
    HD720_15,
    HD720_30,
    HD720_60,
};

// camera information struct
// compatible with volley status
struct CameraInfo
{
    std::string make;
    std::string model;
    std::string firmware;
};

// class encapsulating the zed camera
class zedCamera
{
  public:
    // is camera connected?
    bool isConnected();

    // get camera info
    int getCameraInfo(CameraInfo& info);

    // save/load camera parameters (yml format)
    bool saveParams(const std::string fileName);
    bool loadParams(const std::string fileName);

    // open/close
    // open the camera for capturing at a specified resolution and FPS
    int open(ResolutionFPS);
    void close();

    // reset
    // reset camera to default state
    int reset();

    // getFrame
    // get the latest camera frame
    int getFrame(cv::Mat& frame);

  private:
    sl::Camera m_camera;
    sl::CameraInformation m_cameraInformation;

    cv::Mat slMat2cvMat(sl::Mat& input);
    int getOCVtype(sl::MAT_TYPE type);
    void dumpImageProps(sl::Mat& im);
};

} // namespace zv
