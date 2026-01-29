cmake_minimum_required(VERSION 3.8)
project(ufomap_msgs)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)

# If your msg files depend on other interface packages, list them here too.

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/UFOMap.msg"
  "msg/UFOMapStamped.msg"
  DEPENDENCIES std_msgs
)

# If you really have headers under include/ that you want to install:
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
)

ament_export_dependencies(rosidl_default_runtime)
ament_package()
