cmake_minimum_required(VERSION 3.0.0)
project(ffte VERSION 0.1.0)

include(CTest)
enable_testing()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu11 -Ofast")
file(GLOB_RECURSE SRC src/*.c)
add_executable(ffte main.c ${SRC})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_BINARY_DIR}/ffte ${PROJECT_SOURCE_DIR}/main
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

target_link_libraries(${PROJECT_NAME} m plplot)
include(CPack)
