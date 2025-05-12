# Compiler project

This is a project to illustrate the principles of a single pass compiler.

It is highly OS / Hardware specific and I believe will only work on Windows x86_64 

It is intended to compile from a simple C like language into AT&T assembly using gcc.
At present it only compiles mathematical expressions with integer and float literals.

The compiler is written in C (compiler.c).

The compiler is called from the controller which
- outputs the compiled assembled code to a file
- assembles the code into a native code file
- runs and tests the native code.

Requirements:

- Windows x86_64  device
- GCC toolchain downloaded e.g. using MSys2
- gcc must be available in the PATH of the Windows device

To run:

Just execute the run-controller.bat file, this will run the controller and the compiler, output to assembly files and test the output.
