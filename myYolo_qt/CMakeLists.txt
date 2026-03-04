cmake_minimum_required(VERSION 3.16)

project(myYolo VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(UTIL
    src/util/mysetting.h
    src/util/mysetting.cpp
    src/util/mylog.h
    src/util/mylog.cpp
    src/util/mydir.h
    src/util/mydir.cpp
    src/util/myprocess.h
    src/util/myprocess.cpp
    src/util/processthread.h
    src/util/processthread.cpp
)

set(WIDGET_MAINWINDOW
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
)

set(WIDGET_LOG
    src/WidgetLog/widgetlog.h
    src/WidgetLog/widgetlog.cpp
    src/WidgetLog/widgetlog.ui
)

set(WIDGET_FILTER
    src/WidgetFilter/widgetfilter.h
    src/WidgetFilter/widgetfilter.cpp
    src/WidgetFilter/widgetfilter.ui
)

set(WIDGET_LABEL
    src/WidgetLabel/widgetlabel.h
    src/WidgetLabel/widgetlabel.cpp
    src/WidgetLabel/widgetlabel.ui
)

set(WIDGET_MAIN
    src/WidgetMain/Info/widgetmaininfo.h
    src/WidgetMain/Info/widgetmaininfo.cpp
    src/WidgetMain/Info/widgetmaininfo.ui
    src/WidgetMain/Setting/widgetmainsetting.h
    src/WidgetMain/Setting/widgetmainsetting.cpp
    src/WidgetMain/Setting/widgetmainsetting.ui
)

set(WIDGET_ADD_DATA
    src/WidgetAddData/widgetadddata.h
    src/WidgetAddData/widgetadddata.cpp
    src/WidgetAddData/widgetadddata.ui
)

set(WIDGET_TRAIN
    src/WidgetTrain/widgettrain.h
    src/WidgetTrain/widgettrain.cpp
    src/WidgetTrain/widgettrain.ui
)

set(WIDGET_EXPORT
    src/WidgetExport/widgetexport.h
    src/WidgetExport/widgetexport.cpp
    src/WidgetExport/widgetexport.ui
)

set(WIDGET_TEST
    src/WidgetTest/widgettest.h
    src/WidgetTest/widgettest.cpp
    src/WidgetTest/widgettest.ui
    src/WidgetTest/check_cv_onnx.h
    src/WidgetTest/check_cv_onnx.cpp
    src/WidgetTest/check_onnxruntime.h
    src/WidgetTest/check_onnxruntime.cpp
)

set(INCLUDE
    src/include/global.h
    src/include/keydef.h
    src/include/datadef.h
    src/include/funcdef.h
)

set(COMMON
    src/common/confighandler.h
)

add_executable(myYolo
    ${INCLUDE}
    ${COMMON}
    ${UTIL}
    ${WIDGET_MAINWINDOW}
    ${WIDGET_LOG}
    ${WIDGET_FILTER}
    ${WIDGET_LABEL}
    ${WIDGET_MAIN}
    ${WIDGET_ADD_DATA}
    ${WIDGET_TRAIN}
    ${WIDGET_EXPORT}
    ${WIDGET_TEST}
    myYolo.qrc
)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets LinguistTools)
target_link_libraries(myYolo PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets)

set(TRANSLATIONS
    res/i18n/en.ts
    res/i18n/zh_simple.ts
    res/i18n/zh_traditional.ts
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

target_link_libraries(myYolo PRIVATE spdlog::spdlog)

if(WIN32)
    set(OpenCV_DIR "${PROJECT_SOURCE_DIR}/vendor/opencv/build/x64/vc16/lib")
    find_package(OpenCV REQUIRED)
    target_link_libraries(myYolo PRIVATE ${OpenCV_LIBS})
    target_include_directories(myYolo PRIVATE ${OpenCV_INCLUDE_DIRS})
elseif(LINUX)
    find_package(OpenCV REQUIRED)
    target_link_libraries(myYolo PRIVATE ${OpenCV_LIBS})
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.h
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(myYolo PUBLIC
    "${PROJECT_SOURCE_DIR}/src/include"
    "${PROJECT_SOURCE_DIR}/src/util"
)

get_target_property(INC myYolo INCLUDE_DIRECTORIES)
message(STATUS "myYolo include dirs: ${INC}")
