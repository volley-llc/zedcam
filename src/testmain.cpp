#include <iostream>
#include "zedCamera.h"

int main(int argc, char** argv)
{
    std::cout << "zedCamera test" << std::endl;
    //
    zv::zedCamera zed;
    // Test opening camera with ResolutionFPS enum settings
    zv::ResolutionFPS resFPS = zv::ResolutionFPS::HD1080_30;
    std::cout << "opening camera" << std::endl;
    int ret = zed.open(resFPS);
    std::cout << "opening camera return: " << ret << std::endl;
    //
    std::cout << "getting camera info" << std::endl;
    zv::CameraInfo info;
    ret = zed.getCameraInfo(info);
    std::cout << "info return: " << ret << std::endl;
    std::cout << "info: "
              << "make=" << info.make << ", model=" << info.model << ", firmware=" << info.firmware
              << ", frame_width=" << info.frame_width << ", frame_height=" << info.frame_height
              << std::endl;
    std::cout << "camera connected: " << zed.isConnected() << std::endl;
    //
    std::cout << "getting camera instrinsics" << std::endl;
    cv::Mat im = zed.getIntrinsicMatrix();
    cv::Mat dm = zed.getDistortionCoeffs();
    std::cout << "iMat = " << std::endl << " " << im << std::endl << std::endl;
    std::cout << "dMat = " << std::endl << " " << dm << std::endl << std::endl;
    //
    std::cout << "getting frame (cvMat)" << std::endl;
    cv::Mat capImage;
    ret = zed.getFrame(capImage);
    std::cout << "get frame return: " << ret << std::endl;
    std::cout << "writing jpeg image 'testImage.jpg'" << std::endl;
    cv::imwrite("testImage.jpg", capImage);

    // Test opening camera with yml settings file
    std::cout << "\n===========================================================" << std::endl;
    std::cout << "Test camera opening with yml settings file" << std::endl;
    ret = zed.open("../settings/zed2i_settings.yml");
    std::cout << "opening camera with zed2_settings.yml, return: " << ret << std::endl;
    std::cout << "camera connected: " << zed.isConnected() << std::endl;
    //
    std::cout << "getting camera instrinsics" << std::endl;
    im = zed.getIntrinsicMatrix();
    dm = zed.getDistortionCoeffs();
    std::cout << "iMat = " << std::endl << " " << im << std::endl << std::endl;
    std::cout << "dMat = " << std::endl << " " << dm << std::endl << std::endl;
    //
    std::cout << "End of test, closing camera." << std::endl;
    zed.close();
    std::cout << "exiting" << std::endl;
    //
    exit(0);
}
