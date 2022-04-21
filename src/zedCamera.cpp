#include <iostream>
#include <sl/Camera.hpp>
#include "zedCamera.h"

using namespace sl;

namespace zv {

  // construct/destruct 
  zedCamera::zedCamera() {
  }

  zedCamera::~zedCamera() {
  }

  // save/load camera parameters
  // (yml format)
  bool zedCamera::saveParams(const std::string fileName) {
    return true;;
  }

  bool zedCamera::loadParams(const std::string fileName) {
    return true;;
  }

  // open
  // open the camera for capturing at a specified
  // resolution and FPS
  int zedCamera::open(ResolutionFPS resFPS) {
   
    sl::RESOLUTION res;
    int fps;
 
    switch (resFPS) {
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
    if (err != ERROR_CODE::SUCCESS) {
      std::cerr << "zedCamera::open: ** ERROR **: failed to open camera: " << toString(err) << std::endl;
      m_camera.close();
      return 1; // Quit if an error occurred
    }

    return 0;
  }



  // reset 
  // reset camera to default state
  int zedCamera::reset() {
    return 0;
  }

  // getFrame
  // get the latest camera frame
  int zedCamera::getFrame() {
    return 0;
  }

} 
