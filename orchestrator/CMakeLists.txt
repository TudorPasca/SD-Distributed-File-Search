add_executable(orchestrator main.cpp)

add_dependencies(orchestrator worker)

set_target_properties(orchestrator worker PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_custom_command(TARGET orchestrator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:worker>
        $<TARGET_FILE_DIR:orchestrator>
)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost REQUIRED COMPONENTS process)
target_link_libraries(orchestrator PRIVATE Boost::process)

find_package(Crow CONFIG REQUIRED)
target_link_libraries(orchestrator PRIVATE Crow::Crow asio::asio)

target_compile_features(orchestrator PRIVATE cxx_std_17)