
cmake_minimum_required(VERSION 3.10)

project(NoggitServer)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_MODULE_PATH    "${CMAKE_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})
set(EXECUTABLE_OUTPUT_PATH "bin")
set(LIBARY_OUTPUT_PATH "bin")
# Any compiler options here
set (CMAKE_CXX_LINK_FLAGS "-no-pie" ${CMAKE_CXX_LINK_FLAGS})

# Any packages here
find_package(Boost 1.60 REQUIRED COMPONENTS thread filesystem system)
find_package(StormLib REQUIRED)
find_package(fmt REQUIRED)

include (AutoCollect)
CollectSourceFiles ("${CMAKE_SOURCE_DIR}/src" SOURCE)
GroupSources ("${CMAKE_SOURCE_DIR}/src")
CollectIncludeDirectories ("${CMAKE_SOURCE_DIR}/src" INCLUDES)

add_executable (noggit-server ${SOURCE})

target_include_directories (noggit-server PUBLIC ${INCLUDES})

target_link_libraries (noggit-server Boost::thread Boost::filesystem Boost::system )
target_link_libraries (noggit-server StormLib)
target_link_libraries (noggit-server fmt::fmt)
