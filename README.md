# VertX Game Engine

## Description

The following repository was created to host the creation of the **VertX Game Engine**, made during our 3rd year Game Engines lesson in university at CITM-UPC. 
 
This game engine has been made in **C/C++** programming language, and also using the following **3rd party libraries**:

- [OpenGL](https://www.opengl.org/)
- [SDL 2](https://www.libsdl.org/)
- [GLEW](https://glew.sourceforge.net/)
- [GLM](https://glm.g-truc.net/0.9.9/)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [Assimp](https://assimp.org/)
- [DevIL](https://openil.sourceforge.net/)

You can click [here](https://github.com/CITM-UPC/VertX-Game-Engine) to check out our Github repository where the project is hosted.


## Developers

![](https://raw.githubusercontent.com/CITM-UPC/VertX-Game-Engine/main/TeamPhotos/adriapons.jpg)
 - [Adrià Pons Mensa](https://github.com/AdriaPm)
 
![](https://raw.githubusercontent.com/CITM-UPC/VertX-Game-Engine/main/TeamPhotos/rylangraham.jpg)
 - [Rylan Graham](https://github.com/RylanJGraham)


## Controls

**Camera**:
- {RIGHT Click + WASD} - Camera free movement
- {RIGHT Click + WASD + LSHIFT} - Speeded camera free movement
- {RIGHT Click + Mouse Input} - Camera free look around
- {Mouse WHEEL} - Camera perspective zoom in & out
- {LALT + LEFT Click} - Camera orbitation
- {F} - Reset camera parameters to initial values

## Features

**UI**

***Top Menu***
- File Window: *Visual*: New, Open, Save, Save As (pending functional implementation)
- Edit Window: *Visual* Undo, Redo, Copy, Paste, Cut (pending functional implementation)
- Options -> *Functional* Config Window:

  ***Config***
  - Application Info: *Functional* Engine Title, Creators, Organization
  - Settings: *Functional* Allow Application toggle of fullscreen, windowed, bordererd, v-sync, and more visual settings. Visual frame rate histogram, Delta Time, and Frame Rate Cap. 


**Camera**:
- Camera Free Movement & Look Around based on Left-Right Click
- Camera Speed Adjustment via Camera Inspector
- Camera Perspective Zoom using scrollwheel
- Camera Orbitation implemented
- Camera Reset to old parameters to ensure User Functionality. 



## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](https://mit-license.org/) for further details.
