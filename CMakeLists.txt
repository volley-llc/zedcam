SET(libName zedcamvolley)

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

# Set link dirs (must be done before target creation)
link_directories(${ZED_LIBRARY_DIR})
link_directories(${CUDA_LIBRARY_DIRS})

FILE(GLOB_RECURSE SRC_FILES "src/*.cpp")  

# add target
ADD_LIBRARY(${libName} SHARED ${SRC_FILES})

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
    ${CUDA_nppi_LIBRARY}
)
