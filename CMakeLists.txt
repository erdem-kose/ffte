cmake_minimum_required(VERSION 3.0.0)
project(ffte VERSION 0.1.0)

#C flags section
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -O2 ")

#Options Section
option(FFTE_AVX_ENABLE "Use AVX2 instructions for FFT calculations" ON)

#Sources Section
file(GLOB_RECURSE SRC src/*.c)
file(GLOB_RECURSE UTILS utils/*.c)

add_executable(ffte main.c ${SRC} ${UTILS})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/ffte ${PROJECT_SOURCE_DIR}/main
)

target_link_libraries(${PROJECT_NAME} m plplot)

if(FFTE_AVX_ENABLE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFFTE_AVX_ENABLE -mavx2 -march=native")
endif()

#Install section
