find_package(SQLite3 REQUIRED)

aux_source_directory(src SOURCES)
add_library(MusicManagingSystem SHARED ${SOURCES} MusicManagingSystem.def)
target_include_directories(MusicManagingSystem PUBLIC include)
target_include_directories(MusicManagingSystem PUBLIC ${SQLite3_INCLUDE_DIRS})
target_link_libraries(MusicManagingSystem PRIVATE ${SQLite3_LIBRARIES})
