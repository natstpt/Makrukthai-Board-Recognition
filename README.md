# Makruk Board Recognition

![Makruk Board Recognition Screenshot](https://raw.githubusercontent.com/natstpt/Makrukthai-Board-Recognition/main/screencapture1.png)

This project uses the OpenCV library for image processing to recognize a Makruk (Thai chess) board and output its position in FEN (Forsyth-Edwards Notation) format. It detects the pieces on the board using template matching and displays the detected pieces with rectangles around them.

## Table of Contents

- [Dependencies](#dependencies)
- [How to Build and Run](#how-to-build-and-run)
  - [Install OpenCV](#install-openCV)
  - [Create a Visual Studio Project](#create-a-visual-studio-project)
  - [Configure the Project for OpenCV](#configure-the-project-for-opencv)
  - [Add the Source Code](#add-the-source-code)
  - [Build and Run the Program](#build-and-run-the-program)
- [Project Structure](#project-structure)
- [Acknowledgments](#acknowledgments)

## Dependencies

- OpenCV (version 4.7.0)
- Visual Studio 2022
- Windows OS

## How to Build and Run

### Install OpenCV

1. Download the OpenCV pre-built Windows package from the official website: [OpenCV Releases](https://opencv.org/releases/).
2. Extract the downloaded package to a directory of your choice (e.g., `C:\opencv`).
3. Add the OpenCV binaries to your system's PATH environment variable. Go to `Environment Variables` and add `C:\opencv\build\x64\vc16\bin` (assuming you extracted OpenCV to `C:\opencv`).

### Create a Visual Studio Project

1. Open Visual Studio 2022 and create a new project. Select `Empty Project (C++)` as the project template.
2. Name the project `MakrukBoardRecognition` and set the desired location.

### Configure the Project for OpenCV

1. In the Solution Explorer, right-click the project name and select `Properties`.
2. In the Properties window, select `Configuration` as `All Configurations` and `Platform` as `x64`.
3. Go to `Configuration Properties` > `C/C++` > `General`, and add the OpenCV include directory to `Additional Include Directories`. The path should be something like `C:\opencv\build\include`.
4. Go to `Configuration Properties` > `Linker` > `General`, and add the OpenCV library directory to `Additional Library Directories`. The path should be something like `C:\opencv\build\x64\vc16\lib`.
5. Go to `Configuration Properties` > `Linker` > `Input`, and add the OpenCV libraries to `Additional Dependencies`. For example, if you are using OpenCV 4.7.0, add `opencv_world470.lib` for `Release` configuration and `opencv_world451d.lib` for `Debug` configuration.

### Add the Source Code

1. In the Solution Explorer, right-click the `Source Files` folder, and select `Add` > `New Item`.
2. Choose `C++ File (.cpp)` and name it `main.cpp`.
3. Copy and paste the provided source code into `main.cpp`.
4. Create two new folders in your project directory: `testimages` and `extractimages`.
5. Add the test images of Makruk boards to the `testimages` folder and the template images for each Makruk piece to the `extractimages` folder.

### Build and Run the Program

1. In Visual Studio 2022, press `F7` or click `Build` > `Build Solution` to build the program.
2. Press `Ctrl + F5` or click `Debug` > `Start Without Debugging` to run the program.
3. When prompted, choose your playing side: white (w) or black (b).
4. The program will load an image of the Makruk board (`testimages/makrukthai1.png`) and analyze it.
5. The detected board position will be output in FEN format, and an image with rectangles around the detected pieces will be displayed.

## Project Structure

- `main.cpp`: The main program containing

## Acknowledgments

I would like to express our gratitude to the following resources:

1. **Pieces image**: The piece images used in this project are sourced from [PlayOK](https://www.playok.com/th/makruk/).
