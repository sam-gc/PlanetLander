#ifndef CAMERA_H
#define CAMERA_H

#include "tools/linmath.h"

typedef struct {
    double x;
    double y;
    double w;
    double h;

    double max_w;
    double max_h;

    double scale;
    mat4x4 matrix;
} Camera;

Camera cam_make();
void cam_pan(Camera *cam, double x, double y);
void cam_center_on(Camera *cam, double x, double y);
void cam_set_zoom(Camera *cam, double zoom);
void cam_prepare_matrix(Camera *cam);

#endif
