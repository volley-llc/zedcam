SET(libName zedcamvolley)

option(WITH_TEST "Configure with test exe" OFF)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
#PROJECT(${execName})

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0015 OLD)
endif(COMMAND cmake_policy)

SET(EXECUTABLE_OUTPUT_PATH ".")
SET(VERSION_REQ_CUDA "8.0")

# Find dependent packages
message(STATUS "finding required packages ZED2 and CUDA") 
message(STATUS "(cuda version: ${VERSION_REQ_CUDA})")
find_package(ZED 2 REQUIRED)
find_package(CUDA ${VERSION_REQ_CUDA} REQUIRED)
SET(SPECIAL_OS_LIBS "pthread")

find_package(OpenCV 4.5.0 REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

message(STATUS "cuda nppi: ${CUDA_nppi_LIBRARY}")
message(STATUS "cuda npps: ${CUDA_npps_LIBRARY}")

# Set link dirs (must be done before target creation)
link_directories(${ZED_LIBRARY_DIR})
link_directories(${CUDA_LIBRARY_DIRS})

FILE(GLOB_RECURSE LIB_SRC_FILES "src/zed*.cpp")  
FILE(GLOB_RECURSE EXE_SRC_FILES "src/*.cpp")  

# add target
ADD_LIBRARY(${libName} SHARED ${LIB_SRC_FILES})

if (WITH_TEST) 
  message(STATUS "including test exe")
  ADD_EXECUTABLE("zedtest" ${EXE_SRC_FILES})
  target_include_directories("zedtest"
	  PUBLIC
	  ${CUDA_INCLUDE_DIRS}
	  ${ZED_INCLUDE_DIRS}
	  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  )
  target_link_libraries("zedtest"
    ${ZED_LIBRARIES}
    ${SPECIAL_OS_LIBS}
    ${CUDA_LIBRARIES} 
    ${CUDA_npps_LIBRARY} 
    ${OpenCV_LIBS}
    #${CUDA_nppi_LIBRARY}
  )
endif()

add_definitions(-std=c++0x -g -O3 -Wno-deprecated-declarations)

message(STATUS "cuda include: ${CUDA_INCLUDE_DIRS}")
message(STATUS "zed include: ${ZED_INCLUDE_DIRS}")
target_include_directories(${libName}
	PUBLIC
	${CUDA_INCLUDE_DIRS}
	${ZED_INCLUDE_DIRS}
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Add the required libraries for linking:
target_link_libraries(${libName}
    ${ZED_LIBRARIES}
    ${SPECIAL_OS_LIBS}
    ${CUDA_LIBRARIES} 
    ${CUDA_npps_LIBRARY} 
    ${OpenCV_LIBS}
    #${CUDA_nppi_LIBRARY}
)
