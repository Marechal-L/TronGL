cmake_minimum_required(VERSION 2.6)

project(CGILabs)
enable_testing()

###  Settings  ###{{{
set(CMAKE_CXX_FLAGS "-std=c++11 -DGL_GLEXT_PROTOTYPES -I/opt/X11/include -g") # Don't forget to define GL_GLEXT_PROTOTYPES to ensure that the glCreateShader and other prototypes are declared. Note we could do it in the main file via "#define GL_GLEXT_PROTOTYPES" before "#include <GL/glext.h>"
## setting build type to debug
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_BUILD_TYPE DEBUG)
LINK_DIRECTORIES("~/local_install/lib")
LINK_DIRECTORIES("/usr/local/lib")
LINK_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/lib")
INCLUDE_DIRECTORIES("include")
INCLUDE_DIRECTORIES("/usr/local/include")
INCLUDE_DIRECTORIES("~/local_install/include")
INCLUDE_DIRECTORIES("./3rd-parties/")
INCLUDE_DIRECTORIES("include/irrKlang")
###  Settings  ###}}}

###  OpenGL  ###{{{
find_package(OpenGL)
if (OPENGL_FOUND)
   include_directories(${OPENGL_INCLUDE_DIR})
else (OPENGL_FOUND)
    message("OpenGL environment missing")
endif (OPENGL_FOUND)
set(GL_LIBRARIES ${OPENGL_LIBRARIES} GLEW)
if(APPLE)
 set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}" )
 SET(APPLE_FRAMEWORKS "-framework Cocoa -framework IOkit -framework CoreVideo")
endif(APPLE)
###  OpenGL  ###}}}

###  GLFW3  ###{{{
set( ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:$ENV{HOME}/local_install/lib/pkgconfig")
FIND_PACKAGE( PkgConfig REQUIRED )
PKG_SEARCH_MODULE( GLFW3 REQUIRED glfw3 ) # sets GLFW3 as prefix for glfw vars
# now ${GLFW3_INCLUDE_DIR}, ${GLFW3_LIBRARIES} and ${GLFW3_STATIC_LIBRARIES}
# are set
INCLUDE_DIRECTORIES( ${GLFW3_INCLUDE_DIR} )
###  GLFW3  ###}}}

###  ASSIMP  ###{{{
PKG_SEARCH_MODULE( ASSIMP REQUIRED assimp )
include_directories(${ASSIMP_INCLUDE_DIRS})
link_directories(${ASSIMP_LIBRARY_DIRS})
###  ASSIMP  ###}}}



add_executable(TronGL src/TronGL.cpp  src/Application.cpp src/Camera.cpp src/Input.cpp src/Shader.cpp src/Object.cpp src/Texture.cpp src/Player.cpp src/Grid.cpp src/vboindexer.cpp src/IA.cpp src/Human.cpp src/ParticleGenerator.cpp)
IF(APPLE)
  target_link_libraries(TronGL  ${GL_LIBRARIES} ${APPLE_FRAMEWORKS} ${GLFW3_STATIC_LIBRARIES} ${ASSIMP_LIBRARIES} "${CMAKE_CURRENT_SOURCE_DIR}/lib/libIrrKlang.dylib")
ELSE(APPLE)
  target_link_libraries(TronGL  ${GL_LIBRARIES} ${APPLE_FRAMEWORKS} ${GLFW3_STATIC_LIBRARIES} ${ASSIMP_LIBRARIES} "${CMAKE_CURRENT_SOURCE_DIR}/lib/libIrrKlang.so")
ENDIF(APPLE)

add_dependencies(TronGL resources)
