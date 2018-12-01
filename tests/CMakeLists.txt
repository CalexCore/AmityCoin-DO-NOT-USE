include_directories(${CMAKE_SOURCE_DIR}/external/rocksdb/include)
include_directories(${CMAKE_SOURCE_DIR}/external/linenoise)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/external)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-undef")
endif() # GCC

if(MSVC)
    set(ROCKSDB_LIB rocksdb)
else()
    set(ROCKSDB_LIB rocksdblib)
endif()

# copy test data
if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/DB")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/DB" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

# integration tests
file(GLOB_RECURSE int_tests "${CMAKE_CURRENT_SOURCE_DIR}/integration/*.cpp")
source_group("" FILES ${int_tests})
add_executable(TestSuite.Integration ${int_tests})
target_link_libraries(TestSuite.Integration PRIVATE gtest_main Common Crypto CryptoNoteCore Serialization Logging ${ROCKSDB_LIB})

add_test(Integrity TestSuite.Integration WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# benchmarks
file(GLOB_RECURSE int_tests "${CMAKE_CURRENT_SOURCE_DIR}/benchmark/*")
source_group("" FILES ${int_tests})
add_executable(TestSuite.Benchmark ${int_tests})
target_link_libraries(TestSuite.Benchmark PRIVATE benchmark_main Common Crypto CryptoNoteCore Serialization Logging ${ROCKSDB_LIB})
target_include_directories(TestSuite.Benchmark PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/benchmark")
