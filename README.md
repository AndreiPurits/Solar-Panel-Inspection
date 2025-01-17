# Solar Panel Hotspot Detection with Thermographic Camera

This repository contains the source code and GUI for my Master's thesis project, where I developed a system to detect outliers (hotspots) in solar panel data using a thermographic camera. The project highlights the use of advanced computer vision techniques implemented in C++.

## Features
- **Hotspot Detection**: Identify and highlight hotspots on solar panels using thermal images.
- **User-Friendly GUI**: Includes an interface to upload images, process data, and visualize results with hotspots clearly marked.
- **Optimized C++ Code**: High-performance implementation for real-time processing.

## Demo
Below is an example of the working program with a highlighted hotspot:

![Working Example](https://github.com/AndreiPurits/Solar-Panel-Inspection/blob/main/guiii.png)

## How It Works
1. **Data Input**: Upload a thermographic image of the solar panel.
2. **Processing**: The algorithm analyzes the image to detect temperature anomalies (hotspots).
3. **Visualization**: The GUI displays the processed image with hotspots marked for easy identification.

## Installation
Unfortunately, the program requires the proprietary ERIS thermographic software, which is essential for capturing and preprocessing thermal images. Without this software, running the program as intended is not possible.

## Why This Repository?
While you might not be able to fully run the program without ERIS, this repository serves several purposes:

# Documentation: 
1. Provides a detailed overview of the techniques and implementation for academic and professional reference.
2. Learning Resource: Offers insights into developing similar systems using thermographic data and computer vision in C++.
3. Showcase: Demonstrates the methodology and outcomes of my Master's thesis project.
If you have access to ERIS thermographic software or similar tools, you could adapt the code for your specific use case. Otherwise, feel free to explore the repository to learn from the concepts and techniques applied here.

