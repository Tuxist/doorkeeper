find_package(httppp REQUIRED)

set(pidoorkeeperSrcs
  controller.cpp
  main.cpp
  modules.cpp
)

include_directories(
  ${httppp_INCLUDE_DIR}
)

add_executable (pidoorkeeper ${pidoorkeeperSrcs})

target_link_libraries(pidoorkeeper ${httppp_LIBRARIES} dl)

install(TARGETS pidoorkeeper DESTINATION bin)

add_subdirectory(modules)
