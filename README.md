# Candy Distribution
&emsp;2D top down pixel-art extraction game.

## Descriptionher
&emsp;Game made in C using raylib with CMake as the build tool.
### Idea
You have entered a labyrinth by accident and are unable to escape and you fing other survivors in the labyrinth which gives you a little more hope so you try your hardest every day to collect resources and survive the labyrinth.

## Building and Running
* You need cmake 3.11 or newer installed to build the project, run the following cammand to see what version you have installed if any:
```bash
cmake --version
```
* Raylib is required to build and compile this project, if you have raylib installed in your system run the following:
```bash
cmake -S . -B build
cmake --build build
```
* If raylib is not installed in your system, run the following command to automaticallt download and compile raylib for the project:
```bash
cmake -S . -B build -DFETCH_RAYLIB=ON
cmake --build build
```

* After successfully building, you can find the executable inside the build folder labeled "build", you can run it in your file manager or by running the following in your terminal while in the parent directory:
```bash
./build/survivor
```

## License

Copyright (c) 2026 lokk. All rights reserved.

This project is licensed under the Elastic License 2.0 - see the [LICENSE](LICENSE) file for details.
