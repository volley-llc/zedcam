#include <iostream>
#include "zedCamera.h"

int main(int argc, char** argv)
{
    std::cout << "zedCamera test" << std::endl;
    zv::zedCamera zed;
    zv::ResolutionFPS resFPS = zv::ResolutionFPS::HD1080_30;
    std::cout << "opening camera" << std::endl;
    int ret = zed.open(resFPS);
    std::cout << "opening camera return: " << ret << std::endl;
    std::cout << "\ngetting camera info" << std::endl;
    zv::CameraInfo info;
    ret = zed.getCameraInfo(info);
    std::cout << "info return: " << ret << std::endl;
    std::cout << "info: "
              << "make=" << info.make << ", model=" << info.model << ", firmware=" << info.firmware
              << std::endl;
    std::cout << "\ncamera connected: " << zed.isConnected() << std::endl;
    std::cout << "\ngetting frame" << std::endl;
    cv::Mat capImage;
    ret = zed.getFrame(capImage);
    std::cout << "getFrame() return: " << ret << std::endl;
    cv::imwrite("steve2.jpg", capImage);
    std::cout << "\nmain done" << std::endl;
    exit(0);
}
