set(GLT4_NAME GL_Tut4)				#Naziv prvog projekta u solution-u

file(GLOB GLT4_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB GLT4_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(GLT4_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)

# add executable
add_executable(${GLT4_NAME} ${GLT4_INCS} ${GLT4_SOURCES})

source_group("inc"            FILES ${GLT4_INCS})
source_group("src"            FILES ${GLT4_SOURCES})

target_link_libraries(${GLT4_NAME} debug ${MU_LIB_DEBUG} optimized ${MU_LIB_RELEASE} 
									debug ${NATGUI_LIB_DEBUG} optimized ${NATGUI_LIB_RELEASE} 
								debug ${NATGL_LIB_DEBUG} optimized ${NATGL_LIB_RELEASE})


addOpenGLLibraries(${GLT4_NAME})

setTargetPropertiesForGUIApp(${GLT4_NAME} ${GLT4_PLIST})

setIDEPropertiesForGUIExecutable(${GLT4_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${GLT4_NAME})



