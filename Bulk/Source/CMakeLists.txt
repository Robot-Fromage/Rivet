cmake_minimum_required(VERSION 3.11.0)

# Set Cache variables
set( RIVET_EXPLICIT_HOST_ID         "unset"     CACHE STRING    "Explicitly set host ID from command line" )
set( RIVET_EXPLICIT_COMPILER_ID     "unset"     CACHE STRING    "Explicitly set compiler ID from command line" )
set( RIVET_USE_CONFIG               OFF         CACHE BOOL      "Scpecifies wether to use config files for overriding cache variables" )
set( RIVET_BUILD_SHARED             OFF         CACHE BOOL      "Sets if we build the library shared or static" )
set( RIVET_BUILD_TESTS              OFF         CACHE BOOL      "Sets if we build the test programs" )
set( RIVET_QT_CMAKE_PATH            "unset"     CACHE STRING    "Indicates the path to Qt cmake package" )
SET( RIVET_VERSION_MAJOR            "unset"     CACHE STRING    "Indicate RIVET version major" )
SET( RIVET_VERSION_MINOR            "unset"     CACHE STRING    "Indicate RIVET version minor" )

#--------------------------------------------
# Display host & compiler
message( "Generating: ${RIVET_EXPLICIT_HOST_ID} ${RIVET_EXPLICIT_COMPILER_ID}" )

# Generate only Debug and Release configurations for all projects
set( CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING INTERNAL FORCE )

# Generate only x64 configurations for all projects
#set( CMAKE_GENERATOR_PLATFORM x64 )

# Get rid of cmake's ZERO_CHECK for solution
set( CMAKE_SUPPRESS_REGENERATION true )

# Set Debug postfix for all projects
set( CMAKE_DEBUG_POSTFIX d )

# Set Runtime Library for all projects
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

# Set C++ standard options
set( CMAKE_CXX_FLAGS "-std=c++14" )
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

#--------------------------------------------
# Create global solution
project( RIVET CXX )

#--------------------------------------------
# Flags for Compiler / Platforms
# Flags for CLANG
if( "${RIVET_EXPLICIT_COMPILER_ID}" STREQUAL "CLANG" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1" )

# Flags for GCC
elseif( "${RIVET_EXPLICIT_COMPILER_ID}" STREQUAL "GCC" )
    # Specific flags for GNU GCC
    if( "${RIVET_EXPLICIT_HOST_ID}" STREQUAL "GNU" )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1 -W" )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread" )
    # Specific flags for MinGW GCC
    elseif( "${RIVET_EXPLICIT_HOST_ID}" STREQUAL "WIN" )
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1 -Wa,-mbig-obj" )
    endif()

# Flags for MSVC
elseif( "${RIVET_EXPLICIT_COMPILER_ID}" STREQUAL "MSVC" )
    set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Zi -Gm- -Od -Ob0 -RTC1 -MDd" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -bigobj -EHsc" )
endif()

#--------------------------------------------
# Read Preferences if needed
if( ${RIVET_USE_CONFIG} )
    #--------------------------------------------
    # Read Default Preferences
    include( ${CMAKE_CURRENT_SOURCE_DIR}/../Tools/DefaultConfig.cmake )

    #--------------------------------------------
    # IF USING CLANG ON MAC WITH XCODE
    if( ${CMAKE_GENERATOR} STREQUAL "Xcode" )
        if( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../Xcode_Config.cmake )
                include( ${CMAKE_CURRENT_SOURCE_DIR}/../../Xcode_Config.cmake )
        endif()
    endif()

    #--------------------------------------------
    # IF USING CLANG ON WINDOWS WITH VISUALSTUDIO
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        # using Clang
        if( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 15 2017 Win64" )
            if( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../VisualStudio_Solution_CLANG_LLVM_Config.cmake )
                include( ${CMAKE_CURRENT_SOURCE_DIR}/../../VisualStudio_Solution_CLANG_LLVM_Config.cmake )
            endif()
        endif()

    #--------------------------------------------
    # IF USING GCC
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # Specific flags for GNU GCC
        if( "${RIVET_EXPLICIT_HOST_ID}" STREQUAL "GNU" )
            if( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../SublimeText_Project_GNU_GCC_Config.cmake )
                include( ${CMAKE_CURRENT_SOURCE_DIR}/../../SublimeText_Project_GNU_GCC_Config.cmake )
            endif()
        # Specific flags for MinGW GCC
        elseif( "${RIVET_EXPLICIT_HOST_ID}" STREQUAL "WIN" )
            if( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../MinGW_Makefile_GCC_Config.cmake )
                include( ${CMAKE_CURRENT_SOURCE_DIR}/../../MinGW_Makefile_GCC_Config.cmake )
            endif()
        endif()

    #--------------------------------------------
    # IF USING MSVC
    elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
        if( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../VisualStudio_Solution_MSVC_Config.cmake )
            include( ${CMAKE_CURRENT_SOURCE_DIR}/../../VisualStudio_Solution_MSVC_Config.cmake )
        endif()
    endif()
endif()

#--------------------------------------------
# Qt Options
if( NOT "${RIVET_QT_CMAKE_PATH}" STREQUAL "default" )
    set( CMAKE_PREFIX_PATH ${RIVET_QT_CMAKE_PATH} )
    set( CMAKE_AUTOMOC ON )
endif()

#--------------------------------------------
# Read Global Preferences
include(${CMAKE_CURRENT_SOURCE_DIR}/../Tools/GlobalConfig.cmake)
message( "${RIVET_EXT_VERSION_MAJOR}.${RIVET_EXT_VERSION_MINOR}" )

#--------------------------------------------
# Add projects
include(${CMAKE_CURRENT_SOURCE_DIR}/Rivet/CMakeLists.txt) # Depends on Qt

if( ${RIVET_BUILD_TESTS} )
    include(${CMAKE_CURRENT_SOURCE_DIR}/BlurBehind/CMakeLists.txt)      # Depends on Rivet
    include(${CMAKE_CURRENT_SOURCE_DIR}/CustomCaption/CMakeLists.txt)   # Depends on Rivet
    include(${CMAKE_CURRENT_SOURCE_DIR}/TabDemo/CMakeLists.txt)         # Depends on Rivet
endif()

#--------------------------------------------
# Set default startup project
set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Rivet )

