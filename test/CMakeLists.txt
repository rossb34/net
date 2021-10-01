
add_executable(stream_socket_tests src/socket_tests.cpp)
target_link_libraries(stream_socket_tests ${LIB_NAME} ${CONAN_LIBS})
add_test(NAME run_stream_socket_tests COMMAND stream_socket_tests)

add_executable(address_resolver_tests src/address_resolver_tests.cpp)
target_link_libraries(address_resolver_tests ${LIB_NAME} ${CONAN_LIBS})
add_test(NAME run_address_resolver_tests COMMAND address_resolver_tests)
