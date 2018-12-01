# Required for finding Threads on ARM
find_package(Threads)

## Go get us some static BOOST libraries
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(
    Boost 1.65.1 REQUIRED

    COMPONENTS
        system
        filesystem
        thread
        date_time
        chrono
        regex
        serialization
        program_options
)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
if(APPLE)
  set(Boost_LIBRARIES "${Boost_LIBRARIES}")
elseif(NOT MSVC)
  set(Boost_LIBRARIES "${Boost_LIBRARIES};rt")
endif()
