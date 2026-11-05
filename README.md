# Candy Distribution
&emsp;Top Down 2D survival/horror game made in c.

## Description
&emsp;Game made in C using raylib with CMake as the build tool.
### Idea
You are in halloween dressed as a worm and you need to to feed the kids candy and run away or there will be consequences but the costume restrics your movement so to run away you must find the zipper part to remove your costume.
Your movement is limited to drag and pull.

## Building and Running
* You need cmake 3.11 or newer installed to build the project, run the following cammand to see what version you have installed if any:
```bash
cmake --version
```
* Raylib is required to build and compile this project, if you have raylib installed in your system run the following:
```bash
cmake -S . -B build
```
* If raylib is not installed in your system, run the following command to automaticallt download and compile raylib for the project:
```bash
cmake -S . -B build -DFETCH_RAYLIB=ON
```

* After successfully building, you can find the executable inside the build folder labeled "build", you can run it in your file manager or by running the following in your terminal while in the parent directory:
```bash
./build/candy
```

## License

Copyright (c) 2026 lokk. All rights reserved.

This project is licensed under the Elastic License 2.0 - see the [LICENSE](LICENSE) file for details.
