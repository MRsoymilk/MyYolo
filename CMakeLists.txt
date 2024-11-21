cmake_minimum_required(VERSION 3.16)

project(myYolo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
)

add_executable(myYolo
    ${PROJECT_SOURCES}
    src/include/global.h
    src/include/keydef.h
    src/include/datadef.h
    src/include/funcdef.h
    src/common/confighandler.h
    src/util/mysetting.h
    src/util/mysetting.cpp
    src/util/mylog.h
    src/util/mylog.cpp
    src/util/mydir.h
    src/util/mydir.cpp
    src/WidgetMain/Info/widgetmaininfo.h
    src/WidgetMain/Info/widgetmaininfo.cpp
    src/WidgetMain/Info/widgetmaininfo.ui
    src/WidgetMain/Setting/widgetmainsetting.h
    src/WidgetMain/Setting/widgetmainsetting.cpp
    src/WidgetMain/Setting/widgetmainsetting.ui
    src/WidgetFilter/widgetfilter.h
    src/WidgetFilter/widgetfilter.cpp
    src/WidgetFilter/widgetfilter.ui
    src/WidgetLabel/widgetlabel.h
    src/WidgetLabel/widgetlabel.cpp
    src/WidgetLabel/widgetlabel.ui
    src/WidgetAddData/widgetadddata.h
    src/WidgetAddData/widgetadddata.cpp
    src/WidgetAddData/widgetadddata.ui
    src/WidgetTrain/widgettrain.h
    src/WidgetTrain/widgettrain.cpp
    src/WidgetTrain/widgettrain.ui
    src/WidgetExport/widgetexport.h
    src/WidgetExport/widgetexport.cpp
    src/WidgetExport/widgetexport.ui
    src/WidgetTest/widgettest.h
    src/WidgetTest/widgettest.cpp
    src/WidgetTest/widgettest.ui
    src/WidgetTest/onnxcheck.h
    src/WidgetTest/onnxcheck.cpp
    src/myYolo.qrc
)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets LinguistTools)
target_link_libraries(myYolo PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)

set(TRANSLATIONS
    src/res/i18n/en.ts
    src/res/i18n/zh_simple.ts
    src/res/i18n/zh_traditional.ts
)

qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES)

qt_add_translations(myYolo
               TS_FILES  ${TRANSLATIONS})

install(FILES ${qm_files} DESTINATION "translations")

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.soymilk.myYolo)
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

if(WIN32)
    set(OpenCV_DIR "vendor/opencv/build/x64/vc16/lib")
    find_package(OpenCV REQUIRED)
    target_link_libraries(myYolo PRIVATE ${OpenCV_LIBS})
    target_include_directories(myYolo PRIVATE ${OpenCV_INCLUDE_DIRS})
elseif(LINUX)
    find_package(OpenCV REQUIRED)
    target_link_libraries(myYolo PRIVATE ${OpenCV_LIBS})
endif()
