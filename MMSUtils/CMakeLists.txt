find_package(SQLite3 REQUIRED)

aux_source_directory(src SOURCES)
add_library(MMSUtils SHARED ${SOURCES} MMSUtils.def)
target_include_directories(MMSUtils PUBLIC include)
target_include_directories(MMSUtils PUBLIC ${SQLite3_INCLUDE_DIRS})
target_link_libraries(MMSUtils PRIVATE ${SQLite3_LIBRARIES})

# Windows下可执行文件按照动态库原名寻找库文件，而cmake使用gcc编译器时会按照unix的习惯在名称前加lib，故需要手动将前缀设置为空
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    set_target_properties(MMSUtils PROPERTIES PREFIX "")
  endif()
endif()

install(
  TARGETS MMSUtils
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib)
