cmake_minimum_required(VERSION 3.31)

project(e_programming_language)

################################################################################
## Output directories
################################################################################

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)





file(GLOB CPP_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB C_FILES ${CMAKE_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME} ${CPP_FILES} ${C_FILES})



################################################################################
## Compiler flags
################################################################################

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
        /WX
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD   11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)



target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include/
)


