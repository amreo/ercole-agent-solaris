cmake_minimum_required(VERSION 3.5)

project(ercole-agent)

list(APPEND
  CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/cmake"
)

# C++11 or c++0x support check
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11 " COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "-std=c++11")
    set (CMAKE_CXX_STANDARD 11)
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
    set (CMAKE_CXX_STANDARD 11)
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
#set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Poco REQUIRED COMPONENTS Foundation Util XML JSON Net) 
include_directories(${Poco_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} "main.cpp" "configuration.cpp" "unmarshaller.cpp" "utils.cpp" "whereami.c" "whereami++.cpp")
message("${POCO_Libraries}")
target_link_libraries(${PROJECT_NAME} ${Poco_LIBRARIES})
