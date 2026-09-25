CC = gcc
CFLAGS = -lm -O3

src_files = src/main.c src/rasterizer.c  src/polygon.c src/brush.c src/display_object.c src/mesh.c src/depth_buffer.c src/obj_reader.c

runprogram: clean
		sudo ./main $(ARGS)

clean: compile
		gcc src/clear.c -o clear
		sudo ./clear

compile:
		$(CC) $(src_files) $(CFLAGS) -o main



