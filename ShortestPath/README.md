# Find Shortest Path
This part was written in C++ and run in Linux. It will find the shortest path between the start vertex to the end vertex in a graph (G=<V,E>).

You can find more details on []().

# Function
Take input from standard input and output to standard output. Errors can be output to stantdard output but should always start with "Error:". Command `s a b` means to find the shortest path from `a` to `b`.

* enter input graph to adjacency list and do `s` operation.

* execute `s` command in one graph continuely until inputing new graph.


# Sample input and output
### Input
```
V 15

E {<2,6>,<2,8>,<2,5>,<6,5>,<5,8>,<6,10>,<10,8>}

s 2 10
```

### Output
```
2-8-10
```

### Input
```
V 5

E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>}

s 4 0
```
### Output
```
4-1-2-0
```

# Error recognization
* If input V 5, then edge <2,10> doesn't exist.
* If input V 5, then `s 2 10` doesn't exist.
* E must follow by V.
* s will not work if `V` and `E` are not input successfully.

# Termination
This program will terminate when see EOF.

# Run
cd PROJECT && mkdir build && cd build && cmake ../ && make
```
$ ./ece650-a2
```

