cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project("phantom-pi"
        LANGUAGES CXX
        VERSION   0.1.0
       )

option(BUILD_TESTS "Build optional tests" ON)
option(BUILD_DOCS  "Build documenttaion" ON)

enable_testing()

if(BUILD_TESTS)
    find_library(GTEST gtest)
    find_library(GTEST_MAIN gtest_main)
    message(STATUS "gtest location: ${GTEST}")
    message(STATUS "gtest_main location: ${GTEST_MAIN}")
    if(NOT GTEST OR NOT GTEST_MAIN)
        message(WARNING "Could not find gtest libraries; unit tests disabled")
        set(BUILD_TESTS OFF)
    endif()
endif()

if(BUILD_TESTS)
    function(make_test test)
        add_executable(${test} ${ARGN})
        target_link_libraries(${test}
                                ${GTEST}
                                ${GTEST_MAIN}
                             )
        add_test(${test} ${test} "--gtest_output=xml:${CMAKE_CURRENT_BINARY_DIR}/${test}_results.xml")
    endfunction()
else()
    function(create_test name)
        #do nothing
    endfunction()
endif()

if(BUILD_DOCS)
    find_program(DOXYGEN "doxygen")
    if(DOXYGEN)
        message(STATUS "doxygen location: ${DOXYGEN}")
        find_program(DOT "dot")
        if(DOT)
            message(STATUS "dot location: ${DOT}")
            set(HAVE_DOT "YES")
        else()
            set(HAVE_DOT "NO")
        endif()
        configure_file(doxyfile.in "${CMAKE_CURRENT_BINARY_DIR}/doxyfile" @ONLY)

        add_custom_target(docs)
        add_custom_command(TARGET docs
                                COMMAND "${DOXYGEN}"
                                WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                          )
    else()
        message(WARNING "Couldn't find doxygen, disabling documentation")
    endif()
endif()

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
            include/phantompi/led.hpp
            include/phantompi/types.hpp
        DESTINATION include/phantompi)

install(FILES
            include/phantompi/sysfs/gpio.hpp
            include/phantompi/sysfs/phantompi.hpp
        DESTINATION include/phantompi/sysfs)
