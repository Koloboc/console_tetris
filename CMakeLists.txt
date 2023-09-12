cmake_minimum_required(VERSION 3.7)
project(game_tetris)
set(PROJECT_TARGET game_tetris)

set(INCLUDES_DIR include)

file(GLOB LIST_SOURCES src/*.c)
file(GLOB LIST_HEADERS "${INCLUDES_DIR}/*.h")
set(ALL_SOURCES ${LIST_HEADERS} ${LIST_SOURCES})

message("source = " ${LIST_SOURCES})
message("headers = " ${LIST_HEADERS})

add_executable(${PROJECT_TARGET} ${ALL_SOURCES})
#target_include_directories(${PROJECT_TARGET} PRIVATE ${INCLUDES_DIR})

SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/bin
        CACHE PATH
        "Single Directory for all"
    )

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/bin
        CACHE PATH
        "Single Directory for all"
    )

SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/bin
        CACHE PATH
        "Single Directory for all"
    )

#========================================

target_compile_options(${PROJECT_TARGET} PRIVATE -g -Wall -DDebug)
#target_link_options(${PROJECT_TARGET} PRIVATE -lm)
#target_compile_options(${PROJECT_TARGET} PRIVATE -Wall)


