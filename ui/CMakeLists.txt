add_executable(Main src/ui.c)
target_link_libraries(Main PRIVATE MusicManagingSystem)
target_include_directories(Main PUBLIC include)

install(TARGETS Main RUNTIME DESTINATION bin)
