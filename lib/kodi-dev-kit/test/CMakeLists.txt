add_executable(kodisandbox main_parent.cpp)
target_link_libraries(kodisandbox PUBLIC kodisandbox-server)

add_executable(kodichild main_child.cpp)
target_link_libraries(kodichild PUBLIC kodi-dev-kit)
