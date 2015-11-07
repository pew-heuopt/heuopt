#!/bin/sh

./split_stat.pl output/stagnation_stat.data rand > output/stagnation_stat_rand.data
./split_stat.pl output/stagnation_stat.data first > output/stagnation_stat_first.data
./split_stat.pl output/stagnation_stat.data best > output/stagnation_stat_best.data

./split_stat.pl output/crossings_stat.data rand > output/crossings_stat_rand.data
./split_stat.pl output/crossings_stat.data first > output/crossings_stat_first.data
./split_stat.pl output/crossings_stat.data best > output/crossings_stat_best.data

gnuplot -e "datafile='output/stagnation_stat_rand.data';outputfile='output/stagnation_stat_rand.pdf'" stat_plot_3bars_left.gnuplot
gnuplot -e "datafile='output/stagnation_stat_first.data';outputfile='output/stagnation_stat_first.pdf'" stat_plot_3bars_left.gnuplot
gnuplot -e "datafile='output/stagnation_stat_best.data';outputfile='output/stagnation_stat_best.pdf'" stat_plot_3bars_left.gnuplot

#gnuplot -e "datafile='output/crossings_stat_rand.data';outputfile='output/crossings_stat_rand.pdf'" stat_plot_3bars_left_logscale.gnuplot
#gnuplot -e "datafile='output/crossings_stat_first.data';outputfile='output/crossings_stat_first.pdf'" stat_plot_3bars_left_logscale.gnuplot
#gnuplot -e "datafile='output/crossings_stat_best.data';outputfile='output/crossings_stat_best.pdf'" stat_plot_3bars_left_logscale.gnuplot

gnuplot -e "datafile='output/crossings_stat_rand.data';outputfile='output/crossings_stat_rand.pdf'" stat_plot_3bars_left.gnuplot
gnuplot -e "datafile='output/crossings_stat_first.data';outputfile='output/crossings_stat_first.pdf'" stat_plot_3bars_left.gnuplot
gnuplot -e "datafile='output/crossings_stat_best.data';outputfile='output/crossings_stat_best.pdf'" stat_plot_3bars_left.gnuplot
