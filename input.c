#include "graph.h"

int mouse_x, mouse_y;
int mouse_dragging = 0;

void keyboard(unsigned char key, int x, int y) {
    extern int layout_mode;
    extern double zoom;
    
    switch (key) {
        case 27:
            cleanup();
            exit(0);
            break;
            
        case 'l':
        case 'L':
            layout_mode = 0;
            force_directed_layout();
            break;
            
        case 'c':
        case 'C':
            layout_mode = 1;
            circular_layout();
            break;
            
        case 'r':
        case 'R':
            reset_layout();
            break;
            
        case '+':
        case '=':
            zoom *= 1.2;
            if (zoom > 10.0) zoom = 10.0;
            break;
            
        case '-':
        case '_':
            zoom /= 1.2;
            if (zoom < 0.1) zoom = 0.1;
            break;
    }
    
    glutPostRedisplay();
}

void special_keys(int key, int x, int y) {
    extern double view_x, view_y, zoom;
    double pan_speed = 20.0 / zoom;
    
    switch (key) {
        case GLUT_KEY_LEFT:
            view_x -= pan_speed;
            break;
        case GLUT_KEY_RIGHT:
            view_x += pan_speed;
            break;
        case GLUT_KEY_UP:
            view_y += pan_speed;
            break;
        case GLUT_KEY_DOWN:
            view_y -= pan_speed;
            break;
    }
    
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    extern double zoom;
    
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouse_dragging = 1;
            mouse_x = x;
            mouse_y = y;
        } else {
            mouse_dragging = 0;
        }
    } else if (button == 3) {
        zoom *= 1.1;
        if (zoom > 10.0) zoom = 10.0;
        glutPostRedisplay();
    } else if (button == 4) {
        zoom /= 1.1;
        if (zoom < 0.1) zoom = 0.1;
        glutPostRedisplay();
    }
}

void mouse_motion(int x, int y) {
    extern double view_x, view_y, zoom;
    
    if (mouse_dragging) {
        double dx = (x - mouse_x) * 0.5 / zoom;
        double dy = (mouse_y - y) * 0.5 / zoom;
        
        view_x -= dx;
        view_y -= dy;
        
        mouse_x = x;
        mouse_y = y;
        
        glutPostRedisplay();
    }
}

void idle(void) {
    extern int layout_running, layout_mode;
    
    if (layout_running && layout_mode == 0) {
        force_directed_layout();
        glutPostRedisplay();
    }
}

void cleanup(void) {
    printf("\nExiting...\n");
} 