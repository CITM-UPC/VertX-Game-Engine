# VertX Game Engine

## Description

The following repository was created to host the creation of the **VertX Game Engine**, made during our 3rd year Game Engines university course at CITM-UPC. Please go to section **Controls** for Engine Controls, and **Features** to see the latest updates to functionality.
 
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

![](https://raw.githubusercontent.com/CITM-UPC/VertX-Game-Engine/main/TeamPhotos/joelchaves.jpg)
- [Joel Chaves](https://github.com/JoeyCM)

## Controls

**Camera**:
- {RIGHT Click + WASD} - Camera free movement
- {RIGHT Click + WASD + LSHIFT} - Speeded camera free movement
- {RIGHT Click + Mouse Input} - Camera free look around
- {Mouse WHEEL} - Camera perspective zoom in & out
- {LALT + LEFT Click} - Camera orbitation
- {F} - Reset camera parameters to initial values

## Features

### Top Menu
- [x] **Visual -> File Window:** New, Open, Save, Save As (pending functional implementation) 
- [x] **Visual -> Edit Window:** Undo, Redo, Copy, Paste, Cut (pending functional implementation)
- [x] **Functional -> Window:** Window Menu to display all displays necessary for UI including: *Assets*, *Inspector*, *Assets*, *Console Log*, *Camera Inspector*. Users can see which windows are active. 
- [x] **Functional -> Options Window:** User is able to open *Config Window*. This is expanded on below. 
- [x] **Functional -> Game Object Window:** User is able to render pre-stored primitive objects, and pre-stored textured .fbx file meshes. 
- [x] **Functional -> Help Window:** Window featuring "About" information on Project Libraries and Licensing. Users can also get live access to this Github Repository Page. 

### Config Window
- [x] **Functional -> Application Info:** Engine Title, Creators, Organization.
- [x] **Functional -> Settings:** Allow Application toggle of fullscreen, windowed, bordered, v-sync, and more visual settings. Visual frame rate histogram, Delta Time, and Frame Rate Cap.
- [x] **Functional -> Renderer:** User can Enable & Disable Lighting in the scene, User can enable depthtest functionality for the rendered Game Objects in the scene.
- [x] **Functional -> Hardware:** Detects user CPU, RAM, and GPU Data and displays it. Info on SDL Compile and Linker status.

### Hierarchy Window
- [x] **Functional -> Game Object Hierarchy:** User is able to see all Game Objects rendered into the scene. Selecting Game Object opens information on Mesh, Texture, and Transform in *Inspector Window*.
- [x] **Functional -> Empty Game Object creation:** User is able to create new Empty Game Objects that will be displayed in the *Hierarchy window*.
- [x] **Functional -> Game Object deletion:** User is able to delete all Scene's game objects and also the children of each game object.

### Asset Window
- [x] **Functional -> Asset Visibility:** User is able to see Assets Folder in Project, Assets are displayed in grid similar to Unity and Unreal Stylizing. Drag&Drop from Asset Window is in development.
- [x] **Functional -> Asset Import:** User is able to import their own local files to the Assets Folder in Project by clicking the "Import Local Files" button in the *Assets window*.
- [x] **Functional -> Asset Deletion:** User is able to delete files from the Assets Folder in Project by left-clicking when hovering with the mouse the desired file and a popup window will appear.

### Game Objects
- [x] **Functional -> Game Object Rendering:** Pre-stored Game Objects rendering possible from *Top Menu: Game Object* window. These include primitives, &Textured FBX mesh files (Baker House, Medieval Bed, Obelisk)
- [x] **Functional -> Game Object Primitives:** Pre-stored base Pyramid, Cylinder, Cube, Sphere & more primitive shapes available for rendering from *Top Menu: Game Object*.
- [x] **Functional -> Game Object Mesh Texturing:** FBX assets can have Texture toggled On/Off.
- [x] **Functional -> Drag & Drop Texture&FBX:** FBX meshes and textures able to be rendered in Scene from local PC drag location.
- [x] **Functional -> Drag & Drop Project Storage:** Dragged FBX/Texture objects are stored within the project in Assets Folder with subfolders titled "Library", with all imported and loaded files.
- [x] **Functional -> Game Object Components:** Game Objects have component structure, that controls Mesh, Texture, and Transform when called. These components are called on select in the *Inspector Window*. 
- [x] **Functional -> Game Object View Options:** Game Objects can be rendered to display Vertex Normals & Face Normals on toggle in the *Inspector Window*. Visible normal length can be adjusted via a slider.

### Console
- [x] **Functional -> Editor Console Display:**  Editor console displays to user updates in Renderer, Windows Status, Library Loading & more.
- [x] **Functional -> Editor Console Find:**  User can sort through Console display to find desired elements in search bar.

### Inspector:
Requires selection of Game Object in Hierarchy:
- [x] **Functional -> Game Object Name:** Selected Game Object name (via *Hierarchy*) is displayed on top of inspector window. Renaming method functionality is being implemented using IMGUI input text.
- [x] **Visual -> Transform:** Game Object Rotation, Scaling, and Positioning can be modified.
- [x] **Functional -> Mesh:**  Game Object mesh path is viewable. Game Object Indexes & Vertices numbers are visible. Ability to toggle texture added. Ability to display normals visually, and toggle visibility length. 
- [x] **Functional -> Texture:** Game Object texture path viewable. Information on texture size is available. 

### Camera
- [x] **Functional -> Camera Movement & Look Around:** User can explore scene based on Left-Right Click functionalities. User able to move camera via WASD movement commands. Camera controls are explained above in section *Controls*.
- [x] **Functional -> Camera Object:** User is able to see current camera location in scene via *Camera Inspector* Window. Scene monitors cameras position.  
- [x] **Functional -> Camera Settings:** Camera Speed Adjustment, and more available via *Camera Inspector* Window
- [x] **Functional -> Camera Perspective Zoom:** User can zoom by using scrollwheel.
- [x] **Functional -> Camera Orbitation:** Camera Orbitation implemented around selected Game Object in *Hierarchy* window.
- [x] **Functional -> Camera Reset:** Camera Reset to old parameters to ensure User Functionality.
- [x] **Functional -> Camera Frustum Culling:** Camera is a component with settings that can be modified.

### Camera Inspector
- [x] **Functional -> Camera Speed & Multiplier:** User can modify camera speed for WASD movement using slider toggle. User can also modify the camera speed multiplier to further increase functionality.
- [x] **Functional -> Camera Mouse Sensitivity:** User can modify camera look around speed via slider toggle.
- [x] **Functional -> Mouse Parameters:** User can see the X&Y position of their mouse cursor in the scene. They can also see the position change.
- [x] **Functional -> Camera Parameters:** User can see the X&Y&Z axis position of their Camera. They can also see the position change. They are also display the reference vector position, and the up vector position. 
- [x] **Functional -> Camera Frustum Culling Parameter:** Camera component parameters such as FOV, zNear & zFar rendering planes.

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed-source software. Check [LICENSE](https://mit-license.org/) for further details.
