foreach(flag ${CompilerFlags})
  set(${flag} "${${flag}} ${AMITY_CXX_FLAGS}")
endforeach()

# miniupnpc ========================================================================================
set(UPNPC_BUILD_STATIC ON CACHE BOOL "Build static library")
set(UPNPC_BUILD_SHARED OFF CACHE BOOL "Build shared library")
set(UPNPC_BUILD_TESTS OFF CACHE BOOL "Build test executables")

add_subdirectory(miniupnpc)

if(MSVC)
  set_property(TARGET upnpc-static APPEND_STRING PROPERTY COMPILE_FLAGS " -wd4244 -wd4267")
elseif(NOT MSVC)
  set_property(TARGET upnpc-static APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-undef -Wno-unused-result -Wno-unused-value")
endif()

mark_as_advanced(
    UPNPC_BUILD_SHARED
    UPNPC_BUILD_STATIC
    UPNPC_BUILD_TESTS
)

if (AMITY_BUILD_TESTSUITE)
  # gtest ==========================================================================================
  add_subdirectory(gtest)
  set_property(TARGET gtest gtest_main PROPERTY FOLDER "external")
  set_property(TARGET gtest gtest_main PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/gtest/include")
  # benchmark ======================================================================================
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "TestSuite of google-benchmark disabled" FORCE)
  add_subdirectory(benchmark)
  set_property(TARGET benchmark benchmark_main PROPERTY FOLDER "external")
  mark_as_advanced(
      BENCHMARK_ENABLE_TESTING
      BENCHMARK_ENABLE_EXCEPTIONS
      BENCHMARK_ENABLE_LTO
      BENCHMARK_USE_LIBCXX
      BENCHMARK_BUILD_32_BITS
      BENCHMARK_ENABLE_INSTALL
      BENCHMARK_DOWNLOAD_DEPENDENCIES
      BENCHMARK_ENABLE_GTEST_TESTS
      BENCHMARK_ENABLE_ASSEMBLY_TESTS
  )
endif()

mark_as_advanced(
    gtest_build_samples
    gtest_build_tests
    gtest_disable_pthreads
    gtest_force_shared_crt
    BUILD_SHARED_LIBS
)

# rocksDB ==========================================================================================

## This section is specifically for RocksDB build options that we've disabled for maximum portability
set(ENABLE_AVX OFF CACHE STRING "Enable RocksDB AVX/AVX2? Defaults to OFF")
set(ENABLE_LEAF_FRAME OFF CACHE STRING "Enable RocksDB OMIT_LEAF_FRAME_POINTER detection? Defaults to OFF")
set(ENABLE_SSE42 OFF CACHE STRING "Enable RocksDB SSE4.2 support detection? Defaults to OFF")
set(ENABLE_THREAD_LOCAL OFF CACHE STRING "Enable RocksDB THREAD_LOCAL support detection? Defaults to OFF")
set(ENABLE_SYNC_FILE_RANGE_WRITE OFF CACHE STRING "Enable RocksDB SYNC_FILE_RANGE_WRITE support detection? Defaults to OFF")
set(ENABLE_PTHREAD_MUTEX_ADAPTIVE_NP OFF CACHE STRING "Enable RocksDB PTHREAD_MUTEX_ADAPTIVE_NP support detection? Defaults to OFF")
set(ENABLE_MALLOC_USABLE_SIZE OFF CACHE STRING "Enable RocksDB MALLOC_USABLE_SIZE support detection? Defaults to OFF")
set(ENABLE_SCHED_GETCPU OFF CACHE STRING "Enable RocksDB SCHED_GETCPU support detection? Defaults to OFF")
set(WITH_MD_LIBRARY OFF CACHE BOOL "Code Generation for static library")

mark_as_advanced(
    ENABLE_AVX
    ENABLE_LEAF_FRAME
    ENABLE_SSE42
    ENABLE_THREAD_LOCAL
    ENABLE_SYNC_FILE_RANGE_WRITE
    ENABLE_PTHREAD_MUTEX_ADAPTIVE_NP
    ENABLE_MALLOC_USABLE_SIZE
    ENABLE_SCHED_GETCPU

    ROCKSDB_INSTALL_ON_WINDOWS
    ROCKSDB_LITE

    WITH_JEMALLOC
    WITH_SNAPPY
    WITH_LZ4
    WITH_ZLIB
    WITH_ZSTD
    WITH_GFLAGS
    WITH_ASAN
    WITH_TSAN
    WITH_UBSAN
    WITH_NUMA
    WITH_TBB
    WITH_FALLOCATE
    WITH_JNI
    WITH_TOOLS
    WITH_LIBRADOS
    WITH_MD_LIBRARY
    WITH_TESTS

    PORTABLE
    FORCE_SSE42
    FAIL_ON_WARNINGS
    DISABLE_STALL_NOTIF
    OPTDBG
)

if(MSVC)
    mark_as_advanced(
        WITH_XPRESS
    )
endif()

if(MSVC)
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../version)
    add_subdirectory(rocksDB EXCLUDE_FROM_ALL)
    set_property(TARGET upnpc-static rocksdb PROPERTY FOLDER "external")
else()
    set_property(TARGET upnpc-static PROPERTY FOLDER "external")
    add_custom_target(
       rocksdb
       COMMAND $(MAKE) rocksdb
       WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/rocksdb
    )
    add_library(rocksdblib STATIC IMPORTED GLOBAL)
    set_target_properties(rocksdblib PROPERTIES IMPORTED_LOCATION ${PROJECT_BINARY_DIR}/rocksdb/librocksdb.a)
    add_dependencies(rocksdblib rocksdb)

    ## We need to setup the RocksDB build environment to match our system
    execute_process(
      COMMAND cmake ${PROJECT_SOURCE_DIR}/external/rocksdb -DWITH_GFLAGS=0 -DCMAKE_BUILD_TYPE=MinSizeRel -DWITH_TESTS=OFF -DWITH_TOOLS=OFF -DPORTABLE=ON -B${PROJECT_BINARY_DIR}/rocksdb
    )
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_BINARY_DIR}/rocksdb/librocksdb.a")
endif()
