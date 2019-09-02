cmake_minimum_required (VERSION 3.8)

find_package(nlohmann_json REQUIRED)
find_package(tinyxml2 REQUIRED)
find_package(pugixml REQUIRED)
find_package(RapidJSON REQUIRED)

file(GLOB LIBRARIES_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

file(GLOB TINYXML_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/tinyxml/*.cpp)

include_directories(${RapidJSON_INCLUDE_DIR})

add_executable (LibrariesTest
    ${LIBRARIES_TEST_SOURCES}
    ${TINYXML_SOURCES})

target_link_libraries(LibrariesTest 
    nlohmann_json::nlohmann_json
    pugixml
    tinyxml2::tinyxml2)