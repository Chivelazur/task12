cmake_minimum_required (VERSION 3.12)
project (UNIORB)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE test_src
    ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

add_executable(uniorb_test ${test_src})

if(MSVC)
    target_compile_options(uniorb_test PRIVATE /MP /std:c++17)
endif(MSVC)


add_custom_command(
    TARGET uniorb_test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:uniorb_test>
        ${PROJECT_SOURCE_DIR}/release/
)