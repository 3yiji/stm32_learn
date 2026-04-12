set(LVGL_Include_Dirs
    ${CMAKE_CURRENT_SOURCE_DIR}         # FreeRTOSConfig.h所在目录
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/porting  

    ${CMAKE_CURRENT_SOURCE_DIR}/demos
)

file(GLOB_RECURSE LVGL_port ${CMAKE_CURRENT_SOURCE_DIR}/examples/porting/*.c)
file(GLOB_RECURSE LVGL_Sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# set(LVGL_demos 
#     ${CMAKE_CURRENT_SOURCE_DIR}/demos/lv_demos.c
#     ${CMAKE_CURRENT_SOURCE_DIR}/demos/stress/lv_demo_stress.c
# )
file(GLOB_RECURSE LVGL_Demos ${CMAKE_CURRENT_SOURCE_DIR}/demos/*.c)

add_library(LVGL STATIC)

target_include_directories(LVGL PUBLIC 
    ${LVGL_Include_Dirs}
)
target_sources(LVGL PRIVATE 
    ${LVGL_port} 
    ${LVGL_Sources}
    ${LVGL_Demos}
)
target_link_libraries(LVGL PUBLIC Hardware)
