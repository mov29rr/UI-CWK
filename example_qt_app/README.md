# Qt Hello World

The following application, is a basic running instance of Qt6

## Build

The project uses CMake as a build system.

### Linux
For Linux systems, in a terminal at the root directory create a build directory, in which to place all build generated files, once in this directory target the cmake command at the root directory containing `CMakeLists.txt`, this command generates the Makefiles which build the code, once completed simply execute the Makefile, which should generate the executable `hello`. This executable can be executed with `./hello`.

Build:
```sh
mkdir build
cd build
cmake ..
make
```

Execute:
```sh
./hello
```
