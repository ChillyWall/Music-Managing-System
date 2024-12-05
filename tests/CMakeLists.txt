link_libraries(MusicManagingSystem)
include_directories(${MUSIC_MANAGING_SYSTEM_INCLUDE_DIRS})

add_executable(UtilsTest src/test_utils.c)
add_test(NAME TestUtils COMMAND UtilsTest)
