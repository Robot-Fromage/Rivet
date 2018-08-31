set -e
cd "`dirname "$0"`"

## CD 3 steps up to root/Bulk
cd ../../../

## Make Generated dir for generated project if not exist
if [ ! -d Generated_SublimeText_Project_GNU_GCC ]; then 
	mkdir Generated_SublimeText_Project_GNU_GCC
fi

## Step in Generated dir
cd Generated_SublimeText_Project_GNU_GCC

## Make Release dir for generated project if not exist
if [ ! -d Release ]; then 
	mkdir Release
fi

cd Release

## Clean cmake garbage if there
if [ -d CMakeFiles ]; then 
	rm -r CMakeFiles 
fi
if [ -f cmake_install.cmake ]; then 
	rm cmake_install.cmake 
fi
if [ -f CMakeCache.txt ]; then 
	rm CMakeCache.txt 
fi

## Rebuild Project
cmake -G "Sublime Text 2 - Unix Makefiles" -DRIVET_USE_CONFIG:BOOL=ON -DCMAKE_BUILD_TYPE=Release -DRIVET_EXPLICIT_COMPILER_ID:STRING="GCC" -DRIVET_EXPLICIT_HOST_ID:STRING="GNU" ../../Source

cd ..
## Make Debug dir for generated project if not exist
if [ ! -d Debug ]; then 
	mkdir Debug
fi
cd Debug

## Clean cmake garbage if there
if [ -d CMakeFiles ]; then 
	rm -r CMakeFiles 
fi
if [ -f cmake_install.cmake ]; then 
	rm cmake_install.cmake 
fi
if [ -f CMakeCache.txt ]; then 
	rm CMakeCache.txt 
fi

## Rebuild Project
cmake -G "Sublime Text 2 - Unix Makefiles" -DRIVET_USE_CONFIG:BOOL=ON -DCMAKE_BUILD_TYPE=Debug -DRIVET_EXPLICIT_COMPILER_ID:STRING="GCC" -DRIVET_EXPLICIT_HOST_ID:STRING="GNU" ../../Source