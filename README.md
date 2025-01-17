# Computer Graphics

Clone and initialize the repository:
```
git clone --recurse-submodules -j8 https://github.com/upf-gti/CGFrameworkStudent.git
```

The framework provided is compatible with all operating systems.
The steps for each platform are mandatory to be able to build the framework.
The mandatory C++ IDEs are the following:

* MS Visual Studio Community (Windows)
* XCode (Mac)
* Visual Studio Code (Linux, optional for other platforms)

Detailed installation information can be seen below.

## Windows

MS Visual Studio Community can be downloaded from [here](https://visualstudio.microsoft.com/es/free-developer-offers/). Make sure you select **"Desktop Development with C++"**.

In addition you need to install *CMake* which can be downloaded from [here](https://cmake.org/download/). Select "Windows x64 Installer" for the last version. Remember to set **"Add CMake to PATH"** when asked while installing, otherwhise you won't be able to call CMake from the terminal.

Once you have all required open a Windows Terminal, go to the project folder and do this steps:
```console
cd CGFrameworkStudent
mkdir build
cd build
cmake ..
```

This will generate a Visual Studio project inside the folder ``build/`` (.sln) that you can use to compile and debug the framework.

## Mac

You need XCode installed in your system (you may need to update MacOS version), Homebrew to install the missing libraries and also *CMake*. 

To install Homebrew open a terminal and run this command:
```console
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Continue by installing *cmake* using this command:
```console
brew install cmake
```

Once you have all required open a MacOS Terminal, go to the project folder and do this steps:
```console
cd CGFrameworkStudent
mkdir build && cd build
cmake -G "Xcode" ..
```

This will generate a XCode project inside the folder ``build/`` that you can use to compile and debug the framework.

### How to solve errors in Mac build

If this process leads to cmake errors, run this command an try again:
```console
sudo xcode-select --reset
```

If the error says that it cannot find C/C++ compilers, find them using:
```console
xcrun -find c++
xcrun -find cc
```

Then, try to build again (inside the build directory) specifying the paths:
```console
cmake -D CMAKE_C_COMPILER="Path_of_C_compiler" -D CMAKE_CXX_COMPILER="Path_of_C++_compiler" -G Xcode ..
```

## Linux

Install *cmake*, libraries and compilers using this command:
```console
sudo apt install cmake
sudo apt install build-essential
sudo apt install -y libglu1-mesa-dev freeglut3-dev mesa-common-dev libgl1-mesa-dev
```

Once you have all required open a Linux Terminal, go to the project folder and do this steps:
```console
cd CGFrameworkStudent
mkdir build && cd build
cmake ..
```

This will generate a Makefile inside the folder ``build/`` that you can use to compile framework.

Use ``make`` to compile. You can speed-up compilation using more threads with ``-j(num threads)``, for example: ``make -j8``.

### Visual Studio Code (Linux)

After installing all the libs for your platform, if you need a more light weight IDE which can be used in any platform (included Linux), this is your better option.

Visual Studio Code can be downloaded for each platform [here](https://code.visualstudio.com/download).

**NOTE**: you must have a C/C++ compiler installed and added to the path before progressing with the installation.

#### Configuring VSCode

After VSCode and the requirements for each platform are installed **(the steps for each platform are mandatory!)**, the following extensions are needed to work with C++ code:

```
C/C++
C/C++ Extension Pack
C/C++ Themes
Cmake Tools
```

Then, on VSCode, open the project folder where the CMakeLists.txt is located and the configuration of the project should start automatically. At this point you should be able to build and run the project using the VSCode interface. 

If not, then open the CMake tab on the left of VSCode, and select the configure and build the project options.

Check [this link](https://gourav.io/blog/setup-vscode-to-run-debug-c-cpp-code) to learn how to debug the framework in Visual Studio Code.


## Creating your own repository

After cloning the framework's repository, if you want to work on your own repository:
  - Open the terminal
  - Go to the CGFrameworkStudent cloned repository folder:
```
  cd CGFrameworkStudent
```
  - Create an empty private repository on GitHub.
  - Make the CGFrameworkStudent cloned repository point to your own repository:
```
  git remote set-url origin <your-repository-url.git>
```
  - IMPORTANT make sure that both "fetch" and "push" are pointing to your repository:
```
  git remote -v
```
  - You should see something like:
```
  origin  <your-repository-url.git> (fetch)
  origin  <your-repository-url.git> (push)
```
  - Update changes to the remote
```
  git push
```
