cmake_minimum_required(VERSION 3.22.1)

project("AndroidNative")

add_library(
    native
    SHARED
    native-lib.cpp
)

add_library(AndroidNative SHARED IMPORTED)
set_target_properties(AndroidNative PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../jnilibs/${ANDROID_ABI}/libAndroidNative.so
    IMPORTED_NO_SONAME TRUE
)
target_include_directories(AndroidNative
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../../native/cpp/include
)
target_include_directories(native PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../../.xmake/deps/spdlog/include)

find_library(log-lib log)
target_link_libraries(
    native
    ${log-lib} AndroidNative
)
