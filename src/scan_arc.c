#define _USE_MATH_DEFINES
#include "scan_arc.h"
#include <graffiks/mesh/mesh.h>
#include <graffiks/material/material.h>
#include <math.h>

mesh *_create_scan_mesh(int degrees, int scan_range) {
  int i, j;

  // create vertices for tris in 1.4 degree steps
  float **verts = malloc(sizeof(float *) * (2 + degrees));

  for (i = 0; i < degrees + 2; i++) {
    verts[i] = malloc(sizeof(float) * 3);

    if (i == 0) { // center vertex
      verts[i][0] = 0;
      verts[i][1] = 0;
      verts[i][2] = 0;
      continue;
    }

    double angle_radians = ((i - 1) - 64) * M_PI / 128;
    verts[i][0] = ((float)floor(0.5 + (scan_range * cos(angle_radians)))) / 200;
    verts[i][1] = ((float)floor(0.5 + (scan_range * sin(angle_radians)))) / 200;
    verts[i][2] = 0;
  }

  // faces
  int ***faces = malloc(sizeof(int **) * degrees);
  for (i = 0; i < degrees; i++) {
    faces[i] = malloc(sizeof(int *) * 3);

    for (j = 0; j < 3; j++) {
      faces[i][j] = malloc(sizeof(int) * 3);

      if (j == 0) { // vertex == 0
        faces[i][j][0] = 0;
        faces[i][j][1] = 0;
        faces[i][j][2] = 0;
      } else if (j == 1) { // vertex == i+1
        faces[i][j][0] = i + 1;
        faces[i][j][1] = 0;
        faces[i][j][2] = 0;
      } else if (j == 2) { // vertex = i+2
        faces[i][j][0] = i + 2;
        faces[i][j][1] = 0;
        faces[i][j][2] = 0;
      }
    }
  }

  // normals
  float **normals = malloc(sizeof(float *) * 1);
  normals[0] = malloc(sizeof(float) * 3);
  normals[0][0] = 0;
  normals[0][1] = 0;
  normals[0][2] = 1;

  mesh *mesh = create_mesh(verts, faces, degrees, normals);

  // free verts
  for (i = 0; i < degrees + 2; i++) {
    free(verts[i]);
  }
  free(verts);

  // free faces
  for (i = 0; i < degrees; i++) {
    for (j = 0; j < 3; j++) {
      free(faces[i][j]);
    }
    free(faces[i]);
  }
  free(faces);

  // free normals
  free(normals[0]);
  free(normals);

  return mesh;
}

object *create_scan_arc(renderer_flags flags, int degrees, int scan_range) {
  mesh *mesha = _create_scan_mesh(degrees, scan_range);
  material *mata = create_material(flags);
  float color[4] = {0.1, 0.1, 0.1, 1};
  set_diffuse_color(mata, color);

  mesh **mesh2 = NULL;
  mesh2 = malloc(sizeof(mesh *));
  mesh2[0] = mesha;

  material **mat2 = NULL;
  mat2 = malloc(sizeof(material *));
  mat2[0] = mata;

  object *o = create_object(mesh2, mat2, 1);
  return o;
}

void update_scan_arc(object *o, int degrees, int scan_range) {
  hide_object(o);
  free_mesh(o->meshes[0]);
  o->meshes[0] = _create_scan_mesh(degrees, scan_range);
  show_object(o);
}
