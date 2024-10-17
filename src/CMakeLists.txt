# Set the binary name based on the project name
set(BINARY ${CMAKE_PROJECT_NAME})

# Gather all source files
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

# Add the executable and library
add_executable(${BINARY} ${SOURCES})
add_library(${BINARY}_lib STATIC ${SOURCES})

# Find necessary packages
find_package(Boost 1.71.0 COMPONENTS log log_setup REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(OpenCV REQUIRED)

# Link libraries and include directories
set(LIBRARIES ${OPENSSL_LIBRARIES} ${OpenCV_LIBRARIES} ${Boost_LIBRARIES} jsoncpp curl)
set(INCLUDE_DIRS ${OPENSSL_INCLUDE_DIR} ${OpenCV_INCLUDE_DIR})

target_link_libraries(${BINARY} ${LIBRARIES})
target_include_directories(${BINARY} PUBLIC ${INCLUDE_DIRS})

target_link_libraries(${BINARY}_lib ${LIBRARIES})
target_include_directories(${BINARY}_lib PUBLIC ${INCLUDE_DIRS})

# Add boost global placeholder definition
add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)

# Set the Rust library path
set(RUST_LIB_DIR ${CMAKE_SOURCE_DIR}/libs/Rust/rust_lib/target/release)
set(TERMINAL_DIR ${CMAKE_SOURCE_DIR}/libs/Rust/terminal/target/release)

# Build the Rust libraries using Cargo
add_custom_target(build_rust_lib
    COMMAND cargo build --release --manifest-path=${CMAKE_SOURCE_DIR}/libs/Rust/rust_lib/Cargo.toml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/Rust/rust_lib
)

add_custom_target(build_terminal
    COMMAND cargo build --release --manifest-path=${CMAKE_SOURCE_DIR}/libs/Rust/terminal/Cargo.toml
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/Rust/terminal
)

# Include the Rust libraries
add_library(rust_lib SHARED IMPORTED)
set_target_properties(rust_lib PROPERTIES IMPORTED_LOCATION ${RUST_LIB_DIR}/librust_lib.so)

add_library(terminal SHARED IMPORTED)
set_target_properties(terminal PROPERTIES IMPORTED_LOCATION ${TERMINAL_DIR}/libterminal.so)

# Link the Rust libraries to the executable
add_dependencies(${BINARY} build_rust_lib build_terminal)
target_link_libraries(${BINARY} rust_lib terminal)

install(TARGETS ${BINARY} DESTINATION bin)
