cmake_minimum_required(VERSION 3.22.1)
project(MinerLowC LANGUAGES C)

# Flags
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSION OFF)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
set(COMPILE_WARNING_AS_ERROR ON)

# sets directory
set(HERE ${CMAKE_SOURCE_DIR})
set(INCLUDE_DIR ${HERE}/include)
set(SRC_DIR ${HERE}/src)
set(TSRC_DIR ${HERE}/test_src)
set(BUILD_DIR ${HERE}/build)

# sets output place
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR})

file(GLOB_RECURSE SRCS "${SRC_DIR}/*.c")

if (DO_TEST)
    enable_testing()
    list(REMOVE_ITEM SRCS ${SRC_DIR}/main.c)
    add_executable(sha256_test
        ${SRCS}
        ${TSRC_DIR}/hash/sha256_test.c
    )
    target_include_directories(sha256_test PRIVATE
        ${INCLUDE_DIR}
    )
    target_compile_options(sha256_test PRIVATE -O0)
    add_test(
        NAME "Hash SHA256 Test"
        COMMAND build/sha256_test test_file/sha256msg.txt
    )
else()
    ## Main
    add_executable(${PROJECT_NAME} ${SRCS})
    
    target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
    target_compile_definitions(${PROJECT_NAME} PRIVATE
      PROJECT_NAME="${PROJECT_NAME}"
      PROJECT_VERSION="0.0.0 Debug"
    )
endif()