SET(execName zedtest)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(${execName})

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

# Set include dirs
include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${ZED_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set link dirs
link_directories(${ZED_LIBRARY_DIR})
link_directories(${CUDA_LIBRARY_DIRS})

SET(SRC_FOLDER src)
FILE(GLOB_RECURSE SRC_FILES "${SRC_FOLDER}/*.cpp")  
ADD_EXECUTABLE(${execName} ${SRC_FILES})

add_definitions(-std=c++0x -g -O3)

# Add the required libraries for linking:
TARGET_LINK_LIBRARIES(${execName}
    ${ZED_LIBRARIES}
    ${SPECIAL_OS_LIBS}
    ${CUDA_LIBRARIES} 
    ${CUDA_npps_LIBRARY} 
    ${CUDA_nppi_LIBRARY}
)