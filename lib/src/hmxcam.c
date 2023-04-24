#include "hmxcam.h"
#include "hmxdraw.h"
#include "hmxobj.h"
#include "hmxprimitive.h"
#include "hmxstring.h"
#include "hmxtransform.h"
#include "iohelper.h"
#include <stdio.h>

HX_CAM hmx_cam_load(FILE *file) {
    HX_CAM cam;
    cam.version = iohelper_readu32(file);
    cam.trans = hmx_transform_load(file);
    cam.draw = hmx_draw_load(file);
    cam.near_plane = iohelper_readf(file);
    cam.far_plane = iohelper_readf(file);
    cam.y_fov = iohelper_readf(file);
    cam.screen_rect = hmx_primitive_rect_load(file);
    fread(&cam.z_range, sizeof(Vector2f), 1, file);
    cam.target_tex = hmx_string_load(file);
    return cam;
}
void hmx_cam_print(HX_CAM cam) {
    printf("VERSION: %i\n", cam.version);
    printf("BEGIN TRANS\n"); hmx_transform_print(cam.trans); printf("END TRANS\n");
    printf("BEGIN DRAW\n"); hmx_draw_print(cam.draw); printf("\nEND DRAW\n");
    printf("NEAR PLANE: %f\n", cam.near_plane);
    printf("FAR PLANE: %f\n", cam.far_plane);
    printf("Y FOV: %f\n", cam.y_fov);
    printf("SCREEN RECT: "); hmx_primitive_rect_print(cam.screen_rect);
    printf("\nZ RANGE: %f %f\n", cam.z_range.x, cam.z_range.y);
    printf("TARGET TEX: "); hmx_string_print(cam.target_tex);
    printf("\n");
}