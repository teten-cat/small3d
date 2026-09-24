

# small3d


`small3d` is a software 3D object renderer using zero libraries. Using no graphical libraries nor mathematical libraries. 

**Highlights**: 
* **Zero libraries:** in the age of AI-slop, this project consits of 100% hand-written C code using no external graphical libraries or math libraries.
* **Linear algebra libary:** math library contains the linear algebra needed for 3D graphics and implements the basic definitions of vectors and matrices to transformations such as rotation in three axes.
* **Made portable:** the original `small3d` works in the virtual terminal by writing on the framebuffer but is also ported to use SDL2.
* **Display any object:** Can Import .obj files to display.
* **Easy to configure:** object-based like coordinate system with easily configureable Position/Rotation/Scale




## Demo

<video src="https://github.com/teten-cat/small3d/showcase/mug.mp4"></video>
<video src="https://github.com/teten-cat/small3d/showcase/house.mp4"></video>
<video src="https://github.com/teten-cat/small3d/showcase/shapes.mp4"></video>

## Known Errors and Unfinished Business
* **Object not loading** sometimes the objects doesn't load and only a pixel rotating can be visible. When that happens try turning it on and off again, no need to recompile it.
* **Unfinished Depth Buffer:** A depth buffer was in progress but failed to make it in time.
* **Only Triangulated .obj files work** only .obj files that it's faces are cut into triangles can be displayed (Ctrl+t in blender while selecting faces). Attemps were made to make a Triangulator but faled and I got sick in the progress.

<video src="https://github.com/teten-cat/small3d/showcase/broken_depth.mp4"></video>

## Compile

This program is only tested and intended to work on Linux.  
it is compiled via `gcc` and SLD2 is required for the SDL2 version
### compile and run program
```bash
  make
  ./main
```
