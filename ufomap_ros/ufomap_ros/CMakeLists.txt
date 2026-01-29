cmake_minimum_required(VERSION 3.8)
project(ufomap_ros)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional but common in ROS 2
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ament + ROS 2 deps
find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(ufomap_msgs REQUIRED)

# External ufomap (same as ROS 1)
find_package(ufomap REQUIRED)

# Library
add_library(${PROJECT_NAME} STATIC
  src/conversions.cpp
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Export ROS 2 dependencies to downstream packages
ament_target_dependencies(${PROJECT_NAME}
  geometry_msgs
  sensor_msgs
  ufomap_msgs
)

# Link ufomap
target_link_libraries(${PROJECT_NAME}
  UFO::Map
)

# Install library + headers
install(TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
)

# Export for downstream
ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_include_directories(include)
ament_export_dependencies(geometry_msgs sensor_msgs ufomap_msgs)
ament_export_libraries(${PROJECT_NAME})

ament_package()
