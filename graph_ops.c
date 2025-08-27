#include "graph.h"

Graph graph;

void init_graph(void) {
    memset(&graph, 0, sizeof(Graph));
    graph.node_radius = MIN_NODE_RADIUS;
    srand(time(NULL));
}

int read_graph_input(void) {
    char filename[256];
    char directed_input[10];
    FILE *fp = stdin;
    
    printf("Graph Viewer - Adjacency List Format Only\n");
    printf("=========================================\n\n");
    
    printf("Enter input filename (or press Enter to use stdin): ");
    fflush(stdout);
    if (fgets(filename, sizeof(filename), stdin) != NULL) {
        filename[strcspn(filename, "\n")] = 0;
        
        if (strlen(filename) > 0) {
            fp = fopen(filename, "r");
            if (!fp) {
                fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
                return 0;
            }
        }
    }
    
    printf("Is the graph directed? (y/n): ");
    fflush(stdout);
    if (fgets(directed_input, sizeof(directed_input), stdin) != NULL) {
        graph.is_directed = (directed_input[0] == 'y' || directed_input[0] == 'Y');
    }
    
    printf("\nReading adjacency list data...\n");
    
    if (!parse_adjacency_list_format(fp)) {
        fprintf(stderr, "Error: Invalid adjacency list format\n");
        if (fp != stdin) fclose(fp);
        return 0;
    }
    
    if (fp != stdin) {
        fclose(fp);
    }
    
    calculate_node_degrees();
    
    if (graph.node_count > 50) {
        graph.node_radius = MIN_NODE_RADIUS;
    } else if (graph.node_count > 20) {
        graph.node_radius = MIN_NODE_RADIUS + 5.0;
    } else {
        graph.node_radius = MAX_NODE_RADIUS;
    }
    
    printf("Loaded graph: %d nodes, %d edges (%s)\n", 
           graph.node_count, graph.edge_count, 
           graph.is_directed ? "directed" : "undirected");
    
    return 1;
}

int parse_adjacency_list_format(FILE *fp) {
    char line[MAX_LINE];
    int max_node = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        if (strlen(line) <= 1) continue;
        
        char *colon = strchr(line, ':');
        if (!colon) continue;
        
        int u = atoi(line);
        if (u > max_node) max_node = u;
        
        char *token = strtok(colon + 1, " \t\n");
        while (token) {
            int v = atoi(token);
            if (v > max_node) max_node = v;
            token = strtok(NULL, " \t\n");
        }
    }
    
    if (max_node <= 0 || max_node > MAX_NODES) {
        return 0;
    }
    
    graph.node_count = max_node;
    
    for (int i = 0; i < graph.node_count; i++) {
        graph.nodes[i].x = 0.0;
        graph.nodes[i].y = 0.0;
        graph.nodes[i].vx = 0.0;
        graph.nodes[i].vy = 0.0;
        graph.nodes[i].degree = 0;
    }
    
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        if (strlen(line) <= 1) continue;
        
        char *colon = strchr(line, ':');
        if (!colon) continue;
        
        int u = atoi(line) - 1;
        if (u < 0 || u >= graph.node_count) continue;
        
        char *token = strtok(colon + 1, " \t\n");
        while (token) {
            int v = atoi(token) - 1;
            if (v >= 0 && v < graph.node_count) {
                add_edge(u, v);
            }
            token = strtok(NULL, " \t\n");
        }
    }
    
    return 1;
}

void add_edge(int from, int to) {
    if (graph.edge_count >= MAX_EDGES) {
        fprintf(stderr, "Warning: Maximum edge count exceeded\n");
        return;
    }
    
    if (!graph.is_directed && from != to) {
        if (has_edge(from, to) || has_edge(to, from)) {
            return;
        }
    } else if (graph.is_directed && has_edge(from, to)) {
        return;
    }
    
    graph.edges[graph.edge_count].from = from;
    graph.edges[graph.edge_count].to = to;
    graph.edges[graph.edge_count].is_directed = graph.is_directed;
    graph.edge_count++;
}

int has_edge(int from, int to) {
    for (int i = 0; i < graph.edge_count; i++) {
        if (graph.edges[i].from == from && graph.edges[i].to == to) {
            return 1;
        }
    }
    return 0;
}

void calculate_node_degrees(void) {
    for (int i = 0; i < graph.node_count; i++) {
        graph.nodes[i].degree = 0;
    }
    
    for (int i = 0; i < graph.edge_count; i++) {
        int from = graph.edges[i].from;
        int to = graph.edges[i].to;
        
        if (from == to) {
            graph.nodes[from].degree += 2;
        } else if (graph.is_directed) {
            graph.nodes[from].degree++;
            graph.nodes[to].degree++;
        } else {
            graph.nodes[from].degree++;
            graph.nodes[to].degree++;
        }
    }
} 