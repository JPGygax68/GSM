include(DetermineTargetArchitecture)
determine_target_architecture(CMAKE_LIBRARY_ARCHITECTURE)

add_library(GSM

  gsm.hpp             src/gsm.cpp

  dll.h

  types.hpp
  gfxtypes.hpp

  isessionmgr.hpp
  ibitmap.hpp
  icomp.hpp
  #ifontrast.hpp
  ifontprov.hpp
  #ifont.hpp                     src/ifont.cpp
  idisplay.hpp
  isurface.hpp
  icanvas.hpp

  ievent.hpp
  iptrbtnevt.hpp
  iptrmovevt.hpp
  imousewhlevt.hpp
  ikeybevt.hpp
  igfxres.hpp

  compmgr.hpp                   src/compmgr.cpp
  charlist.hpp                  src/charlist.cpp

  opengl/opengl.hpp             src/opengl/opengl.cpp

  src/mswin/mswinsessmgr.hpp    src/mswin/mswinsessmgr.cpp
  src/mswin/mswinsurf.hpp       src/mswin/mswinsurf.cpp
  src/mswin/mswinogl.hpp        src/mswin/mswinogl.cpp
  src/mswin/mswinfontprov.hpp   src/mswin/mswinfontprov.cpp
  src/mswin/mswinfont.hpp       src/mswin/mswinfont.cpp
  src/mswin/mswinerr.hpp        src/mswin/mswinerr.cpp
  src/mswin/mswinwindow.hpp
  src/mswin/mswinevt.hpp        src/mswin/mswinevt.cpp
  src/mswin/mswincanvas.hpp

  util/format.hpp
  util/text.hpp                 src/util/text.cpp
  util/errlog.hpp
  util/bitset.hpp
)

target_compile_definitions(GSM PUBLIC GSM_STATIC)

# message("GSM source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(GSM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

if (WIN32)
  target_link_libraries(GSM PRIVATE OpenGL32 GLU32)
else()
  target_link_libraries(GSM PRIVATE opengl glu)
endif()

#message("CMAKE_CXX_LIBRARY_ARCHITECTURE: ${CMAKE_CXX_LIBRARY_ARCHITECTURE}")
#message("CMAKE_LIBRARY_ARCHITECTURE: ${CMAKE_LIBRARY_ARCHITECTURE}")
find_package(GLEW REQUIRED)
target_link_libraries(GSM PUBLIC ${GLEW_LIBRARY})
message(${GLEW_INCLUDE_DIR})
target_include_directories(GSM PUBLIC ${GLEW_INCLUDE_DIR})
