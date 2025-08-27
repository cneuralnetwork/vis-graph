#include "graph.h"

double view_x = 0.0, view_y = 0.0;
double zoom = 1.0;
int window_width = WINDOW_WIDTH;
int window_height = WINDOW_HEIGHT;

void init_opengl(void) {
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(2.0f);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    double aspect = (double)window_width / window_height;
    double half_width = 300.0 / zoom;
    double half_height = half_width / aspect;
    
    glOrtho(view_x - half_width, view_x + half_width,
            view_y - half_height, view_y + half_height,
            -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    for (int i = 0; i < graph.edge_count; i++) {
        draw_edge(i);
    }
    
    for (int i = 0; i < graph.node_count; i++) {
        draw_node(i);
    }
    
    draw_legend();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    window_width = w;
    window_height = h;
    glViewport(0, 0, w, h);
}

void draw_node(int i) {
    double x = graph.nodes[i].x;
    double y = graph.nodes[i].y;
    double radius = graph.node_radius / zoom;
    
    glColor3f(0.3f, 0.6f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(x, y);
    for (int j = 0; j <= 32; j++) {
        double angle = j * 2.0 * PI / 32;
        glVertex2d(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
    
    glColor3f(0.1f, 0.3f, 0.7f);
    glBegin(GL_LINE_LOOP);
    for (int j = 0; j < 32; j++) {
        double angle = j * 2.0 * PI / 32;
        glVertex2d(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
    
    double number_scale = radius / 15.0;
    if (number_scale < 0.5) number_scale = 0.5;
    if (number_scale > 2.0) number_scale = 2.0;
    draw_number(x, y, i + 1, number_scale);
}

void draw_edge(int i) {
    int from = graph.edges[i].from;
    int to = graph.edges[i].to;
    
    double x1 = graph.nodes[from].x;
    double y1 = graph.nodes[from].y;
    double x2 = graph.nodes[to].x;
    double y2 = graph.nodes[to].y;
    
    if (from == to) {
        draw_self_loop(x1, y1, graph.node_radius / zoom);
        return;
    }
    
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glEnd();
    
    if (graph.is_directed) {
        draw_arrow(x1, y1, x2, y2, graph.node_radius / zoom);
    }
}

void draw_arrow(double x1, double y1, double x2, double y2, double radius) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double length = sqrt(dx * dx + dy * dy);
    
    if (length < 0.01) return;
    
    dx /= length;
    dy /= length;
    
    double arrow_x = x2 - dx * radius;
    double arrow_y = y2 - dy * radius;
    
    double arrow_size = ARROW_SIZE / zoom;
    
    double perp_x = -dy;
    double perp_y = dx;
    
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_TRIANGLES);
    glVertex2d(arrow_x, arrow_y);
    glVertex2d(arrow_x - dx * arrow_size + perp_x * arrow_size * 0.5,
               arrow_y - dy * arrow_size + perp_y * arrow_size * 0.5);
    glVertex2d(arrow_x - dx * arrow_size - perp_x * arrow_size * 0.5,
               arrow_y - dy * arrow_size - perp_y * arrow_size * 0.5);
    glEnd();
}

void draw_self_loop(double x, double y, double node_radius) {
    double loop_radius = SELF_LOOP_RADIUS / zoom;
    double center_x = x + node_radius + loop_radius;
    double center_y = y;
    
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 32; i++) {
        double angle = i * 2.0 * PI / 32;
        glVertex2d(center_x + loop_radius * cos(angle),
                   center_y + loop_radius * sin(angle));
    }
    glEnd();
    
    if (graph.is_directed) {
        double arrow_x = center_x + loop_radius;
        double arrow_y = center_y;
        double arrow_size = ARROW_SIZE / zoom;
        
        glBegin(GL_TRIANGLES);
        glVertex2d(arrow_x, arrow_y);
        glVertex2d(arrow_x - arrow_size, arrow_y + arrow_size * 0.5);
        glVertex2d(arrow_x - arrow_size, arrow_y - arrow_size * 0.5);
        glEnd();
    }
}

void draw_legend(void) {
}

void draw_number(double x, double y, int number, double scale) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f * scale);
    
    static const int patterns[10] = {
        0b1111110, 0b0110000, 0b1101101, 0b1111001, 0b0110011,
        0b1011011, 0b1011111, 0b1110000, 0b1111111, 0b1111011
    };
    
    double size = 8.0 * scale;
    double half_width = size * 0.4;
    double half_height = size * 0.6;
    
    char num_str[16];
    sprintf(num_str, "%d", number);
    int len = strlen(num_str);
    double total_width = len * size * 0.8;
    double start_x = x - total_width / 2.0;
    
    for (int digit_pos = 0; digit_pos < len; digit_pos++) {
        int digit = num_str[digit_pos] - '0';
        if (digit < 0 || digit > 9) continue;
        
        double digit_x = start_x + digit_pos * size * 0.8;
        int pattern = patterns[digit];
        
        glBegin(GL_LINES);
        
        if (pattern & 0b1000000) {
            glVertex2d(digit_x - half_width, y + half_height);
            glVertex2d(digit_x + half_width, y + half_height);
        }
        
        if (pattern & 0b0100000) {
            glVertex2d(digit_x + half_width, y + half_height);
            glVertex2d(digit_x + half_width, y);
        }
        
        if (pattern & 0b0010000) {
            glVertex2d(digit_x + half_width, y);
            glVertex2d(digit_x + half_width, y - half_height);
        }
        
        if (pattern & 0b0001000) {
            glVertex2d(digit_x - half_width, y - half_height);
            glVertex2d(digit_x + half_width, y - half_height);
        }
        
        if (pattern & 0b0000100) {
            glVertex2d(digit_x - half_width, y);
            glVertex2d(digit_x - half_width, y - half_height);
        }
        
        if (pattern & 0b0000010) {
            glVertex2d(digit_x - half_width, y + half_height);
            glVertex2d(digit_x - half_width, y);
        }
        
        if (pattern & 0b0000001) {
            glVertex2d(digit_x - half_width, y);
            glVertex2d(digit_x + half_width, y);
        }
        
        glEnd();
    }
    
    glLineWidth(2.0f);
}

void screen_to_world(int screen_x, int screen_y, double *world_x, double *world_y) {
    double aspect = (double)window_width / window_height;
    double half_width = 300.0 / zoom;
    double half_height = half_width / aspect;
    
    *world_x = view_x + (screen_x - window_width / 2.0) * 2.0 * half_width / window_width;
    *world_y = view_y + (window_height / 2.0 - screen_y) * 2.0 * half_height / window_height;
}

void world_to_screen(double world_x, double world_y, int *screen_x, int *screen_y) {
    double aspect = (double)window_width / window_height;
    double half_width = 300.0 / zoom;
    double half_height = half_width / aspect;
    
    *screen_x = (int)((world_x - view_x) * window_width / (2.0 * half_width) + window_width / 2.0);
    *screen_y = (int)(window_height / 2.0 - (world_y - view_y) * window_height / (2.0 * half_height));
}

double distance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
} 