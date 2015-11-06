#!/bin/sh

./split_stat.pl output/stagnation_stat.data rand > output/stagnation_stat_rand.data
./split_stat.pl output/stagnation_stat.data first > output/stagnation_stat_first.data
./split_stat.pl output/stagnation_stat.data best > output/stagnation_stat_best.data

./split_stat.pl output/crossings_stat.data rand > output/crossings_stat_rand.data
./split_stat.pl output/crossings_stat.data first > output/crossings_stat_first.data
./split_stat.pl output/crossings_stat.data best > output/crossings_stat_best.data
