# =============[ Main App ]=============
add_executable( ${APP_NAME}
    cameras/look_at.cpp
    cameras/orthographic.cpp
    cameras/perspective.cpp
    core/app.cpp
    core/background.cpp
    core/builder.cpp
    core/converters.cpp
    core/film.cpp
    core/image_io.cpp
    core/main.cpp
    core/ray.cpp
    core/scene.cpp
    ../ext/tiny_obj_loader.cc
    integrators/raycast_integrator.cpp
    integrators/blinnphong_integrator.cpp
    light/ambient_light.cpp
    light/directional_light.cpp
    light/point_light.cpp
    light/spot_light.cpp
    light/visibility_tester.cpp
    math/geometry.cpp
    msg_system/error.cpp
    primitive/box.cpp
    primitive/material.cpp
    primitive/plane.cpp
    primitive/primitive.cpp  
    primitive/sphere.cpp
    primitive/surfel.cpp
    primitive/triangle.cpp
   )
# ------------------------------------------------------------
include_directories(external/glm)


# =============[ Compile for release/debug ]=================
target_compile_features( ${APP_NAME}  PUBLIC cxx_std_17 )
# target_compile_options( ${APP_NAME} PRIVATE -Wall -pedantic -Wgnu-anonymous-struct -Wnested-anon-types -g)
# target_compile_options( ${APP_NAME} PRIVATE -Wall -pedantic -g)
target_compile_options( ${APP_NAME} PRIVATE -Wall -pedantic -O3)
# Use this to define the symbol DEBUG
# target_compile_definitions(${APP_NAME} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

set(DEBUG_LEVEL 0 CACHE STRING "Debug level")
target_compile_definitions(${APP_NAME} PUBLIC "DEBUG_LEVEL=${DEBUG_LEVEL}")

# ------------------------------------------------------------

# =============[ Include folders ]=================
# XML parser
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib_tinyxml2)
# Image I/O lib
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib_lodepng)
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib_stb_image)
# Math library
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib_cybase)
# Camera folder
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/cameras)
# Core folder
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/core)
# Ext folder
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/../ext)
# Integrator folder
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/integrators)
# Light folder
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/light)
# math system foldedr
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/math)
# msg system foldedr
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/msg_system)
# primitive foldedr
target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/primitive)
# # External headers
# target_include_directories( ${APP_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/ext)
# ------------------------------------------------------------

# =============[ Libs ]=================
# find_package(glm CONFIG REQUIRED)
# target_link_libraries(${APP_NAME} PRIVATE glm::glm)
target_link_libraries( ${APP_NAME} PRIVATE ${LIB_TINYXML2})
target_link_libraries( ${APP_NAME} PRIVATE ${LIB_LODEPNG})
# ------------------------------------------------------------

# =============[ App properties ]=================
# Define where the binary will be output to.
set_target_properties(${APP_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
# ------------------------------------------------------------
