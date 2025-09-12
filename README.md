# Laser Triangulation App

> Application for 3D scanning using laser triangulation

---

- [Description](#description)
- [Installation](#installation)
- [Usage](#usage)
- [Contributions](#contributions)
  
---

##  Description

This application allows user to scan a 3D object using an IDS camera and a laser beam. This is done by finding an intersection between a camera ray that a point lies on and the laser beam defined by a plane. Application was tested for objects of different heights (between 1 and 50 mm) where the error was within 0.3 mm range. Tools available in this application include:

- Image Acquisition
- Point Height Detection
- Laser Beam Extraction and Skeletonization
- Point Extraction
- 2D and 3D Point Cloud Representation

Libraries used for the application include uEye library, OpenCV and OpenGL. Graphical user interface was developed using natID framework which is required for running the application as intended. There are also two seperate codes in "nonatid" folder that you can run without the framework. All of the algorithms are implemented here as well, apart from the 2D and 3D representation.

---

## Installation

### Prerequisites

- CMake (https://cmake.org/)
- uEye library (https://en.ids-imaging.com/downloads.html)
- OpenCV (https://opencv.org/)
- natID (https://github.com/idzafic/natID)

### Steps

- Install the required libraries and programs
- Configure the project using the given CMakeLists.txt file
- If the configuration doesn't succeed, make sure that:
  - natID was installed correctly  
  - The paths in CMakeLists.txt for different libraries are correct
  - The environment variable OpenCV_DIR pointing to OpenCV build exists
  - Paths to OpenCV lib and bin folder are added to the PATH variable
- Run the application using an IDE of choice

The application should be compatible with Windows, Linux and MacOS. Testing so far was done only on Windows devices. 

## Usage
The application consists of several tools which can be used as follows:

### Calibration
Calibration option opens a new tab where you can calibrate your camera. It has two different modes.
First is Auto Calibration mode where you can define your calibration board and the folder where the images of the board are located. By clicking the Auto button, the application will calculate the intrinsic and extrinsic camera parameters. Extrinsec camera parameters are calculated using the image with the first name in the alphabetical order.
Second mode is Manual Calibration in which you need to input the intrinsic and extrinsic camera parameters.
After the calibration is done, click save to apply the changes to the parameters. You can also export the parameters using Export button, and import them later using Import button. Make sure to click save after the import as well.

### Camera
Camera option let's you take images from your camera. The tools here are accessed by keyboard shortcuts:
- 'c' - save an image
- 'a' - set lower framerate
- 'd' - set higher framerate
- 'p' - enable auto focus
- 'm' - disable auto focus
- 'q' - close

### Point Height
Point Height option allow you to select an image and find coordinated of a point on the laser beam you click on. Click 'e' to clear the screen. Make sure to finish calibration before using this option

### Line Detection
Line Detection option allows you to extract the laser line from all images in a folder. You can select the HSV range for the laser beam, after which the skeletonization is done using Zhang-Suen algorithm.

### Line Height
Line Height option extracts all the laser line points from images in a folder which were created by the Line Detection tool. The points are saved in an XML file where X Y and Z coordinates are defined for every point. This option also requires calibration.

### 2D and 3D
2D and 3D options are used to see the points form an XML file (or a set of XML files) in a plane or space. In 2D mode you can use 'a' and 'd' to find the height of different points. If you want to move faster through points, hold down Ctrl. In 3D mode you can use 'w', 'a', 's', 'd', 'q' and 'e' to move through 3D space. You can also specify how far apart are different profiles of the object in mm.

## Contributions
### Application development
Admir Ahmespahić
### Used Icons
- https://icons8.com/icon/8200/target
- https://icons8.com/icon/11772/camera
- https://icons8.com/icon/19420/height
- https://icons8.com/icon/TRQf1O2splpS/transform
- https://icons8.com/icon/pDdm5kfzxP7q/transform
- https://icons8.com/icon/8310/scatter-plot
