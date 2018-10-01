# Pagerank Calculator

## A C implementation of the pagerank algorithm with synchronous and asynchronous Gauss-Seidel iterations.
The algorithm needs the CRS representation of a graph in memory in order to compute its pagerank.
Methods for loading web-graphs into the memory that way are provided for a specific file structure.
Therefore, in order to load any web-graph, you might need to write a method that converts
your web-graph file into a file of that structure. 
For an example of such method check the already implemented
one that converts files like the ones [here](https://snap.stanford.edu/data/).

For the parallel algorithm, a simple edge coloring algorithm has been written in a way that makes the
algorithm equivalent to the serial one. Gauss-Seidel convergence speed depends on the order of the nodes too.
The best way to order the nodes is NP but there are heuristic greedy ways to do it. 
Non such way has been implemented as it is, but the way the sources are written makes them easily extensible to
support such algorithms.

In case you want to extend these programs, it's strongly recommended to check the documentation.
A lot of helper methods for debugging/visualizing have already been implemented and might help.

The documentation for the C source can be found [here](https://nicktheway.github.io/Pagerank/).

### Included programs:

* PRcalculator
* PRcalculatorSerial
* WFGcreator

### Usage instructions:

1. Clone/Download repository.
2. Compile with:
    ```$ make ```
3. Create web-graphs using the *WFGcreator* with the included ./data/* or other data like these.
4. Calculate the pagerank using the parallel or the serial calculator.
5. Compare and view results using the python script

    ```python3 ./scripts/visualize_pr.py```
    
    this script will work if you provide all the results (serial+parallel) and modify the path variables accordingly.

### Program execution Example:
To run the parallel program for 10 iterations, store the result in the binary file "./results/pageranks/web-Google.data" 
and the logs in the text file "./results/logs/web-Google.log" run:

```
./bin/PRcalculator ./data/WGFiles/web-Google.wg -i 10 -o ./results/pageranks/web-Google.data -l ./results/logs/web-Google.log
```
To calculate the same thing with the serial program run:

```
./bin/PRserialcalculator ./data/WGFiles/web-Google.wg -i 10 -o ./results/serial/pageranks/web-Google.data -l ./results/serial/logs/web-Google.log
```

To view the results, after modifying the path variables in the python script, run it:
```
python3 ./scripts/visualize_pr.py
```
### More compile options
Run:
+ ```make DEBUG=1``` to compile with debug symbols.
+ ```make WFGcreator``` to compile only the WFGcreator.
+ ```make PRcalculator``` to compile only the parallel version of the pagerank calculator.
+ ```make PRcalculatorSerial``` to compile only the serial version of the pagerank calculator.

### PRcalculator notes
Both versions of the pagerank calculator program support arguments that change the produced files path.
If these arguments are not given, the programs will print their output into predefined paths. If these paths contain folders
that do not exist, the programs will exit early. These predefined paths can be changed before compiling, directly from the source.

Other options that can easily be changed from the sources include the default values for:
+ Convergence constant.
+ Max number of iterations that are supported by the programs.
+ Min number of elements a group must contain in order to initiate parallel threads on the parallel program.

### Visualize results
The included python script is only an example, that was used to generate the images for the report.
All the useful information of the programs are contained in the log file the programs generate and of course the output
binary file with the pagerank values.

The python script can easilly be changed and produce any type of data visualization when the data are available.
For that reason the *helpers.py* script is included and contains two methods that load all the data from the log files 
the PRcalculator programs produce.

---

Developed by *Katomeris Nikolaos*,\
as the last project of *Parallel and Distributed Systems*,\
*Aristotle University Of Thessaloniki, Greece,*\
*7th Semester, 2017-2018.*





