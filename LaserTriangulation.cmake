set(PROJEKAT_NAME LaserTriangulation)				

file(GLOB PROJEKAT_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB PROJEKAT_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB PROJEKAT_SHADERS  ${CMAKE_CURRENT_LIST_DIR}/res/shaders/*)
set(PROJEKAT_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)

# add executable
add_executable(${PROJEKAT_NAME} ${PROJEKAT_INCS} ${PROJEKAT_SOURCES}  ${PROJEKAT_SHADERS})

source_group("inc"            	FILES ${PROJEKAT_INCS})
source_group("src"            	FILES ${PROJEKAT_SOURCES})
source_group("shaders"        	FILES ${PROJEKAT_SHADERS})

# za OpenCV
find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})


target_link_libraries(${PROJEKAT_NAME} 	
							debug ${MU_LIB_DEBUG}			optimized ${MU_LIB_RELEASE}  
							debug ${NATGUI_LIB_DEBUG}		optimized ${NATGUI_LIB_RELEASE} 
							debug ${DP_LIB_DEBUG} 			optimized ${DP_LIB_RELEASE} 
							debug ${NATGL_LIB_DEBUG} 		optimized ${NATGL_LIB_RELEASE}
							${OpenCV_LIBS}
							)

addOpenGLLibraries(${PROJEKAT_NAME})

setTargetPropertiesForGUIApp(${PROJEKAT_NAME} ${PROJEKAT_PLIST})

setIDEPropertiesForGUIExecutable(${PROJEKAT_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${PROJEKAT_NAME})



