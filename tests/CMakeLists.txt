link_libraries(MMSUtils)

add_executable(UtilsTest src/test_utils.c)
add_test(NAME TestUtils COMMAND UtilsTest)
