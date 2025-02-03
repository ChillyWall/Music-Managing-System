add_executable(MusicManagementSystem src/ui.c)
target_link_libraries(MusicManagementSystem PRIVATE MMSUtils)
target_include_directories(MusicManagementSystem PUBLIC include)

install(TARGETS MusicManagementSystem RUNTIME DESTINATION bin)
