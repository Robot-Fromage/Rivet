#!/bin/sh

set -e
cd "`dirname "$0"`"

if [ ! -f Xcode_Config.cmake ]; then
     cp "Bulk/Tools/DefaultConfig.cmake" "Xcode_Config.cmake"
fi

if [ ! -f Ubuntu_Config.cmake ]; then
     cp "Bulk/Tools/DefaultConfig.cmake" "SublimeText_Project_GNU_GCC_Config.cmake"
fi