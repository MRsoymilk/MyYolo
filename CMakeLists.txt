cmake_minimum_required(VERSION 3.16)

project(myYolo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(myYolo
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        WidgetTest/widgettest.h WidgetTest/widgettest.cpp WidgetTest/widgettest.ui
        WidgetAddData/widgetadddata.h WidgetAddData/widgetadddata.cpp WidgetAddData/widgetadddata.ui
        WidgetTrain/widgettrain.h WidgetTrain/widgettrain.cpp WidgetTrain/widgettrain.ui
        util/mysetting.h util/mysetting.cpp
        util/mylog.h util/mylog.cpp
        include/keydef.h
        util/mydir.h util/mydir.cpp
        WidgetExport/widgetexport.h WidgetExport/widgetexport.cpp WidgetExport/widgetexport.ui
        WidgetTest/onnxcheck.h WidgetTest/onnxcheck.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET myYolo APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(myYolo SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(myYolo
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(myYolo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.myYolo)
endif()
set_target_properties(myYolo PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS myYolo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(myYolo)
endif()

add_subdirectory(vendor/spdlog-1.15.0/)
target_link_libraries(myYolo PRIVATE spdlog::spdlog)

set(OpenCV_DIR "D:/opencv409/build/x64/vc16/lib")
find_package(OpenCV REQUIRED)
target_link_libraries(myYolo PRIVATE ${OpenCV_LIBS})
target_include_directories(myYolo PRIVATE ${OpenCV_INCLUDE_DIRS})
