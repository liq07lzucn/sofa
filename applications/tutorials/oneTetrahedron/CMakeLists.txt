cmake_minimum_required(VERSION 3.12)
project(oneTetrahedron)

find_package(SofaGui)
find_package(SofaOpenglVisual REQUIRED)

add_executable(${PROJECT_NAME} oneTetrahedron.cpp)
target_link_libraries(${PROJECT_NAME} SofaGuiMain SofaOpenglVisual)
