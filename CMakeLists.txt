cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

project(mpp_solver VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -static-libstdc++ -static-libgcc")

set(BIN_DIR "${CMAKE_BINARY_DIR}/bin")
file(MAKE_DIRECTORY ${BIN_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

link_directories(
    "3rd_party/ginac/lib"
    "3rd_party/cln/lib"
)

SET(GUI_SOURCES
    src/gui/systemModel.cpp
)

Set(MPP_SOURCES
    src/mpp/systemModel.cpp
    src/mpp/engine.cpp
)

Set(HELPERS_SOURCES
    src/helpers/converter.cpp
    src/helpers/plotter.cpp
)

add_library(mpp_lib STATIC
    ${HELPERS_SOURCES}
    ${MPP_SOURCES}
    ${GUI_SOURCES}
    src/symbols.cpp
)

target_include_directories(mpp_lib PUBLIC
    "include"
    "3rd_party/ginac/include"
    "3rd_party/cln/include"
    "3rd_party/gnuplot-cpp/include"
)

target_link_libraries(mpp_lib PUBLIC
    ginac
    cln
    Qt${QT_VERSION_MAJOR}::Widgets
)

set(PROJECT_SOURCES
    src/main.cpp
    src/gui/mainwindow.cpp
    src/gui/mainwindow.ui
    include/gui/mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mpp_solver
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        include/gui/mainwindow.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET mpp_solver APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_BINARY_DIR}/../qt_test_autogen/include"
    "include"
    "include/gui"
    "3rd_party/ginac/include"
    "3rd_party/cln/include"
)

target_link_libraries(mpp_solver PRIVATE 
    mpp_lib
    Qt${QT_VERSION_MAJOR}::Widgets
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.mpp_solver)
endif()
set_target_properties(mpp_solver PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS mpp_solver
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(mpp_solver)
endif()

# add runtime deps to bin directory
set(RUNTIME_DEPS "${CMAKE_SOURCE_DIR}/bin")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${RUNTIME_DEPS} ${BIN_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:${PROJECT_NAME}> ${BIN_DIR}
    COMMENT "Copying files and executable to ${BIN_DIR}"
)

#tests
add_subdirectory(tests)

# use windeployqt to prepare bundle
