find_package(httppp REQUIRED)

set(picamSrcs
  picam.cpp
  camevent.cpp
  recordcam.cpp
)

include_directories(
  ${httppp_INCLUDE_DIR}
)

add_executable (picam ${picamSrcs})

target_link_libraries(picam ${httppp_LIBRARIES} dl pthread)

install(TARGETS picam DESTINATION bin)
