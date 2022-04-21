#pragma once
#include <sl/Camera.hpp>

namespace zv
{

  enum class ResolutionFPS {
    HD1080_15,
    HD1080_30,
    HD720_15,
    HD720_30,
    HD720_60,
  };


  class zedCamera
  {
    public:
      // construct/destruct 
      zedCamera();
      virtual ~zedCamera();

      // save/load camera parameters
      // (yml format)
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
      int getFrame();  

      // getControls
      // retrieve the current camera control settings 

      // setControls
      // set the current camera control settings  
  
    private:
      sl::Camera m_camera;
      sl::CameraInformation m_cameraInformation; 
  };

}
