cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project("phantom-pi"
        LANGUAGES CXX
        VERSION   0.1.0
       )

enable_testing()

function(make_test test)
    add_executable(${test} ${ARGN})
    target_link_libraries(${test}
                            gtest
                            gtest_main
                         )
    add_test(${test} ${test})
endfunction()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_subdirectory(test)
