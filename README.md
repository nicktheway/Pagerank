# Pagerank Calculator

This API contains methods for storing web-graphs in CRS format, coloring them and
calculating their pagerank vector.

It's easily debugable with the various graph visualizing methods that are included.

## Included programs:

* PRcalculator
* PRcalculatorSerial
* WFGcreator

## Usage instructions:

1. Clone/Download repository.
2. Compile with:
    ```$ make ```
3. Create web-graphs using the *WFGcreator* with the included ./data/* or other data like these.
4. Calculate the pagerank using the parallel or the serial calculator.
5. Compare and view results using the python script
    ```python3 ./scripts/visualize_pr.py```
    this script will work if you provide all the results (serial+parallel) and modify the  path variables accordingly.

## C API documentation
Documentation on the C API can be found here: https://nicktheway.github.io/Pagerank/





