cmake_minimum_required(VERSION 3.16)
project(RutasInspeccion LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Toma automaticamente todos los .cpp y .h dentro de src.
# Asi nadie tiene que editar este archivo al agregar clases nuevas.
file(GLOB_RECURSE FUENTES CONFIGURE_DEPENDS src/*.cpp src/*.h)

qt_add_executable(RutasInspeccion ${FUENTES})
target_include_directories(RutasInspeccion PRIVATE src)
target_link_libraries(RutasInspeccion PRIVATE Qt6::Widgets)

# Copia la carpeta datos junto al ejecutable para tener los CSV a mano
add_custom_command(TARGET RutasInspeccion POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/datos $<TARGET_FILE_DIR:RutasInspeccion>/datos)
