cmake_minimum_required(VERSION 3.16)
project(SamkoonMonitorQt5 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets WebEngineWidgets)

add_executable(SamkoonMonitorQt5
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

target_link_libraries(SamkoonMonitorQt5
    PRIVATE
    Qt5::Core
    Qt5::Widgets
    Qt5::WebEngineWidgets
)

set_target_properties(SamkoonMonitorQt5 PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# CI 注入版本号，显示在窗口标题，便于确认实际运行的构建版本
set(APP_VERSION "" CACHE STRING "Version string shown in window title")
if(APP_VERSION)
    target_compile_definitions(SamkoonMonitorQt5 PRIVATE APP_VERSION="${APP_VERSION}")
endif()

# 本地调试模式：cmake -DUSE_LOCAL_SDK=ON .. && cmake --build .
option(USE_LOCAL_SDK "Load SDK from local server (http://localhost:8888)" OFF)
if(USE_LOCAL_SDK)
    target_compile_definitions(SamkoonMonitorQt5 PRIVATE USE_LOCAL_SDK)
    message(STATUS "SDK source: http://localhost:8888/sdk/sk-monitor/latest/sdk-embed.html")
else()
    message(STATUS "SDK source: CDN (xdata.samkoonyun.com)")
endif()
