cmake_minimum_required (VERSION 3.8)

find_package(nlohmann_json REQUIRED)
find_package(tinyxml2 REQUIRED)
find_package(pugixml REQUIRED)
find_package(Poco REQUIRED COMPONENTS JSON XML)

file(GLOB LIBRARIES_TEST_SOURCES *.cpp *.hpp *.h)

add_executable (LibrariesTest ${LIBRARIES_TEST_SOURCES})

target_link_libraries(LibrariesTest 
    nlohmann_json::nlohmann_json
    pugixml
    tinyxml2::tinyxml2
    Poco::XML)