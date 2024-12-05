cmake_minimum_required(VERSION 3.16)

project(myYolo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
        myYolo_qt/main.cpp
        myYolo_qt/mainwindow.cpp
        myYolo_qt/mainwindow.h
        myYolo_qt/mainwindow.ui
)

add_executable(myYolo
    ${PROJECT_SOURCES}
    myYolo_qt/include/global.h
    myYolo_qt/include/keydef.h
    myYolo_qt/include/datadef.h
    myYolo_qt/include/funcdef.h
    myYolo_qt/common/confighandler.h
    myYolo_qt/util/mysetting.h
    myYolo_qt/util/mysetting.cpp
    myYolo_qt/util/mylog.h
    myYolo_qt/util/mylog.cpp
    myYolo_qt/util/mydir.h
    myYolo_qt/util/mydir.cpp
    myYolo_qt/util/myprocess.h
    myYolo_qt/util/myprocess.cpp
    myYolo_qt/util/processthread.h
    myYolo_qt/util/processthread.cpp
    myYolo_qt/WidgetLog/widgetlog.h
    myYolo_qt/WidgetLog/widgetlog.cpp
    myYolo_qt/WidgetLog/widgetlog.ui
    myYolo_qt/WidgetMain/Info/widgetmaininfo.h
    myYolo_qt/WidgetMain/Info/widgetmaininfo.cpp
    myYolo_qt/WidgetMain/Info/widgetmaininfo.ui
    myYolo_qt/WidgetMain/Setting/widgetmainsetting.h
    myYolo_qt/WidgetMain/Setting/widgetmainsetting.cpp
    myYolo_qt/WidgetMain/Setting/widgetmainsetting.ui
    myYolo_qt/WidgetFilter/widgetfilter.h
    myYolo_qt/WidgetFilter/widgetfilter.cpp
    myYolo_qt/WidgetFilter/widgetfilter.ui
    myYolo_qt/WidgetLabel/widgetlabel.h
    myYolo_qt/WidgetLabel/widgetlabel.cpp
    myYolo_qt/WidgetLabel/widgetlabel.ui
    myYolo_qt/WidgetAddData/widgetadddata.h
    myYolo_qt/WidgetAddData/widgetadddata.cpp
    myYolo_qt/WidgetAddData/widgetadddata.ui
    myYolo_qt/WidgetTrain/widgettrain.h
    myYolo_qt/WidgetTrain/widgettrain.cpp
    myYolo_qt/WidgetTrain/widgettrain.ui
    myYolo_qt/WidgetExport/widgetexport.h
    myYolo_qt/WidgetExport/widgetexport.cpp
    myYolo_qt/WidgetExport/widgetexport.ui
    myYolo_qt/WidgetTest/widgettest.h
    myYolo_qt/WidgetTest/widgettest.cpp
    myYolo_qt/WidgetTest/widgettest.ui
    myYolo_qt/WidgetTest/onnxcheck.h
    myYolo_qt/WidgetTest/onnxcheck.cpp
    myYolo_qt/myYolo.qrc
)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets LinguistTools)
target_link_libraries(myYolo PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)

set(TRANSLATIONS
    myYolo_qt/res/i18n/en.ts
    myYolo_qt/res/i18n/zh_simple.ts
    myYolo_qt/res/i18n/zh_traditional.ts
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
