cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME Test_ocv)	

project(${PROJECT_NAME})

set(UEYE_INCLUDE_DIR "C:/Program Files/IDS/uEye/Develop/include")
set(UEYE_LIB_DIR "C:/Program Files/IDS/uEye/Develop/lib")


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${UEYE_INCLUDE_DIR})
link_directories(${UEYE_LIB_DIR})
#doaj sve cpp fajlove potrebne za implmentaciju
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp) 
target_link_libraries(${PROJECT_NAME}  ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} uEye_api_64)