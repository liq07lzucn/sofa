cmake_minimum_required(VERSION 3.12)
project(SofaLoader)

sofa_find_package(TinyXML REQUIRED)

set(HEADER_FILES
    BaseVTKReader.h
    BaseVTKReader.inl
    MeshObjLoader.h
    MeshVTKLoader.h
    config.h
    initLoader.h
)

set(SOURCE_FILES
    BaseVTKReader.cpp
    BaseVTKReader.inl
    MeshObjLoader.cpp
    MeshVTKLoader.cpp
    initLoader.cpp
)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC SofaSimulationTree SofaHelper)
target_link_libraries(${PROJECT_NAME} PRIVATE tinyxml) # Private because not exported in API

sofa_install_targets(SofaCommon ${PROJECT_NAME} "SofaCommon/${PROJECT_NAME}")
