# Getting started 

compile:

    cd src; 
    
    make

you can build release or debug builds:

    make debug
    make release


make statistics for all program variants:

    ./make_stat_data.py  

see output dir for generated content.
*be are the actual book embeddings, the *data files are carrying statistic information.
    
after that you can plot stats:

    ./make_stat_plots.sh

this will generate the respective pdfs in the output dir
    


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

