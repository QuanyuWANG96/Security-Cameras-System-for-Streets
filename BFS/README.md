# BFS
The requirements refered to [encoding](https://github.com/QuanyuWANG96/Security-Cameras-System-for-Streets/blob/master/BFS/a4_encoding.pdf) and [minisat specification](https://github.com/QuanyuWANG96/Security-Cameras-System-for-Streets/blob/master/BFS/ece650.a4.pdf)


# Function
Use `MiniSat SAT solver` to solve Minimal Vertex Cover problem. 

The minimal vertext cover is to find the most effective locations to install cameras.

# Sample run
### Input
```
$ ./ece650-a4 V 5 E {<0,4>,<4,1>,<0,3>,<3,4>,<3,2>,<1,3>}
```
### Output
```
3 4
```

# Execution
```
cd PROJECT && mkdir build && cd build && cmake ../
```

