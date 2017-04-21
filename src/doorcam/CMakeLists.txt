find_package(httppp REQUIRED)

set(doorcamSrcs
  doorcam.cpp
  camevent.cpp
  recordcam.cpp
)

include_directories(
  ${httppp_INCLUDE_DIR}
)

add_executable (doorcam ${doorcamSrcs})

target_link_libraries(doorcam ${httppp_LIBRARIES} dl pthread)

install(TARGETS doorcam DESTINATION bin)
