cmake_minimum_required(VERSION 3.0)

project(boost_spsc_demo)

add_definitions(-std=c++14)

find_package(Boost 1.67 COMPONENTS system)
if (Boost_fOUND)
  include_directories(${Boost_INCLUDE_DIR})
endif()


add_executable(boost_sesc_demo main.cc)
