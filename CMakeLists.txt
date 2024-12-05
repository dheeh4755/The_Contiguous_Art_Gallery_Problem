cmake_minimum_required(VERSION 3.12...3.29)
project(ContiguousArtGallery)

set(CMAKE_CXX_STANDARD 20)

#CGAL_Qt6 is needed for the drawing.
find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt6)

add_executable(main main.cpp Algorithm1.cpp
        "Algorithm2.cpp"
        "Algorithm2.h"
        Helper.cpp
        Helper.h)
target_link_libraries(main PRIVATE CGAL::CGAL)


if(CGAL_Qt6_FOUND)

    target_link_libraries(main PUBLIC CGAL::CGAL_Basic_viewer)
endif()

