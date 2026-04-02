set(LVGL_Include_Dirs
    ${CMAKE_CURRENT_SOURCE_DIR}         # FreeRTOSConfig.h所在目录
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/porting  
)

file(GLOB_RECURSE LVGL_port ${CMAKE_CURRENT_SOURCE_DIR}/examples/porting/*.c)
file(GLOB_RECURSE LVGL_Sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_library(LVGL STATIC)

target_include_directories(LVGL PUBLIC 
    ${LVGL_Include_Dirs}
)
target_sources(LVGL PRIVATE 
    ${LVGL_port} 
    ${LVGL_Sources}
)
target_link_libraries(LVGL PUBLIC Hardware)
