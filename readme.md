
# Source Code Overview


    * official input/output classes from TU:

        lib/kpmp_instance.*
        lib/kpmp_solution_writer.*
    
    * solution representation:
    
        lib/kpmp_solution.cpp  

    * asssignments

        assignement1_1.cpp: construction heuristic


# Algorithm Descriptions

## Counting Crossings

The current implementation is a simple crossing check for all combinations, 
runtime complexity (n-1) * n / 2.


# Dependencies

* boost unit test library

  on debian systems you can install this library with

    apt-get install libboost-test-dev

