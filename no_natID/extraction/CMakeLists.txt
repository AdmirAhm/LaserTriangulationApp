cmake_minimum_required(VERSION 3.18)

set(PROJECT_NAME Test_ocv)	

project(${PROJECT_NAME})
find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

#doaj sve cpp fajlove potrebne za implmentaciju
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp) 
target_link_libraries(${PROJECT_NAME}  ${OpenCV_LIBS})