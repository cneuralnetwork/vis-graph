# Graph Viewer

A simple OpenGL-based graph visualization tool that reads adjacency list format and displays graphs with interactive layouts.

## Requirements

Install the following packages:

Ubuntu/Debian:
```
sudo apt install build-essential freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev
```

Fedora/RHEL:
```
sudo dnf install gcc make freeglut-devel mesa-libGL-devel mesa-libGLU-devel
```

## Build

```
make
```

## Usage

Run the program:
```
./graph_viewer
```

When prompted:
- Enter a filename containing adjacency list data, or press Enter to use stdin
- Specify if the graph is directed (y/n)

## Input Format

Adjacency list format where each line contains:
```
node_id: neighbor1 neighbor2 neighbor3
```

Example:
```
1: 2 3
2: 4
3: 5
4: 6
5: 7 2
6: 8
7: 8
8:
```

## Controls

- L: Force-directed layout
- C: Circular layout  
- R: Reset view and layout
- Mouse drag: Pan view
- Mouse wheel: Zoom in/out
- Arrow keys: Pan view
- +/-: Zoom in/out
- Esc: Exit

## Clean Up

```
make clean
``` 