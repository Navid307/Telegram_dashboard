# Set the binary name for the tests
set(BINARY ${CMAKE_PROJECT_NAME}_tests)

# Gather all test source files
file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.hpp *.cpp)

# Add GoogleTest
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../libs/googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest)
include(GoogleTest)
enable_testing()

# Add testing definition
add_definitions(-DTESTING)

# Find necessary packages
find_package(Boost 1.71.0 COMPONENTS log log_setup filesystem system REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(OpenCV REQUIRED)

# Add boost global placeholder definition
add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)

# Create the test executable
add_executable(${BINARY} ${TEST_SOURCES})
add_test(NAME ${BINARY} COMMAND ${BINARY})

# Set the Rust library paths
set(RUST_LIB_DIR ${CMAKE_SOURCE_DIR}/libs/Rust/rust_lib/target/release)
set(TERMINAL_DIR ${CMAKE_SOURCE_DIR}/libs/Rust/terminal/target/release)

# Include the Rust libraries
add_library(rust_lib SHARED IMPORTED)
set_target_properties(rust_lib PROPERTIES IMPORTED_LOCATION ${RUST_LIB_DIR}/librust_lib.so)

add_library(terminal SHARED IMPORTED)
set_target_properties(terminal PROPERTIES IMPORTED_LOCATION ${TERMINAL_DIR}/libterminal.so)

# Add dependencies on Rust libraries
add_dependencies(${BINARY} build_rust_lib build_terminal)

# Link libraries
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest ${Boost_LIBRARIES} rust_lib terminal jsoncpp curl)
