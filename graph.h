#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#define MAX_NODES 1000
#define MAX_EDGES 10000
#define MAX_LINE 1024
#define PI 3.14159265358979323846
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MIN_NODE_RADIUS 8.0
#define MAX_NODE_RADIUS 25.0
#define ARROW_SIZE 15.0
#define SELF_LOOP_RADIUS 20.0
#define LAYOUT_ITERATIONS 500
#define FORCE_LAYOUT_THRESHOLD 200

typedef struct {
    double x, y;
    double vx, vy;
    int degree;
} Node;

typedef struct {
    int from, to;
    int is_directed;
} Edge;

typedef struct {
    Node nodes[MAX_NODES];
    Edge edges[MAX_EDGES];
    int node_count;
    int edge_count;
    int is_directed;
    double node_radius;
} Graph;

extern Graph graph;
extern double view_x, view_y;
extern double zoom;
extern int window_width, window_height;
extern int mouse_x, mouse_y;
extern int mouse_dragging;
extern int layout_mode;
extern int layout_running;
extern int layout_iteration;

void init_graph(void);
int read_graph_input(void);
int parse_adjacency_list_format(FILE *fp);
void add_edge(int from, int to);
int has_edge(int from, int to);
void calculate_node_degrees(void);

void force_directed_layout(void);
void circular_layout(void);
void reset_layout(void);

void init_opengl(void);
void display(void);
void reshape(int w, int h);
void draw_node(int i);
void draw_edge(int i);
void draw_arrow(double x1, double y1, double x2, double y2, double radius);
void draw_self_loop(double x, double y, double radius);
void draw_legend(void);
void draw_number(double x, double y, int number, double scale);

void keyboard(unsigned char key, int x, int y);
void special_keys(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouse_motion(int x, int y);
void idle(void);

void screen_to_world(int screen_x, int screen_y, double *world_x, double *world_y);
void world_to_screen(double world_x, double world_y, int *screen_x, int *screen_y);
double distance(double x1, double y1, double x2, double y2);
void cleanup(void);

#endif 