add_executable(MusicManagingSystem src/ui.c)
target_link_libraries(MusicManagingSystem PRIVATE MMSUtils)
target_include_directories(MusicManagingSystem PUBLIC include)

install(TARGETS MusicManagingSystem RUNTIME DESTINATION bin)
