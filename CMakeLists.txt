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

add_subdirectory(compile_check)
add_subdirectory(example)
add_subdirectory(test)

install(FILES
            include/phantompi/button.hpp
            include/phantompi/file.hpp
            include/phantompi/handle.hpp
            include/phantompi/types.hpp
        DESTINATION include/phantompi)

install(FILES
            include/phantompi/sysfs/gpio.hpp
            include/phantompi/sysfs/phantompi.hpp
        DESTINATION include/phantompi/sysfs)
