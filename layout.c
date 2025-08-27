#include "graph.h"

int layout_mode = 0;
int layout_running = 0;
int layout_iteration = 0;

void force_directed_layout(void) {
    if (!layout_running) {
        layout_running = 1;
        layout_iteration = 0;
        
        double area = 400.0 * 300.0;
        double k = sqrt(area / graph.node_count);
        
        for (int i = 0; i < graph.node_count; i++) {
            graph.nodes[i].x = (rand() / (double)RAND_MAX - 0.5) * 200.0;
            graph.nodes[i].y = (rand() / (double)RAND_MAX - 0.5) * 150.0;
            graph.nodes[i].vx = 0.0;
            graph.nodes[i].vy = 0.0;
        }
        
        printf("Starting force-directed layout (k=%.2f)...\n", k);
    }
    
    if (layout_iteration >= LAYOUT_ITERATIONS) {
        layout_running = 0;
        printf("Force-directed layout completed (%d iterations)\n", layout_iteration);
        return;
    }
    
    double area = 400.0 * 300.0;
    double k = sqrt(area / graph.node_count);
    double temperature = 100.0 * (1.0 - layout_iteration / (double)LAYOUT_ITERATIONS);
    
    for (int i = 0; i < graph.node_count; i++) {
        graph.nodes[i].vx = 0.0;
        graph.nodes[i].vy = 0.0;
    }
    
    for (int i = 0; i < graph.node_count; i++) {
        for (int j = i + 1; j < graph.node_count; j++) {
            double dx = graph.nodes[i].x - graph.nodes[j].x;
            double dy = graph.nodes[i].y - graph.nodes[j].y;
            double dist = sqrt(dx * dx + dy * dy);
            
            if (dist < 0.01) dist = 0.01;
            
            double force = k * k / dist;
            double fx = force * dx / dist;
            double fy = force * dy / dist;
            
            graph.nodes[i].vx += fx;
            graph.nodes[i].vy += fy;
            graph.nodes[j].vx -= fx;
            graph.nodes[j].vy -= fy;
        }
    }
    
    for (int e = 0; e < graph.edge_count; e++) {
        int i = graph.edges[e].from;
        int j = graph.edges[e].to;
        
        if (i == j) continue;
        
        double dx = graph.nodes[i].x - graph.nodes[j].x;
        double dy = graph.nodes[i].y - graph.nodes[j].y;
        double dist = sqrt(dx * dx + dy * dy);
        
        if (dist < 0.01) dist = 0.01;
        
        double force = dist * dist / k;
        double fx = force * dx / dist;
        double fy = force * dy / dist;
        
        graph.nodes[i].vx -= fx;
        graph.nodes[i].vy -= fy;
        graph.nodes[j].vx += fx;
        graph.nodes[j].vy += fy;
    }
    
    for (int i = 0; i < graph.node_count; i++) {
        double vel = sqrt(graph.nodes[i].vx * graph.nodes[i].vx + 
                         graph.nodes[i].vy * graph.nodes[i].vy);
        if (vel > temperature) {
            graph.nodes[i].vx = graph.nodes[i].vx / vel * temperature;
            graph.nodes[i].vy = graph.nodes[i].vy / vel * temperature;
        }
        
        graph.nodes[i].x += graph.nodes[i].vx;
        graph.nodes[i].y += graph.nodes[i].vy;
    }
    
    layout_iteration++;
    
    if (layout_iteration % 50 == 0) {
        printf("Layout progress: %d/%d iterations\n", layout_iteration, LAYOUT_ITERATIONS);
    }
}

void circular_layout(void) {
    double radius = fmin(200.0, 150.0);
    double angle_step = 2.0 * PI / graph.node_count;
    
    for (int i = 0; i < graph.node_count; i++) {
        double angle = i * angle_step;
        graph.nodes[i].x = radius * cos(angle);
        graph.nodes[i].y = radius * sin(angle);
        graph.nodes[i].vx = 0.0;
        graph.nodes[i].vy = 0.0;
    }
    
    layout_running = 0;
    printf("Applied circular layout\n");
}

void reset_layout(void) {
    extern double view_x, view_y, zoom;
    view_x = 0.0;
    view_y = 0.0;
    zoom = 1.0;
    layout_running = 0;
    
    if (graph.node_count > FORCE_LAYOUT_THRESHOLD) {
        circular_layout();
        layout_mode = 1;
    } else {
        layout_mode = 0;
        force_directed_layout();
    }
    
    printf("Reset view and layout\n");
} 