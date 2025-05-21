cmake_minimum_required(VERSION 3.5)

project(mpp_solver_tests)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Задаем настройки для загрузки GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1  # Укажите нужную версию GoogleTest
)

FetchContent_MakeAvailable(googletest)

set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")
file(MAKE_DIRECTORY ${BIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

enable_testing()

set(TESTS
    engineTests.cpp
    #converterTests.cpp
    #plotterTests.cpp
)

add_executable(${PROJECT_NAME} main.cpp ${TESTS})
target_link_libraries(${PROJECT_NAME} GTest::gtest GTest::gmock mpp_lib)
target_include_directories(${PROJECT_NAME} PRIVATE
    "../include"
)
