set(indexSrcs index.cpp)
add_library (index SHARED ${indexSrcs})
target_link_libraries(index ${httppp_LIBRARIES})
install(TARGETS index DESTINATION lib/pidoorkeeper)

set(cameraSrcs camera.cpp)
add_library (camera SHARED ${cameraSrcs})
target_link_libraries(camera ${httppp_LIBRARIES})
install(TARGETS camera DESTINATION lib/pidoorkeeper)
