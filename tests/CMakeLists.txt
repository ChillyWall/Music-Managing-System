add_executable(Test src/test.c)
target_link_libraries(Test PRIVATE MusicManagingSystem)
target_include_directories(Test PRIVATE ${MUSIC_MANAGING_SYSTEM_INCLUDE_DIRS})

