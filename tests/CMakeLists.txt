find_package(GTest REQUIRED)
link_libraries(GTest::GTest GTest::Main)

link_libraries(MusicManagingSystem)
include_directories(${MUSIC_MANAGING_SYSTEM_INCLUDE_DIRS})

find_package(SQLite3 REQUIRED)
link_libraries(${SQLite3_LIBRARIES})

add_executable(UtilsTest src/test.c)
add_test(NAME TestUtils COMMAND UtilsTest)
