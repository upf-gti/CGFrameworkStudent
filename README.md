# Computer Graphics
Follow the instructions below to clone, configure, and build the project.

#### Clone and initialize the repository:
```
git clone --recurse-submodules -j8 https://github.com/upf-gti/CGFrameworkStudent.git
```

Once cloned, you can optionally [work on your own repository](#creating-your-own-repository).

The framework provided is compatible with all operating systems. Here are the steps for each platform:
1. [Windows](#windows)
2. [Mac](#mac)
3. [Linux](#linux)


## Windows
1. Install [MS Visual Studio Community](https://visualstudio.microsoft.com/es/free-developer-offers/) <br>
&rarr; Select **"Desktop Development with C++"**
2. Install [*CMake*](https://cmake.org/download/) <br>
  &rarr; Select "Windows x64 Installer" for the last version. <br>
  &rarr; Enable **"Add CMake to PATH"** during installation (otherwise CMake won't work in the terminal)

#### Configure the project: <br>
Open a Windows Terminal, go to the project folder, and run:
```console
cd CGFrameworkStudent
mkdir build
cd build
cmake .. -DCMAKE_POLICY_VERSION_MINIMUM=3.12
```

Double click on the ``.sln`` VS project inside the ``build/`` folder.

You are all set!

Remember you can optionally [create your own repository](#creating-your-own-repository).

## Mac
1. Install XCode (you may need to update MacOS version)
2. Install Homebrew (to install the missing libraries). Open a terminal and run this command:
```console
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
3. Install *CMake*:
```console
brew install cmake
```

#### Configure the project: <br>
Open a MacOS Terminal, go to the project folder, and run:
```console
cd CGFrameworkStudent
mkdir build && cd build
cmake -G "Xcode" ..
```

This will generate a XCode project inside the folder ``build/``.

You are all set!

Remember you can optionally [create your own repository](#creating-your-own-repository).

### How to solve errors in Mac build

If *CMake* fails, run this command an try again:
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
1. Install *CMake*, libraries and compilers using these commands:
```console
sudo apt install cmake
sudo apt install build-essential
sudo apt install -y libglu1-mesa-dev freeglut3-dev mesa-common-dev libgl1-mesa-dev
```

#### Configure the project: <br>
Open a Linux Terminal, go to the project folder, and run:
```console
cd CGFrameworkStudent
mkdir build && cd build
cmake ..
```

This will generate a Makefile inside the folder ``build/``.

Use ``make`` to compile. You can speed-up compilation using more threads with ``-j(num threads)``, for example: ``make -j8``.

Remember you can optionally [create your own repository](#creating-your-own-repository).

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
If you want to push your local copy to your own GitHub repo:
1. Create an empty private repository on GitHub
2. Open the terminal
3. Go to the *CGFrameworkStudent* cloned repository folder:
```
  cd CGFrameworkStudent
```
4. Point your local repo to your newly created GitHub repo:
```
  git remote set-url origin <your-repository-url.git>
```
5. **IMPORTANT:** verify both "fetch" and "push" are pointing to your repository:
```
  git remote -v
```
&emsp;&emsp;You should see:
```
  origin  <your-repository-url.git> (fetch)
  origin  <your-repository-url.git> (push)
```
6. Update changes to the remote
```
  git push
```
