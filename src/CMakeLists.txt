find_package(httppp REQUIRED)

include_directories(
  ${httppp_INCLUDE_DIR}
)

add_subdirectory(picam)
