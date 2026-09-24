#include "display_objects.h"
#include "math/linearmath.h"
#include "polygon.h"
#include "brush.h"


/*
  The render function replaces this:

  for(int i = 0; i < 6; i++) {
    for(int j = 0; j < 3; j++) {
      vec4 v4 = vec4_from_vec3(pyramid_polygons[i][j]);
      vec4 new_pos = Mul_mat4_vec4(Rotate, v4);
      pyramid_polygons[i][j] = vec3_from_vec4(new_pos);
    }
  }

  for(int i = 0; i < 6; i++) {
    vec2int snap_p0 = {.x = (int)round(pyramid_polygons[i][0].x), (int)round(pyramid_polygons[i][0].y)};
    vec2int snap_p1 = {.x = (int)round(pyramid_polygons[i][1].x), (int)round(pyramid_polygons[i][1].y)};
    vec2int snap_p2 = {.x = (int)round(pyramid_polygons[i][2].x), (int)round(pyramid_polygons[i][2].y)};
    Draw_Polygon(snap_p0, snap_p1, snap_p2, colors[i], WIREFRAME);
  }
*/

int initial = 0;
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

// int check = 0;

mat4 proj;
mat4 view;


void Init_Object_Renderer(int screen_width, int screen_height) {
  proj = Identity_mat4();
  view = Identity_mat4();
  SCREEN_WIDTH = screen_width;
  SCREEN_HEIGHT = screen_height;
}

void Set_ProjectionMatrix(mat4 m) {
  proj = m;
}

void Set_ViewMatrix(mat4 m) {
  view = m;
}

// renders DisplayObject
void Render(DisplayObject obj) {
  mat4 model;
  model = Mul_mat4(Rotate(obj.rotation), Scale(obj.scale));
  model = Mul_mat4(Translate(obj.position), model);
  // model matrix: will the vertices
  // [translate matrix] * [rotate matrix] * [scale matrix]
  // transformation should be applied from the right. so Scale -> Rotate -> Translate

  // for this part it isnt the model matrix (its the product of projection matrix, view matrix and model matrix)
  // but i think its better not taking extra memory and just write on the model matrix
  model = Mul_mat4(view, model);
  model = Mul_mat4(proj, model);

  int indices_length = obj.mesh->indices_length;
  for(int i = 0; i < indices_length; i++) {
    vec4 v4_0 = vec4_from_vec3(obj.mesh->vertices[obj.mesh->indices[3*i]].position);
    vec4 v4_1 = vec4_from_vec3(obj.mesh->vertices[obj.mesh->indices[3*i+1]].position);
    vec4 v4_2 = vec4_from_vec3(obj.mesh->vertices[obj.mesh->indices[3*i+2]].position);
    v4_0 = Mul_mat4_vec4(model, v4_0);
    v4_1 = Mul_mat4_vec4(model, v4_1);
    v4_2 = Mul_mat4_vec4(model, v4_2);

    if(v4_0.w != 0) {
      v4_0.x /= v4_0.w;
      v4_0.y /= v4_0.w;
      v4_0.z /= v4_0.w;
    }
    if(v4_1.w != 0) {
      v4_1.x /= v4_1.w;
      v4_1.y /= v4_1.w;
      v4_1.z /= v4_1.w;
    }
    if(v4_2.w != 0) {
      v4_2.x /= v4_2.w;
      v4_2.y /= v4_2.w;
      v4_2.z /= v4_2.w;
    }

    
    vec2int ip0 = {.x = (int)round(((v4_0.x + 1)) * SCREEN_WIDTH/2), .y = (int)round(((1 - v4_0.y)) * SCREEN_HEIGHT/2)};
    vec2int ip1 = {.x = (int)round(((v4_1.x + 1)) * SCREEN_WIDTH/2), .y = (int)round(((1 - v4_1.y)) * SCREEN_HEIGHT/2)};
    vec2int ip2 = {.x = (int)round(((v4_2.x + 1)) * SCREEN_WIDTH/2), .y = (int)round(((1 - v4_2.y)) * SCREEN_HEIGHT/2)};
    
    
    Draw_Polygon(ip0, ip1, ip2, obj.color, obj.mode);
    
    // bottom is the version using the depth buffer, abandoned

    // vec3 p0 = {.x = round(((v4_0.x + 1)) * SCREEN_WIDTH/2), .y = round(((1 - v4_0.y)) * SCREEN_HEIGHT/2), .z = v4_0.z};
    // vec3 p1 = {.x = round(((v4_1.x + 1)) * SCREEN_WIDTH/2), .y = round(((1 - v4_1.y)) * SCREEN_HEIGHT/2), .z = v4_1.z};
    // vec3 p2 = {.x = round(((v4_2.x + 1)) * SCREEN_WIDTH/2), .y = round(((1 - v4_2.y)) * SCREEN_HEIGHT/2), .z = v4_2.z};
    
    // Draw_Space_Polygon(p0, p1, p2, obj.color, obj.mode);
  }
}
