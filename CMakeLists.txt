cmake_minimum_required(VERSION 3.15)
project(workspace-controller)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 정적 빌드 옵션
option(BUILD_STATIC "Build static executable" OFF)

if(BUILD_STATIC)
  message(STATUS "Static Build Mode")
  
  if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    if(NOT DEFINED VCPKG_TARGET_TRIPLET)
      set(VCPKG_TARGET_TRIPLET "x64-windows-static")
    endif()
  else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  endif()
endif()

# Find libarchive
if(MSVC)
  find_package(LibArchive REQUIRED)
else()
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(LIBARCHIVE REQUIRED libarchive)
endif()

add_executable(
  workspace-controller
  src/main.cc
  src/utils/utils.cc
  src/services/workspaceService.cc
  src/controllers/httpController.cc
  src/controllers/robotController.cc
  src/controllers/workspaceController.cc
)

target_include_directories(workspace-controller
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/libs
)

if(MSVC)
  target_link_libraries(workspace-controller
    PRIVATE
      LibArchive::LibArchive
  )
  
else()
  target_include_directories(workspace-controller
    PRIVATE
      ${LIBARCHIVE_INCLUDE_DIRS}
  )
  
  if(BUILD_STATIC)
    execute_process(
      COMMAND pkg-config --libs --static libarchive
      OUTPUT_VARIABLE LIBARCHIVE_STATIC_LIBS
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    execute_process(
      COMMAND pkg-config --libs --static libxml2
      OUTPUT_VARIABLE LIBXML2_STATIC_LIBS
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    target_link_libraries(workspace-controller
      PRIVATE
        ${LIBARCHIVE_STATIC_LIBS}
        ${LIBXML2_STATIC_LIBS}
        icuuc
        icudata
        icui18n
        z        
        lzma     
        pthread
    )
    
    target_link_options(workspace-controller
      PRIVATE
        -static-libgcc
        -static-libstdc++
    )
  else()
    target_link_libraries(workspace-controller
      PRIVATE
        ${LIBARCHIVE_LIBRARIES}
    )
  endif()
endif()
