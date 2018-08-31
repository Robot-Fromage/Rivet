cmake_minimum_required( VERSION 3.13.2 )

file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "BlurBehind/Include/*"
    "BlurBehind/Source/*"
)

add_executable( BlurBehind ${source_list} )

target_link_libraries( BlurBehind Rivet )

set_target_properties( BlurBehind PROPERTIES CMAKE_AUTOMOC ON )

target_include_directories(
    BlurBehind
    PUBLIC
    "BlurBehind/Include/"
    PRIVATE
    "BlurBehind/Source/"
)

# For MSVC parse pretty filter names
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
