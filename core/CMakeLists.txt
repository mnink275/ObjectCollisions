file(
    GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp
)
file(
    GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp
)

add_library(
    ${PROJECT_NAME}_objs OBJECT ${SOURCES} ${HEADERS}
)
target_include_directories(
    ${PROJECT_NAME}_objs PUBLIC ${PROJECT_SOURCE_DIR}/core/include
)

target_link_libraries(
    ${PROJECT_NAME}_objs sfml-graphics
)

target_compile_definitions(
    ${PROJECT_NAME}_objs PRIVATE RESOURCE_FOLDER="${RESOURCE_FOLDER}"
)

# IDEs should put the headers in a nice place
source_group(
    TREE "${PROJECT_SOURCE_DIR}/core/include"
    PREFIX "Header Files"
    FILES ${HEADERS}
)
