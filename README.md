# Lamina-Project
Isoforce mesh optimization for computational microbiology -- Student Project in Computer Graphics

# Licensing
For you ease of use, the following source code dependencies have been included and are made available to you under the following licenses:

0. Eigen math library -- MPL 2.0 copyleft license
0. Sqlite3 -- Released into the Public Domain
0. TCLAP -- MIT License

All other code was written by the students and is hereby released under the MIT License. A copy of the license has been provided to you with the source directory structure. 

# Requirements
The software was developed targetting Debian, but should be portable to other systems with little effort. It requires a compiler supporting C++11. The build system assumes the use of GCC but with minor changes will build with Clang. On Linux Mint 17, the program can compiled as follows:

```
sudo aptitude install build-essential autoconf automake freeglut3-dev
make clean
make
```

# Usage

```
./Generator.bin  {--point|--bar|--wave} [--fib] [-n <integer>] [-f
                    <string>] [--] [--version] [-h]
./Simulator.bin  {--anneal|--genetic} [--endTemp <double>] [--startTemp
                    <double>] [--mutationRate <double>] [--maxStepTime
                    <int>] [--populationSize <int>] [--alpha <double>]
                    [--numGens <int>] [--fieldStrength <double>] [-f
                    <string>] [--] [--version] [-h]
./main.bin sys.db
```

For extended usage information, pass -h or --help command line arguments. 

# Directory Structure

* lib/ -- dependent libraries
* test/ -- database test program
* Viewer/ -- results visualization application
* lib/eigen -- Eigen math library
* lib/sim -- generation and simulation library
* lib/sqlite -- underlying database implementation (sqlite3)
* lib/tclap -- TCLAP: Templated Comand Line Argument Parser library

# Files of Interest

* AnnealingSimulation.cpp -- implements simulated annealing algorithm
* GeneticAlgorithm.cpp -- implements genetic algorithm
* Database.cpp -- interface to the sqlite database
* Simulation.cpp -- simulation interface


