cmake_minimum_required (VERSION 3.8)

project ("LibrariesTest")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#set(CMAKE_PREFIX_PATH "D:/ProgramData/SDK/msvc2019_64")

set(CMAKE_CXX_STANDARD 17)

add_subdirectory ("LibrariesTest")
