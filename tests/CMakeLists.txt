cmake_minimum_required(VERSION 3.5)

add_subdirectory(ext/catch2)
include_directories(ext/catch2/single_include/catch2)
include(Catch)

function(create_test NAME)
    add_executable("${NAME}_tests" "${NAME}_tests.cpp")
    target_link_libraries("${NAME}_tests" cg Catch2::Catch2)
    catch_discover_tests("${NAME}_tests")
endfunction()

create_test(displayfile)
create_test(shapes)
