#include <stdio.h>
#include <stdlib.h>
#include "../math/vec3.h"
#include "../mesh.h"

#define BUFFER_LENGTH 1024

void Print_Mesh(Mesh m) {
  printf("contains:\n");
  printf("vertex: %d\n", m.vertices_length);
  printf("face: %d\n", m.indices_length);
}

char* test_line = "f 111/64/33 110/127/33 109/125/33 108/123/33 107/121/33 106/119/33 105/117/33 104/115/33 103/113/33 102/111/33 101/109/33 100/107/33 99/105/33 97/102/33 98/101/33 128/161/33 127/159/33 126/157/33 125/155/33 124/153/33 123/151/33 122/149/33 121/147/33 120/145/33 119/143/33 118/141/33 117/139/33 116/137/33 115/135/33 114/133/33 113/131/33 112/63/33\0";

int f_nonTess_count = 0;

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
      f_nonTess_count++;
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
      m.indices_length+= f_number;
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

  return m;

  fclose(input);
}

int main() {
  // printf("Mug tesselated\n");
  // Mesh mesh1 = OBJ_Parse("../../import/ATessellatedMug.obj");
  printf("Mug not tesselated\n");
  Mesh mesh2 = OBJ_Parse("../../import/ATerriblyModeledMug.obj");

  printf("\nnon tesselated face count %d\n", f_nonTess_count);

  printf("\n\n");
  for(int i = 0; i < mesh2.vertices_length; i++) {
    printf("%f %f %f\n", mesh2.vertices[i].position.x, mesh2.vertices[i].position.y, mesh2.vertices[i].position.z);
  }
  for(int i = 0; i < mesh2.indices_length; i++) {
    printf("%d %d %d\n", mesh2.indices[3*i], mesh2.indices[3*i+1], mesh2.indices[3*i+2]);
  }
  // printf("Hexagon\n");
  // Mesh mesh3 = OBJ_Parse("../../import/Hexagon.obj");

  // printf("cube tesselated\n");
  // Mesh mesh4 = OBJ_Parse("../../import/CubeTessalated.obj");
  // printf("cube not tesselated\n");
  // Mesh mesh5 = OBJ_Parse("../../import/Cube.obj");

  // printf("32 edge circle Tessalated\n");
  // Mesh mesh6 = OBJ_Parse("../../import/32edgeTesselated.obj");
  // printf("32 edge circle not Tessalated\n");
  // Mesh mesh7 = OBJ_Parse("../../import/32edge.obj");



  // printf("mesh vertices_length : %d\n", mesh.vertices_length);
  // printf("mesh indices_length : %d\n", mesh.indices_length);

  // printf("vertex 1 to 3\n");
  // Print_vec3(mesh.vertices[0].position);
  // Print_vec3(mesh.vertices[1].position);
  // Print_vec3(mesh.vertices[2].position);
  // printf("most last 3 vertices\n");
  // Print_vec3(mesh.vertices[mesh.vertices_length-3].position);
  // Print_vec3(mesh.vertices[mesh.vertices_length-2].position);
  // Print_vec3(mesh.vertices[mesh.vertices_length-1].position);
  // printf("\n");
  // printf("face 1 to 3\n");
  // printf("%d %d %d\n", mesh.indices[0], mesh.indices[1], mesh.indices[2]);
  // printf("%d %d %d\n", mesh.indices[3], mesh.indices[4], mesh.indices[5]);
  // printf("%d %d %d\n", mesh.indices[6], mesh.indices[7], mesh.indices[8]);
  // printf("most last 3 faces\n");
  // printf("%d %d %d\n", mesh.indices[mesh.indices_length*3-9], mesh.indices[mesh.indices_length*3-8], mesh.indices[mesh.indices_length*3-7]);
  // printf("%d %d %d\n", mesh.indices[mesh.indices_length*3-6], mesh.indices[mesh.indices_length*3-5], mesh.indices[mesh.indices_length*3-4]);
  // printf("%d %d %d\n", mesh.indices[mesh.indices_length*3-3], mesh.indices[mesh.indices_length*3-2], mesh.indices[mesh.indices_length*3-1]);

  /*

  int face_length = 0;
  int *this_face = NULL;
  int this_vert = -1;
  int f_count = 0;

  char *string = test_line;
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
      // printf("face indides \n");
      // for(int i = 0; i < face_length; i++) {
      //   printf("%d ", this_face[i]);
      // }
      // printf("\n");
    
      // number of face information from this ine
      int f_number = face_length-2;
      int i0, i1, i2;
      int v0, v1, v2;
      i0 = 0;
      i1 = 1;
      i2 = 2;
      v0 = this_face[i0];
      v1 = this_face[i1];
      v2 = this_face[i2];
      
      for(int i = 0; i < f_number; i++) {
        // printf("%d %d %d\n", v0, v1, v2);
        v1 = this_face[++i1];
        v2 = this_face[++i2];
      }
      f_count += f_number;
    }
    free(this_face);
    this_face = NULL;

  printf("fcount %d\n", f_count);
  */
  return 0;
}