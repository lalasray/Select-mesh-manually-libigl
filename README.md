# libigl-demo project
## Tasks (DONE)

1. Minimal:
    
    Build libigl -> Enable reading geometry in OBJ file format -> Read appleseed shaderball object -> Visualize shaderball object (using libigl) -> Allow selecting mesh faces by clicking on them -> For each selected face: assign color, print face ID, vertices IDs, vertices coordinates in terminal -> Store all selected face IDs in a file.
    
2. Optional:

    Store all selected and unselected mesh faces in different OBJ and STL geometry files -> Visualize the selected and unselected mesh faces (using libigl)


## Steps

1.Install libigl

```
git clone https://github.com/libigl/libigl.git
```

2.Place this project right next to the libigl folder. For example, if libigl is installed in `~/foo/libigl`, then you can create a copy of this right next to it:

```
git clone https://github.com/lalasray/libigl-demo.git ~/foo/libigl-demo
```

3.Read appleseed shaderball object

```
git clone https://github.com/appleseedhq/shaderball

```

4.Extract the zip file to get the appleseed.obj


## Dependencies

The only dependencies are STL, Eigen, [libigl](http://libigl.github.io/libigl/) and the dependencies
of the `igl::opengl::glfw::Viewer` (OpenGL, glad and GLFW).
The CMake build system will automatically download libigl and its dependencies using
[CMake FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html),
thus requiring no setup on your part.

To use a local copy of libigl rather than downloading the repository via FetchContent, you can use
the CMake cache variable `FETCHCONTENT_SOURCE_DIR_LIBIGL` when configuring your CMake project for
the first time:
```
cmake -DFETCHCONTENT_SOURCE_DIR_LIBIGL=<path-to-libigl> ..
```
When changing this value, do not forget to clear your `CMakeCache.txt`, or to update the cache variable
via `cmake-gui` or `ccmake`.

## Compile

Compile this project using the standard cmake routine:

    mkdir build
    cd build
    cmake ..
    make

This should find and build the dependencies and create a `example_bin` binary.

For ease of use you can copy the appleseed.obj file into the build folder but you can also directly reference it using terminal.

## Run

From within the `build` directory just issue:

    ./example appleseed.obj

A glfw app should launch displaying the obj files.
By deafult all faces will be white in color but when you click a face it will be changed to red.
The clicked face ID, assosciated Vertices ID and there world coordinates will be displayed on the terminal.
It will also genrate a `face.txt` files containing the IDs of all faces which has been selected.The original Face matrix, the selected face matrix and unselected face matrix will be displayed in the terminal.
It will also genrate `selectedmesh.obj` and `selectedmesh.stl` for selected meshes and `unselectedmesh.obj` and `unselectedmesh.stl` for unselected meshes.

###To visualize the selected mesh press `keyboard "1"`
###To visualize the unselected mesh press `keyboard "2"`
    


## Reference

This is modified version of:

https://libigl.github.io/example-project/

Please,Refer to it in case of any probelms with dependancies.
