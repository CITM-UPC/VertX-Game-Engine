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

You can click [here](https://citm-upc.github.io/VertX-Game-Engine/) to check out VertX Game Engine's official website.

## Controls

**Camera**:
- {RIGHT Click + WASD} - Camera free movement
- {RIGHT Click + WASD + LSHIFT} - Speeded camera free movement
- {RIGHT Click + Mouse Input} - Camera free look around
- {RIGHT Click +  CTRL} - Descended Camera Movement
- {WASD} - When in Game Controls Movement of Selected Game Objects
- {Mouse WHEEL} - Camera perspective zoom in & out
- {LALT + LEFT Click} - Camera orbitation
- {F} - Reset camera parameters to initial values

# Features in Editor

## Top Menu
**File Dropdown:** New Scene, Open Scene, Save, Save As, Load Scene. <br>
**Edit Dropdown:** Play, Pause, Step, & *Settings Window*. Edit Window also includes Undo, Redo, Select, Select All, Select Children, Invert Children, & more however these are visual and not functional features. <br>
**Debug:** Enable *Camera Debug*, *Raycast Debug*, *Bounding Boxes Debug*, and *Demo Window* (for IMGUI customization). <br>
**Window:** Window dropdown displays all displays necessary for UI including: *Assets*, *Inspector*, *Hierarchy*, *Console Log*, *File Explorer*, and *FPS Graph*. Users can also see which windows are currently active. <br>

### Game Objects Window:
**Game Object Primitives** User is able to access GameObject Features including GameObject Asset Creation (Planes, Quads, Circles, Squares etc) <br>

**Enable Draw Mode** <br>

**Move/Delete Game Objects Functionality** activate the *Move & Delete Game Object Window* (only accessible with Game Object selected in Scene). <br>

### Help Window
**Check Releases:** User is able to click Check Releases item to check VertX engine release updates online <br>

**Learn More** select About item to view information on team and included libraries. <br>

## Hierarchy Window / System
**Game Object Hierarchy:** User is able to see all Game Objects rendered into the scene. <br>

**Selection** Selecting Game Object opens information on Mesh, Texture, and Transform in the *Inspector Window*.<br>

**Structuring** Hierarchy window uses recursive child, parent structuring, where the user is able to toggle the dropdown and view all Game Object components. <br>

## Asset Window / System
**Asset Window Visibility:** User is able to see Base VertX Folder in the Project, items in the assets window are displayed in grid similar to Unity and Unreal Stylizing. <br>

**Asset Window Navigation:** User's can navigate through the asset window by using the Back button, and on double click of files. <br>

**Asset Adding** User is able to import their own local files to the Assets Folder in Project by clicking the "Import Local Files" button. User is also able to add assets via Drag&Drop functionality. <br>

**Assets -> Library** Assets are copied to the Library folder when they are loaded into the scene. This allows for only the Library folder being necessary on project load&save. Library folder is sorted via Materials and Meshes. <br>

**Asset Deletion:** User is able to delete files from the Assets Window by *right-clicking* when hovering with the mouse the desired file and a popup window will appear.<br>

## Game Objects System 
**Game Object Primitives**: Pre-stored base Pyramid, Cylinder, Cube, Sphere & more primitive shapes available for rendering from *Top Menu: Game Object*.<br>

**Game Object Mesh Texturing:** FBX assets can have Texture toggled On/Off. As well as checkered texturing enabled/disabled. <br>

**Drag & Drop Rending of Texture&FBX:** FBX meshes and textures able to be rendered in Scene from local PC drag location. Make sure texture is bound to FBX for full support. <br>

**Game Object Components:** Game Objects have component structure, that controls Mesh, Texture, and Transform when called. These components are called on select in the *Inspector Window*. <br>

**Game Object View Options:** Game Objects can be rendered to display Vertex Normals & Face Normals on toggle in the *Inspector Window*. Visible normal length can be adjusted via a slider.<br>

**Game Object Parenting System** Game Objects can be parented or unparented. This is done by selected a Game Object in Hierarchy, then going to the *Top Menu: Game Object* item and selecting "Move" (which activates the *Reparenting Window*) or "Delete" item. <br> 

## Console Log Window
**Editor/Engine Console Display:** Editor console displays to user updates in Renderer, Windows Status, Library Loading & more. <br>

**Editor Console Find:** User can sort through Console display to find desired elements in search bar. <br>

## Inspector:
*Requires selection of Game Object in Hierarchy:*
**Game Object Naming:** Selected Game Object name (via *Hierarchy*) is displayed on top of inspector window. You are able to rename the Game Object to what you wish.  <br>

**Transform:** Game Object Rotation, Scaling, and Positioning can be modified by sliding the sliders or by inputing a desire number.  <br>

**Mesh:**  Game Object mesh path is viewable. Game Object Indexes & Vertices numbers are able to be toggled. Ability to display normals visually, and toggle visibility length.  <br>

**Texture:** Game Object texture path is viewable. Information on texture size is available. Ability to change texture to checkered or off visually.  <br>

## Camera
**Camera Movement & Look Around:** User can explore scene based on Left-Right Click functionalities. User able to move camera via WASD movement commands. Camera controls are explained above in section *Controls*. <br>

**Camera Object:** User is able to see current camera location in scene via *Camera Inspector* Window. Scene monitors cameras position.  <br>

**Camera Settings:** Camera Speed Adjustment, and more available via *Camera Inspector* Window<br>

**Camera Perspective Zoom:** User can zoom by using scrollwheel.<br>

**Camera Orbitation:** Camera Orbitation implemented around selected Game Object in *Hierarchy* window.<br>

**Camera Reset:** Camera Reset to old parameters to ensure User Functionality.<br>

**Camera Frustum Culling:** Camera can be modified to enable Frustrum Culling, Depth buffering, Polygon smoothing & more! This is accessible in the *Settings* window, in the *Renderer* tab. <br>

## FPS Graph Window 
**Enable Vsync:** User can enable/disable vsync functionality. <br>

**Cap FPS:** User can cap the applications fps. <br>

## File Explorer Window 
**View Library** User can toggle through to view their library view dropdowns for the respective folders. Less visual clutter than the *Assets Window* <br>

## Settings Window 
### Display Tab
**Window: Adjust Sizing** User can adjust the applications size using sliders. <br>

**Window: Window Settings** User can adjust the applications to be fullscreen, bordered, or resizeable. <br>

**Framerate: Adjust FPS** User can cap the applications fps. <br>

**Framerate: Enable Vsync:** User can enable/disable vsync functionality. <br>


### Controls Tab
**Mouse Information**: User can see mouse position on screen and motion. <br>

### Renderer Tab
**Depth Test** User can enable depth buffering and testing. <br>

**Cull Face** User can enable cull faces. <br>

**Lighting** User can apply lighting to the scene, or toggle it off. <br>

**Line/Polygon Smoothin** User can smooth lines and polygons in the scene. <br>

**Color Material** User can adjust textures of all game objects. <br>

### Systems Tab
**Software Information** User can view latest information on compiled libraries <br>

**Hardware** User can view information on their systems hardware <br>

## Camera Debug
**View Information on Camera** Can view information information on camera look at Pos, position, and Rotation Matrix. <br>

# Features in Scene
## Display
**Display Clearing**: Display is cleared to show only the scene. All external windows are removed. <br>

## Game Object Movement Window
**Move Game Objects**: Game Objects can be moved around the scene when selected in this window on click. Controls to move the selected object is with WASD. <br>

## Dynamic Spatial Sound
**Spatial Audio**: Selected Game Objects have spatial sound attached to them. Listen as they move around the camera!<br>

## Background Audio
**Backing Track:** While in game mode listen for two backing tracks which loop every 30 seconds! <br>


## Developers

![](https://raw.githubusercontent.com/CITM-UPC/VertX-Game-Engine/main/TeamPhotos/adriapons.jpg)
 - [Adrià Pons Mensa](https://github.com/AdriaPm)
 
![](https://raw.githubusercontent.com/CITM-UPC/VertX-Game-Engine/main/TeamPhotos/rylangraham.jpg)
 - [Rylan Graham](https://github.com/RylanJGraham)

![](https://raw.githubusercontent.com/CITM-UPC/VertX-Game-Engine/main/TeamPhotos/joelchaves.jpg)
- [Joel Chaves](https://github.com/JoeyCM)

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed-source software. Check [LICENSE](https://mit-license.org/) for further details.
