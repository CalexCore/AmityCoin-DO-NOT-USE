# This file is attended to be invoked by a custom target ${CMAKE_COMMAND}
# conditionally updating the version definition.

file(READ ${AMITY_VERSION_INPUT_FILE} AMITY_VERSION)
string(REPLACE "." ";" VERSION_LIST ${AMITY_VERSION})
list(GET VERSION_LIST 0 AMITY_VERSION_MAJOR)
list(GET VERSION_LIST 1 AMITY_VERSION_MINOR)
list(GET VERSION_LIST 2 AMITY_VERSION_PATCH)
list(GET VERSION_LIST 3 AMITY_VERSION_TWEAK)
message(STATUS "[AMITY] Current Version: ${AMITY_VERSION}")

if(DEFINED ENV{APPVEYOR})
    set(GIT_BRANCH $ENV{APPVEYOR_REPO_BRANCH})
    set(GIT_COMMIT_HASH $ENV{APPVEYOR_REPO_COMMIT})
else()
    find_package(Git QUIET)
    if(Git_FOUND)
        # Get the current working branch
        execute_process(
            COMMAND git rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_BRANCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        # Get the latest abbreviated commit hash of the working branch
        execute_process(
            COMMAND git log -1 --format=%h
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT_HASH
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        message(STATUS "[AMITY] Git Version:\n\tBranch:\t${GIT_BRANCH}\n\tCommit:\t${GIT_COMMIT_HASH}")
    else()
        set(GIT_BRANCH "NA")
        set(GIT_COMMIT_HASH "NA")
    endif()
endif() # DEFINED ENV{APPVEYOR}

set(AMITY_VERSION_FILE "${AMITY_VERSION_INCLUDE_DIR}/version.h")
set(tempfile "${CMAKE_BINARY_DIR}/version.h")
configure_file(${AMITY_VERSION_TEMPLATE} ${tempfile})

if(EXISTS ${AMITY_VERSION_FILE})
    file(MD5 ${AMITY_VERSION_FILE} currentHash)
    file(MD5 ${tempfile} newHash)

    if(NOT currentHash STREQUAL newHash)
        file(COPY ${tempfile} DESTINATION ${AMITY_VERSION_INCLUDE_DIR})
    endif()
else()
    file(COPY ${tempfile} DESTINATION ${AMITY_VERSION_INCLUDE_DIR})
endif()
