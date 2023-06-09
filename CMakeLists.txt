cmake_minimum_required(VERSION 2.8)

message("/***************Version Info****************/")
message("Current OCR proj code Version is v0.2")
message("/******************************************/")

#1.config compiler
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER "arm-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")

project(paddle_lite_ocr)
set(TARGET paddle_lite_ocr)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -mfpu=neon -pthread")

#2.config OpenMP
set(OpenMP_CXX_FLAGS "-fopenmp")
set(OpenMP_CXX_LIB_NAMES "gomp;pthread")
set(OpenMP_C_FLAGS "-fopenmp")
set(OpenMP_C_LIB_NAMES "gomp;pthread")
set(OpenMP_gomp_LIBRARY "/usr/arm-linux-gnueabihf/lib/libgomp.so")
set(OpenMP_pthread_LIBRARY "/usr/arm-linux-gnueabihf/lib/libpthread.so")

#3.Set ARMLINUX_ARCH_ABI to armv8 or armv7 or armv7hf
set(ARMLINUX_ARCH_ABI armv7hf)

#4. set path include
include_directories(${CMAKE_SOURCE_DIR}/include)

#5. path to Paddle-Lite lib
set(LITE_DIR "${CMAKE_SOURCE_DIR}/ocr_lib")

#6. link Paddle-Lite directory
link_directories(${LITE_DIR}/lib)

#7. add OpenMP package
FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
  message("OPENMP FOUND")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

#8.add opencv lib
if(${OCV_LIB_VERSION} STREQUAL "4")
set(OpenCV_DIR ${LITE_DIR}/opencv-4-1-0-linux-armv7hf/lib/cmake/opencv4)
else()
set(OpenCV_DIR ${LITE_DIR}/opencv-3-4-1-linux-armv7hf/share/OpenCV)
endif()
find_package(OpenCV REQUIRED)

#9.set camera files 
file(GLOB_RECURSE CAMERA_SRC_LIST "${CMAKE_SOURCE_DIR}/camera/*.cpp" "${CMAKE_SOURCE_DIR}/camera/*.c") 
message(STATUS "CAMERA_SRC_LIST = ${CAMERA_SRC_LIST}")     

#10.set ocr files
file(GLOB_RECURSE OCR_SRC_LIST "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.cc" "${CMAKE_SOURCE_DIR}/src/*.c") 
message(STATUS "OCR_SRC_LIST = ${OCR_SRC_LIST}")     

set(CMAKE_CXX_STANDARD 14)

#11.add executable output
add_executable(${TARGET} ${OCR_SRC_LIST} ${CAMERA_SRC_LIST})
target_link_libraries(${TARGET} -lpaddle_light_api_shared ${OpenCV_LIBS})
target_link_libraries(${TARGET} -ldl)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/paddle_lite_ocr)
install(TARGETS paddle_lite_ocr DESTINATION ./)
#install(DIRECTORY model DESTINATION ./)