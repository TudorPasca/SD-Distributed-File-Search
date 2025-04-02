add_executable(worker
        worker.cpp
        source/Controller/SearchController.cpp
        include/Controller/SearchController.h
        source/DTO/FileDTO.cpp
        include/DTO/FileDTO.h
        source/FileScraper/FileScraper.cpp
        include/FileScraper/FileScraper.h
        source/FileScraper/RegexFileIgnorer.cpp
        include/FileScraper/RegexFileIgnorer.h
        include/FileScraper/IFileScraper.h
        include/FileScraper/IFileIgnorer.h
)

target_compile_features(worker PRIVATE cxx_std_17)

find_package(Crow CONFIG REQUIRED)
target_link_libraries(worker PRIVATE Crow::Crow asio::asio)

set_target_properties(worker PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)