find_package(SQLite3 REQUIRED)

aux_source_directory(src SOURCES)
add_library(MusicManagingSystem SHARED ${SOURCES})
target_include_directories(MusicManagingSystem PUBLIC include)
target_include_directories(MusicManagingSystem PRIVATE ${SQLite3_INCLUDE_DIRS})
target_link_libraries(MusicManagingSystem PRIVATE ${SQLite3_LIBRARIES})

set(MUSIC_MANAGING_SYSTEM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
