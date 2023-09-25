cmake_minimum_required(VERSION 3.16)

if (DEFINED ENV{GITHUB_RUN_NUMBER})
    set(BUILD_NUMBER $ENV{GITHUB_RUN_NUMBER})
else()
    set(BUILD_NUMBER 1)
endif()


project(allocator VERSION 0.0.1.${BUILD_NUMBER})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# add_library(appLib src/filter.cpp
#                     src/ip.cpp
#             )
# target_include_directories(appLib 
#                             PUBLIC include
#                             )
# target_include_directories(appLib 
#                             PUBLIC 
#                             ${range-v3_INCLUDE_DIRS}
#                             )

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)
# target_link_libraries(${CMAKE_PROJECT_NAME} appLib)

add_executable(appTest test/src/test.cpp)
target_include_directories(appTest PRIVATE include)
target_link_libraries(appTest ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    # COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

include(CTest)
enable_testing()
add_test(appTest appTest)

install(TARGETS ${CMAKE_PROJECT_NAME}
        RUNTIME DESTINATION bin
        )

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Sergei Rybin") #required
include(CPack)

