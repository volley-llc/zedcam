#include <iostream>
#include "zedCamera.h"

int main(int argc, char **argv) {
   std::cout << "zedCamera test" << std::endl;
   zv::zedCamera zed;
   zv::ResolutionFPS resFPS = zv::ResolutionFPS::HD1080_30;
   zed.open(resFPS);
   std::cout << "main done" << std::endl;
   exit(0); 
}

