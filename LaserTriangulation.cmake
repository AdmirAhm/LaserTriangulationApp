set(TESTLASER_NAME LaserTriangulation)				#Naziv prvog projekta u solution-u

file(GLOB TESTLASER_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB TESTLASER_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(TESTLASER_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)
file(GLOB TESTLASER_INC_TD  ${MY_INC}/td/*.h)
file(GLOB TESTLASER_INC_GUI ${MY_INC}/gui/*.h)
file(GLOB TESTLASER_INC_GUI_GL ${MY_INC}/gui/gl/*.h)

# add executable
add_executable(${TESTLASER_NAME} ${TESTLASER_INCS} ${TESTLASER_SOURCES} ${TESTLASER_INC_TD} ${TESTLASER_INC_GUI} ${TESTLASER_INC_GUI_GL})

source_group("inc"            FILES ${TESTLASER_INCS})
source_group("src"            FILES ${TESTLASER_SOURCES})
source_group("inc\\td"        FILES ${TESTLASER_INC_TD})
source_group("inc\\gui"        FILES ${TESTLASER_INC_GUI})
source_group("inc\\gui"        FILES ${TESTLASER_INC_GUI})
source_group("inc\\gl"        FILES ${TESTLASER_INC_GUI_GL})

target_link_libraries(${TESTLASER_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})

addOpenGLLibraries(${TESTLASER_NAME})

setTargetPropertiesForGUIApp(${TESTLASER_NAME} ${TESTLASER_PLIST})

setIDEPropertiesForGUIExecutable(${TESTLASER_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${TESTLASER_NAME})

