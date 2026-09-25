#include <stdio.h>
#include <stdlib.h>
#include "math/vec3.h"
#include "mesh.h"

#define BUFFER_LENGTH 1024



// this wont work for non-tessellated/triangulated obj files
// i tried to make it compatible but had enough and got sick for 2 weeks in the progress
// the edge cases of two vertex being on the same place just messed me up and i had enough

// please triangulate your object in a 3D modeling software (its only Ctrl+T in blender)

Mesh OBJ_Parse(char *filename) {
  
  Mesh m;
  
  FILE *input;
  input = fopen(filename, "r");
  
  if(input == NULL) {
    printf("failed to open file\n");
    return m;
  }
  
  char string[BUFFER_LENGTH];
  float vx, vy, vz;
  int fx, fy, fz;
  int v_count = 0;
  int f_count = 0;

  while(fgets(string, BUFFER_LENGTH, input) != NULL) {
    if(string[0] == 'v' && string[1] == ' ') {
      v_count++;
    }
    // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
    // f 75/21/10   76/23/10   23/22/10
  }

  // printf("first check: %d vertices\n", v_count);
  // printf("first check: %d faces\n", f_count);

  m.vertices_length = v_count;
  m.indices_length = 0;
  m.vertices = malloc(sizeof(Vertex) * v_count);
  m.indices = NULL;
  rewind(input); 
  v_count = 0;
  f_count = 0;


  // temp variables used while working on faces
  int face_length = 0;
  int *this_face = NULL;
  int this_vert = -1;

  int indexes_saved = 0;
  int sum_counting = 0;

  while(fgets(string, BUFFER_LENGTH, input) != NULL) {
    char* pointer = string;
    if(sscanf(string, "v %f %f %f", &vx, &vy, &vz) == 3) {
      // printf("v %f %f %f\n", vx, vy, vz);
      vec3 v = {.x = vx, .y = vy, .z = vz};
      m.vertices[v_count] = (Vertex){.position = v};

      v_count++;
    }

    // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
    // f 75/21/10   76/23/10   23/22/10

    // example: f 147/181/37 146/164/37 15/164/37 135/199/37 136/196/37
    //  this gets faces using the ears method. optimal is Blenders "Beauty" method but too complex for now
    // 147-146-15 147-15-135 147-135-136
    // in this example 5 vertexes get 3 faces: n vertex -> n-2 faces
    if(string[0] == 'f' && string[1] == ' ') {
      face_length = 0;  // reset
      for(int i = 1; string[i] != '\0'; i++) {
        // every vertex index is after a whitespace
        if(string[i] == ' ' && atoi(&string[i+1]) != 0) {
          if(sscanf(&string[i+1], "%d", &this_vert) == 1) {
            face_length++;
            this_face = (int *)realloc(this_face, face_length * sizeof(int));

            this_face[face_length-1] = this_vert;
          }
        }
      }
      // this_face should have all the vertexes(not the normal or texture vertexes)
    
      // number of face information from this ine
      int f_number = face_length-2;
      m.indices_length += f_number;
      int i0, i1, i2; // index for each vertex of face in this_face
      int v0, v1, v2; // vertex
      i0 = 0; 
      i1 = 1;
      i2 = 2;
      v0 = this_face[i0];
      v1 = this_face[i1];
      v2 = this_face[i2];
      
      for(int i = 0; i < f_number; i++) {
        m.indices = (int *)realloc(m.indices, (indexes_saved+3) * sizeof(int));

        // printf("%d %d %d\n", v0, v1, v2);
        v1 = this_face[i1++];
        v2 = this_face[i2++];
        // -1 to make it 0 based indexing (.obj works on 1 based indexing)
        m.indices[indexes_saved] = v0-1;
        m.indices[indexes_saved+1] = v1-1;
        m.indices[indexes_saved+2] = v2-1;
        indexes_saved += 3;
      }

      f_count += f_number;
    }
    free(this_face);
    this_face = NULL;
  }

  m.indices_length = f_count;

  // printf("second check: %d vertices\n", v_count);
  // printf("second check: %d faces\n", f_count);

  fclose(input);
  
  return m;
}

