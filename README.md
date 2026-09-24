# small3d
`small3d` is a software 3D object renderer using zero libraries. Using no graphical libraries nor mathematical libraries. 

**Highlights**: 
* **Zero libraries:** in the age of AI-slop, this project consists of 100% hand-written C code using no external graphical libraries or math libraries.
* **Linear algebra library:** math library contains the linear algebra needed for 3D graphics and implements the basic definitions of vectors and matrices to transformations such as rotation in three axes.
* **Made portable:** the original `small3d` works in the virtual terminal by writing on the framebuffer but is also ported to use SDL2.
* **Display any object:** Can Import .obj files to display.
* **Easy to configure:** object-based like coordinate system with easily configurable Position/Rotation/Scale

## Demo

<video src="https://github.com/user-attachments/assets/56d639bb-c4ed-4ca8-9db4-c3b50b5d59ee"></video>
<video src="https://github.com/user-attachments/assets/5803c1dc-b75a-413f-b2f3-068b9e8f1bdd"></video>
<video src="https://github.com/user-attachments/assets/c77f6298-1a81-4734-822b-711251023a04"></video>

## Known Errors and Unfinished Business
* **Object not loading** sometimes the objects doesn't load and only a pixel rotating can be visible. When that happens try turning it on and off again, no need to recompile it.
* **Unfinished Depth Buffer:** A depth buffer was in progress but failed to make it in time.
* **Only Triangulated .obj files work** only .obj files that it's faces are cut into triangles can be displayed (Ctrl+t in blender while selecting faces). Attempts were made to make a Triangulator but failed and I got sick in the progress.

<video src="https://github.com/user-attachments/assets/06da2e44-47fb-40a5-a891-68d8c6eb4145"></video>

## Compile

This program is only tested and intended to work on Linux.  
it is compiled via `gcc` and SLD2 is required for the SDL2 version
### compile and run program
```bash
  make
  ./main
```
