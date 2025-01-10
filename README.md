# Computer Graphics

Clone and initialize the repository:
```
git clone --recurse-submodules -j8 https://github.com/upf-gti/CGFrameworkStudent.git
```
If you want to work on your own repository:
  - Go to the CGFrameworkStudent cloned repository folder:
```
  cd CGFrameworkStudent
```
  - Create an empty private repository on GitHub.
  - Make the cloned repository point to your own repository:
```
  git remote set-url origin <your-repository.git>
```
  - IMPORTANT make sure that both "fetch" and "push" are pointing to your repository:
```
  git remote -v
```
  - You should see something like:
```
  origin  <your-repository.git> (fetch)
  origin  <your-repository.git> (push)
```
  - Update changes to the remote
```
  git push
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
mkdir build && cd build
cmake -G "Xcode" ..
```

This will generate a XCode project inside the folder ``build/`` that you can use to compile and debug the framework.

If this process leads to cmake errors, run this command an try again:

```console
sudo xcode-select --reset
```

## Linux

Install *cmake* and needed libraries using this command:
```console
sudo apt install cmake
sudo apt install build-essential
sudo apt install -y libglu1-mesa-dev freeglut3-dev mesa-common-dev libgl1-mesa-dev
```

Once you have all required open a Linux Terminal, go to the project folder and do this steps:
```console
mkdir build && cd build
cmake ..
```

This will generate a Makefile inside the folder ``build/`` that you can use to compile framework.

Use ``make`` to compile. You can speed-up compilation using more threads with ``-j(num threads)``, for example: ``make -j8``.

### Visual Studio Code (Linux, optional for other platforms)

After installing all the libs for your platform, if you need a more light weight IDEs which can be used in any platform (included Linux), this is your better option.

Visual Studio Code can be downloaded for each platform [here](https://code.visualstudio.com/download).

#### Configuring VSCode

After the VSCode and the requirements for each platform are installed **(the steps for each platform are mandatory!)**, the following extensions are needed to work with C++ code:

```
C/C++
C/C++ Extension Pack
C/C++ Themes
Cmake Tools
```

Check [this link](https://gourav.io/blog/setup-vscode-to-run-debug-c-cpp-code) to learn how to debug the framework in Visual Studio Code.

First, open the project folder where the CMakeLists.txt is located, then open the CMake tab on the left, configure and build the project.
