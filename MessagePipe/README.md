# Message Pipe
This part I use multi process and message pipe to combine fraph-building and searching-shortest-path part. I integraed all modules from streets generation randomly to generate graph, to find shortest path bettween arbitrary two coordinates input by keyboard.

More details and requirements you can find on [pipe Together](https://github.com/QuanyuWANG96/Security-Cameras-System-for-Streets/blob/master/MessagePipe/ece650.a3.pdf)

# Function
* Modify the output format of the Python script from `build graph` to match the input format of the C++ program as well as `shortest path`.

* Write program in C++ to generate random street information for the Python script.

* Write a driver program in C++ that uses Inter-Process Communication (IPC) to link the output of the random input generator to the input of the Python script, and the output of the Python script to the input of the C++ program from project 2 to output the input graph on standard output.


# Files
[rgen.cpp](https://github.com/QuanyuWANG96/Security-Cameras-System-for-Streets/blob/master/MessagePipe/rgen.cpp) random streets generator

[ece650-a1.py](https://github.com/QuanyuWANG96/Security-Cameras-System-for-Streets/blob/master/MessagePipe/ece650-a1.py) map streets into graph G=<V,E>

[ece650-a2.cpp](https://github.com/QuanyuWANG96/Security-Cameras-System-for-Streets/blob/master/MessagePipe/ece650-a2.cpp) find shortest path in a graph

[ece650-a3.cpp](https://github.com/QuanyuWANG96/Security-Cameras-System-for-Streets/blob/master/MessagePipe/ece650-a3.cpp) driver program, pipe all the three files above


# Sample input and output
### Input
```
$ ./ece650-a3 -s 5 -n 4 -l 5
```
### Output
```
V 8

E {<0,2>,<0,3>,<0,4>,<1,3>,<4,7>,<5,2>,<5,6>}
```
### Input
```
s 2 4
```
### Output
```
2-0-4
```

# Random input generator and Command-line arguments
* `-s k` where `k` is an integer >= 2. The number of streets should be a random integer in [2, k]. If this option is not specified, you should use a default of k = 10; that is, the number of streets should be a random integer in [2, 10].

* `-n k` where `k` is an integer >= 1. The number of line-segments in each street should be a random integer in [1, k]. Default: k = 5.

* `-l k` where `k` is an integer >= 5. Your process should wait a random number w seconds, where w is in [5, k] before generating the next (random) input. Default: k = 5.

* `-c k` where `k` is an integer >= 1. Your process should generate (x, y) coordinates such that every x and y value is in the range [-k, k]. For example, if k = 15, all of your coordinate values should be integers between -15 and 15. Default: k = 20.

# Execution process

* enough `r` commands to clear any existing street database
* `a` commands to add new streets satisfying the specification
*` g` command to ece650-a1.py
* wait for specified number of seconds and repeat.

# Driver
The driver program has the overall control. It will using message pipe to allow three different processes communicate with each other.

(1) the random generator[rgen.cpp](https://github.com/QuanyuWANG96/Security-Cameras-System-for-Streets/blob/master/MessagePipe/rgen.cpp) 
(2) street to graph [ece650-a1.py](https://github.com/QuanyuWANG96/Security-Cameras-System-for-Streets/blob/master/MessagePipe/ece650-a1.py) 
(3) find shortest path [ece650-a2.cpp](https://github.com/QuanyuWANG96/Security-Cameras-System-for-Streets/blob/master/MessagePipe/ece650-a2.cpp) find shortest path in a graph

# Termination
All processes would terminate gracefully (and quietly) once see EOF at stdin.

# Execution
```
cd PROJECT && mkdir build && cd build && cmake ../ && make install

$ ./ece650-a3 -s 5 -n 4 -l 5

s 3 10
```
