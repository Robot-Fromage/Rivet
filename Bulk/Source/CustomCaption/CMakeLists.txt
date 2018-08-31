cmake_minimum_required( VERSION 3.13.2 )

file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "CustomCaption/Include/*"
    "CustomCaption/Source/*"
)

add_executable( CustomCaption ${source_list} )

target_link_libraries( CustomCaption Rivet )

set_target_properties( CustomCaption PROPERTIES CMAKE_AUTOMOC ON )

target_include_directories(
    CustomCaption
    PUBLIC
    "CustomCaption/Include/"
    PRIVATE
    "CustomCaption/Source/"
)

# For MSVC parse pretty filter names
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
