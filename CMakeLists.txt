cmake_minimum_required(VERSION 3.0)
project(AstarAlgorithm)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/piksel)

set(MAIN_FILE ${PROJECT_SOURCE_DIR}/src/AstarAlgorithm.cpp)

file(GLOB_RECURSE SRC_FILES "src/*")

# remove main sources
LIST(REMOVE_ITEM SRC_FILES ${MAIN_FILE})

get_filename_component(FILE_NAME ${MAIN_FILE} NAME_WE)
add_executable(${FILE_NAME} ${MAIN_FILE} ${SRC_FILES})
target_include_directories(${FILE_NAME} PUBLIC src PUBLIC lib/piksel/src)
target_link_libraries(${FILE_NAME} piksel)