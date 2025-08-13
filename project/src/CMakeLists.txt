cmake_minimum_required(VERSION 3.10)

get_filename_component(CURRENT_DIR_PATH "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE) 
get_filename_component(CURRENT_DIR_NAME "${CURRENT_DIR_PATH}" NAME) 
project(${CURRENT_DIR_NAME})

set(CMAKE_CXX_STANDARD 11) 

if(MSVC)
	add_compile_options(/Zc:__cplusplus)
endif()

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets Gui Svg REQUIRED)

set(UI_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
)

set(RCC_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/image.qrc
)

set(HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
	${CMAKE_CURRENT_SOURCE_DIR}/kglobaldata.h
	${CMAKE_CURRENT_SOURCE_DIR}/ktoolbar.h
	${CMAKE_CURRENT_SOURCE_DIR}/ktoolbutton.h
	${CMAKE_CURRENT_SOURCE_DIR}/kcanvas.h
	${CMAKE_CURRENT_SOURCE_DIR}/kshape.h
	${CMAKE_CURRENT_SOURCE_DIR}/kshapefactory.h
	${CMAKE_CURRENT_SOURCE_DIR}/krect.h
	${CMAKE_CURRENT_SOURCE_DIR}/kcircle.h
	${CMAKE_CURRENT_SOURCE_DIR}/kpen.h
	${CMAKE_CURRENT_SOURCE_DIR}/kline.h
	${CMAKE_CURRENT_SOURCE_DIR}/ktext.h
	${CMAKE_CURRENT_SOURCE_DIR}/ktopmenu.h
	${CMAKE_CURRENT_SOURCE_DIR}/ktab.h
	${CMAKE_CURRENT_SOURCE_DIR}/ktabitem.h
	${CMAKE_CURRENT_SOURCE_DIR}/filemanager.h
	${CMAKE_CURRENT_SOURCE_DIR}/kstylepanel.h
	${CMAKE_CURRENT_SOURCE_DIR}/kpropertybox.h
	${CMAKE_CURRENT_SOURCE_DIR}/linestyledelegate.h
	${CMAKE_CURRENT_SOURCE_DIR}/linestylecombox.h
	${CMAKE_CURRENT_SOURCE_DIR}/kpolygonpanel.h
	${CMAKE_CURRENT_SOURCE_DIR}/kpolygon.h
)
set(CPP_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/kglobaldata.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ktoolbar.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ktoolbutton.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/kcanvas.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/kshape.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/kshapefactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/krect.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/kcircle.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/kpen.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/kline.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ktext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ktopmenu.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ktab.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ktabitem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/filemanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/kstylepanel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/kpropertybox.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/kpolygonpanel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/kpolygon.cpp
)

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

source_group("Form Files" FILES ${UI_FILES})
source_group("Resource Files" FILES ${RCC_FILES})

add_executable(${PROJECT_NAME} WIN32 ${HEADER_FILES} ${CPP_FILES} ${UI_FILES} ${RCC_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt5::Widgets
	Qt5::Core
	Qt5::Gui
	Qt5::Svg
)