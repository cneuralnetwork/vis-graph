#include "graph.h"

int main(int argc, char **argv) {
    init_graph();
    
    if (!read_graph_input()) {
        return 1;
    }
    
    printf("\nControls:\n");
    printf("  L - Force layout, C - Circular layout, R - Reset\n");
    printf("  Mouse drag - Pan, Mouse wheel - Zoom, Esc - Exit\n\n");
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Graph Viewer");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keys);
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_motion);
    glutIdleFunc(idle);
    
    init_opengl();
    reset_layout();
    
    printf("Starting graph viewer...\n");
    glutMainLoop();
    
    return 0;
} 