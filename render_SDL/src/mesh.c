#include "mesh.h"
#include "math/vec3.h"
#include <math.h>

void Print_Mesh(Mesh mesh) {
  printf("mesh vertices_length : %d\n", mesh.vertices_length);
  printf("mesh indices_length : %d\n", mesh.indices_length);

  printf("vertex 1 to 3\n");
  Print_vec3(mesh.vertices[0].position);
  Print_vec3(mesh.vertices[1].position);
  Print_vec3(mesh.vertices[2].position);
  printf("most last 3 vertices\n");
  Print_vec3(mesh.vertices[mesh.vertices_length-3].position);
  Print_vec3(mesh.vertices[mesh.vertices_length-2].position);
  Print_vec3(mesh.vertices[mesh.vertices_length-1].position);
  printf("\n");
  printf("face 1 to 3\n");
  printf("%d %d %d\n", mesh.indices[0], mesh.indices[1], mesh.indices[2]);
  printf("%d %d %d\n", mesh.indices[3], mesh.indices[4], mesh.indices[5]);
  printf("%d %d %d\n", mesh.indices[6], mesh.indices[7], mesh.indices[8]);
  printf("most last 3 faces\n");
  printf("%d %d %d\n", mesh.indices[mesh.indices_length*3-9], mesh.indices[mesh.indices_length*3-8], mesh.indices[mesh.indices_length*3-7]);
  printf("%d %d %d\n", mesh.indices[mesh.indices_length*3-6], mesh.indices[mesh.indices_length*3-5], mesh.indices[mesh.indices_length*3-4]);
  printf("%d %d %d\n", mesh.indices[mesh.indices_length*3-3], mesh.indices[mesh.indices_length*3-2], mesh.indices[mesh.indices_length*3-1]);
}

void Print_WHOLE_Vert(Mesh* mesh) {
  printf("vertices_length %d\n", mesh->vertices_length);
  printf("vertexes\n");
  for(int i = 0; i < mesh->vertices_length; i++) {
    printf("%d : %f, %f, %f\n", i,  mesh->vertices[i].position.x, mesh->vertices[i].position.y, mesh->vertices[i].position.z);
  }
}

void Print_WHOLE_Indc(Mesh* mesh) {
  printf("indices_length %d\n",mesh->indices_length);
  printf("indices\n");
  for(int i = 0; i < mesh->indices_length; i++) {
    printf("%d : %d, %d, %d\n", i, mesh->indices[3*i], mesh->indices[3*i+1], mesh->indices[3*i+2]);
  }
}

// center the mesh in local space
void Normal_Mesh(Mesh* mesh) {
  // equivillant to blenders "set origin to geomery" and moving object to (0, 0, 0)

  float maxX = -INFINITY, maxY = -INFINITY, maxZ = -INFINITY;
  float minX = INFINITY, minY = INFINITY, minZ = INFINITY;
  
  for(int i = 0; i < mesh->vertices_length; i++) {
    if(mesh->vertices[i].position.x > maxX) maxX = mesh->vertices[i].position.x;
    if(mesh->vertices[i].position.y > maxY) maxY = mesh->vertices[i].position.y;
    if(mesh->vertices[i].position.z > maxZ) maxZ = mesh->vertices[i].position.z;

    if(mesh->vertices[i].position.x < minX) minX = mesh->vertices[i].position.x;
    if(mesh->vertices[i].position.y < minY) minY = mesh->vertices[i].position.y;
    if(mesh->vertices[i].position.z < minZ) minZ = mesh->vertices[i].position.z;
  }

  float side_x = maxX - minX;
  float side_y = maxY - minY;
  float side_z = maxZ - minZ;

  // longest side of the box
  float bounding_box_length = (side_x > side_y && side_x > side_z) ? side_x : ((side_y > side_z) ? side_y : side_z);
  

  // shift whole thing by its centre of bounding box

  vec3 offset = {.x = (maxX + minX)/2, .y = (maxY + minY)/2, .z = (maxZ + minZ)/2};

  for(int i = 0; i < mesh->vertices_length; i++) {
    mesh->vertices[i].position = Sub_vec3(mesh->vertices[i].position, offset);
    mesh->vertices[i].position = Times_vec3(mesh->vertices[i].position, 1/bounding_box_length);
  }

}

void Clear_Mesh(Mesh *m) {
  if(m == NULL) return;

  free(m->vertices);
  free(m->indices);
  free(m);
}
