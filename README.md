# StudyMate – Pomodoro Clock with GUI

## Overview
**StudyMate** is a cross-platform time-management application designed to help users improve productivity and focus.  
It implements the **Pomodoro Technique** with a modern GUI and provides **concentration analytics** to track study/work sessions.


## Features
- ⏱️ **Task Timer** – Implements the Pomodoro technique with customizable work/break durations  
- 📊 **Concentration Analytics** – Tracks completed sessions and generates simple statistics  
- 🖥️ **Cross-Platform GUI** – Built with Qt, runs on Windows, Linux, and macOS  

## Screenshots
<p align="center">
  <img src="./docs/screenshot_main.png" alt="StudyMate Main Window" width="400"/>
  <img src="./docs/screenshot_stats.png" alt="StudyMate Statistics" width="400"/>
</p>

*(Add screenshots in `docs/` folder for better presentation.)*

## Tech Stack
- **C++**  
- **Qt (Cross-Platform GUI Framework)**  

## Usage
Clone the repo and build with Qt Creator or CMake:

```bash
git clone https://github.com/Leeye1/StudyMate-GUI.git
cd StudyMate-GUI
mkdir build && cd build
cmake ..
make
./StudyMate
