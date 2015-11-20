#!/bin/sh



./make_random_stat.py --program general-vns &
./make_random_stat.py --program general-vns-stoch-order &
./make_random_stat.py --program general-vns-det-order &

wait
