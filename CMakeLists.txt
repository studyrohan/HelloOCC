cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# Project name
project(HelloOCC CXX)

# OpenCascade
find_package(OpenCASCADE)

# Configure C++ compiler's includes dir
include_directories(SYSTEM ${OpenCASCADE_INCLUDE_DIR})

# Add executable
add_executable(HelloOCC
 main.cpp
 Viewer.cpp
 Viewer.h
 ViewerInteractor.cpp
 ViewerInteractor.h
)

# Add linker options
foreach(LIB ${OpenCASCADE_LIBRARIES})
  target_link_libraries(HelloOCC debug     ${OpenCASCADE_LIBRARY_DIR}d/${LIB}.lib)
  target_link_libraries(HelloOCC optimized ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()

# Adjust runtime environment
set_property(TARGET HelloOCC PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=$<$<CONFIG:DEBUG>:${OpenCASCADE_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${OpenCASCADE_BINARY_DIR}>;%PATH%")
