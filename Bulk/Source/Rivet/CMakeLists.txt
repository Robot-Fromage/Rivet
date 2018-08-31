cmake_minimum_required( VERSION 3.13.2 )

file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "Rivet/Include/*"
    "Rivet/Source/*"
)

add_library( Rivet ${source_list} )

set_target_properties( Rivet PROPERTIES CMAKE_AUTOMOC ON )
target_compile_definitions( Rivet PUBLIC UNICODE )
target_compile_definitions( Rivet PRIVATE RIVET_EXT_VERSION_MAJOR=${RIVET_VERSION_MAJOR} )
target_compile_definitions( Rivet PRIVATE RIVET_EXT_VERSION_MINOR=${RIVET_VERSION_MINOR} )
set_target_properties( Rivet PROPERTIES OUTPUT_NAME "Rivet${RIVET_VERSION_MAJOR}.${RIVET_VERSION_MINOR}" )

find_package(
    Qt5
    REQUIRED COMPONENTS
    Core
    Widgets
    Gui
    WinExtras
)

target_link_libraries(
    Rivet
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::WinExtras
)

target_include_directories(
    Rivet
    PUBLIC
    "Rivet/Include/"
    PRIVATE
    "Rivet/Source/Rivet/"
)

# For MSVC parse pretty filter names
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
