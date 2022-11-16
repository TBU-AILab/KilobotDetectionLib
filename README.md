# KILOLIB library

Library containing basic functions and classes for Kilobot detection and tracking, camera calibration and distance
measuring.

## Implementation into your project

There are two simple ways how to implement the library into your project. The whole library project is based on CMake,
so you can directly use it as a CMake subproject in your application. Because the library is composed of only a few
files, the files can be copied into your project separately, and any build system can be used.

### Using CMake subproject

The library can be cloned into your project structure as a subfolder. In that case, you need to append the following
lines to your main CMakeList.txt file.

```cmake
  # add subdirectory and enable the library compilaction
add_subdirectory(Kilobot_detection_library)

# link library with your app and set include paths
# NEED TO BE PLACED AFTER  add_executable/add_library command 
target_link_libraries(${CMAKE_PROJECT_NAME} Kilobot_detection_library)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC [path to the lib]/include)
```

Moreover, the library project also contains example applications. Their compilation is disabled by default. You can
enable the compilation by adding the following -D parameters into your cmake command.

```bash
  cmake -DBUILD_EXAMPLES:BOOL=ON ../
```

### Using only library files

The library is composed of files placed in /include and /src folder. These files must be copied into your project.
If you use a different folder structure, maybe the correction of include paths will be necessary.

## Lib Doxygen documentation

<https://tbu-ailab.github.io/Kilolib/doc/output/html/>

## Examples

The example applications are placed in "/examples/ folder. Each example in specific subfolder. Compilation of examples
is disabled in default. If you want to enable it you need to append following -D parameter into your cmake command.

```bash
  cmake  -DBUILD_EXAMPLES:BOOL=ON ../
```

### Example descritipon

//TODO:

## Project structure

```
..
.
+-- examples # example applications using the library
|   +-- calibration_load
|   +-- calibration_store
|   +-- detector_and_tracker # most detailed example including updated tracker
|
+-- include             # libraries header files
+-- src                 # libraries source files
|
+-- utils   
|   +-- images          # set of photos for image calibration
|   +-- best.onnx       # NN (YOLOv5) Model used in examples
|   +-- bestCUDA.onnx   # NN (YOLOv5) Model used in examples - CUDA enabled
|   +-- bodyguard.mp4   # example video of one Kilobot's experiment
|
+-- CMakeLists.txt      # the configuration the whole library
```
