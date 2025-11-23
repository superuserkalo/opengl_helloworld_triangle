# opengl_helloworld_triangle

Hello World of Graphics API programming, the triangle!

<img width="803" height="630" alt="image" src="https://github.com/user-attachments/assets/4a7ecdfc-59b7-487a-a895-604887a5fcb3" />

A good guide for getting into opengl and the one I relied on writing this is:
https://open.gl/drawing

C++ Standard: C++20
Compiler and Version: MSVC 19.44.35213 for x86
Build and Debug on x64

This project relies on GLAD (GL API Dynamic Loader) to correctly initialize and load 
the function pointers for modern OpenGL (version 3.3 Core Profile) exposed by your graphics driver. 
The necessary glad.c and header files were generated using the GLAD tool: 
https://glad.dav1d.de

Settings used for GLAD:
Langauge: C/C++
Specification: OpenGL
gl: Version 3.3
Profile: Core
Extensions: None
Generate a loader: Selected

This project also relies on SDL3(Simple DirectMedia Layer 3) a cross-platform software development library 
designed to provide a hardware abstraction layer for computer multimedia hardware components.
This is always my hardware abstraction layer of choice for user-input, window creation and management etc.

SDL3 Installation Guide:
https://github.com/libsdl-org/SDL/blob/main/INSTALL.md
I downloaded and used the VC Devel

SDL also provides Graphics APIs itself with SDL_Renderer and now with SDL3 SDL_GPU which porvides modern graphics programming.
I still chose to use OpenGL seperately on its own for the sake of learning purposes.

API by Category reference(very helpful): 
https://wiki.libsdl.org/SDL3/APIByCategory

How I built it for reference:
Exclaimer: This is just how I did it, it might be more correct ways of doing it such as using cmake or vcpkg!

## 1. Source Integration (GLAD)

The generated glad.c source file, must be added directly to your project's **Source Files** and compiled with the main application(location of main.cpp).

## 2. Build-Time Linking (SDL3 and OpenGL)

This configuration tells the compiler and linker where to find the necessary files (the .h headers and the static .lib libraries). These changes are made in the **Project Properties** window.
You can get there by right clicking your Project and clicking properties. 

| Property Location                       | Action |                                                | Value to Specify |

| **C/C++ $\to$ General**    | **Additional Include Directories**     | Path to the folders containing the **SDL3 headers** and **GLAD headers**. |
| **Linker $\to$ General**   | **Additional Library Directories**     | Path to the folder containing the **SDL3 static libraries** (the .lib files). |
| **Linker $\to$ Input**     | **Additional Dependencies**            | SDL3.lib; SDL3main.lib; opengl32.lib; |


## 3. Run-Time Requirement

The project will compile but fail to run unless the SDL dynamic library is present in the executable's directory.

1.  **Locate the DLL:** Find the **SDL3.dll** file within the original SDL development package you downloaded.
2.  **Copy the DLL:** Copy the **SDL3.dll** file into the output folder that contains the compiled executable (`.exe`). This is typically the **Debug** or **Release** folder or where your **main.ccp** is located.

**If this DLL is missing, the program will compile successfully but crash immediately upon launch.**



