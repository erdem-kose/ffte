cmake_minimum_required(VERSION 3.31)

# Project name
project(ffte VERSION 0.1.0)

# Specify the version of the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source directories
file(GLOB_RECURSE SOURCE_C_FILES "src/*.c*")
file(GLOB_RECURSE SOURCE_H_FILES "src/*.h*")
file(GLOB_RECURSE UTILS_C_FILES "utils/*.c*")
file(GLOB_RECURSE UTILS_H_FILES "utils/*.h*")
file(GLOB_RECURSE TEST_C_FILES "tests/*.c*")
file(GLOB_RECURSE TEST_H_FILES "tests/*.h*")
set(ZLIB_PATH "lib/external/zlib/installfolder/")
set(LIBPNG_PATH "lib/external/libpng/installfolder/")
set(LIBGD_PATH "lib/external/libgd/installfolder/")
set(PLPLOT_PATH "lib/external/plplot/installfolder/")
set(GOOGLETEST_PATH "lib/external/googletest/")

# Add the executable
link_directories("${ZLIB_PATH}/lib/"
                 "${LIBPNG_PATH}/lib/"
                 "${LIBGD_PATH}/lib/"
                 "${PLPLOT_PATH}/lib/"
                 "${PLPLOT_PATH}/lib/qsastime/"
                 "${GOOGLETEST_PATH}/lib/")

add_executable(${PROJECT_NAME} ${SOURCE_C_FILES} 
                               ${TEST_C_FILES} 
                               ${UTILS_C_FILES})

target_link_libraries(${PROJECT_NAME} gtest gtest_main m pthread zlibstatic png gd plplot qsastime )
target_include_directories(${PROJECT_NAME} PRIVATE "${ZLIB_PATH}/include/"
                                                   "${LIBPNG_PATH}/include/"
                                                   "${LIBGD_PATH}/include/"
                                                   "${PLPLOT_PATH}/include/"
                                                   "${GOOGLETEST_PATH}/include/")

# Testing
enable_testing()
add_test(NAME example_test COMMAND ${PROJECT_NAME})
