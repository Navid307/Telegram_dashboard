cmake_minimum_required(VERSION 3.16.5)

project(dashboard)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-DCHAT_ID="${CHAT_ID}" -DBOT_TOKEN="${BOT_TOKEN}")

set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)