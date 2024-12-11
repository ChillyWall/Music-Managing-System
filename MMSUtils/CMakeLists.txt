find_package(SQLite3 REQUIRED)

aux_source_directory(src SOURCES)
add_library(MMSUtils SHARED ${SOURCES} MMSUtils.def)
target_include_directories(MMSUtils PUBLIC include)
target_include_directories(MMSUtils PUBLIC ${SQLite3_INCLUDE_DIRS})
target_link_libraries(MMSUtils PRIVATE ${SQLite3_LIBRARIES})

install(
  TARGETS MMSUtils
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)
