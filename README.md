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
- File Window:  (**Status: Visual**) / New, Open, Save, Save As (pending functional implementation)
- Edit Window:  (**Status: Visual**) / Undo, Redo, Copy, Paste, Cut (pending functional implementation)
- Options ->  (**Status: Functional**) / Config Window:

***Config Window***
- Application Info: (**Status: Functional**) / Engine Title, Creators, Organization
- Settings: (**Status: Functional**) / Allow Application toggle of fullscreen, windowed, bordererd, v-sync, and more visual settings. Visual frame rate histogram, Delta Time, and Frame Rate Cap.
- Hardware: (**Status: Functional**) / Detects user CPU, RAM, and GPU Data and Displays it, Info on SDL Compiled and Linker status.

***Hierarchy Window***
- Game Object Hierarchy: (**Status: Functional**) / User is able to see all Game Objects rendered into scene. Selecting Game Object opens information on Mesh, Texture, and Transform in *Inspector Window*.

***Asset Window***
- Asset Visibility: Users able to see Asset Folder in Project, Assets are displayed in grid similar to Unity and Unreal Stylizing. Drag&Drop from Asset Window is in development.

***Game Objects***
- Game Objects Rendering: Game Objects rendering possible from *Top Menu: Game Object* window.
- Game Object Primitives: Pyramid, Cylinder, Cube, Sphere & more primitives available for rendering from *Top Menu: Game Object*.
- Game Object Meshes: BakerHouse and Race Car meshes are renderable from *Top Menu: Game Object*.
- Drag&Drop: FBX meshes and textures able to be rendered in Scene from local PC drag location.
- Drag&Drop Storage: Dragged FBX/Texture objects are stored within the project in Assets Folder with subfolders titled "FBX_Assets" & "Image_Assets" based on file extension.

***Inspector:*** 

Requires selection of Game Object in Hierarchy
- Game Object Name: Game Object name is displayed on top of inspector window. Renaming method functionality is being implemented using IMGUI input text.
- Game Object Transform: Game Object Rotation, Scaling, and Positioning shown. Only Visual. Functional implementation in progress.
- Game Object Mesh: Game Object mesh path viewable, Game Object mesh visibility toggle visually implemented, Indexes & Vertices numbers are visible on Game Object selection. 


**Camera**:
- Camera Free Movement & Look Around based on Left-Right Click
- Camera Speed Adjustment via Camera Inspector
- Camera Perspective Zoom using scrollwheel
- Camera Orbitation implemented
- Camera Reset to old parameters to ensure User Functionality. 



## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](https://mit-license.org/) for further details.
