cmake_minimum_required(VERSION 3.9.2)

include(cmake/ApplyDefaultBuildType.cmake)

project(
  AmityCoin
  
  DESCRIPTION
    "AmityCoin brings you a truly private cryptocurrency you can trust"

  LANGUAGES 
    C 
    CXX
)

## This section describes our general CMake setup options
set(CMAKE_SUPPRESS_REGENERATION ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

## Options for configuring build system
option(AMITY_BUILD_EXECUTABLES "Builds the executables for this project (daemon, wallet...)." ON)
option(AMITY_BUILD_CRYPTOTEST "Buils the cryptotest target for testing purposes." OFF)

include(cmake/CompilerConfiguration.cmake)
include(cmake/Dependencies.cmake)
include(cmake/Platform.cmake)
include(cmake/Version.cmake)

## Enable test integration
if(AMITY_BUILD_CRYPTOTEST)
  enable_testing()
endif() # AMITY_BUILD_CRYPTOTEST

add_subdirectory(external)
add_subdirectory(src)
