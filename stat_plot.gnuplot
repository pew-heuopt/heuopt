



clear
reset
unset key
# Make the x axis labels easier to read.
set xtics rotate out
# Select histogram data
set style data histogram
# Give the bars a plain fill pattern, and draw a solid line around them.
set style fill solid border

set key inside left top vertical Right noreverse noenhanced autotitle nobox

set logscale y

set terminal pdf
set output "output/crossings_stat.pdf"
set style histogram clustered
plot for [COL=2:4] 'output/crossings_stat.data' using COL:xticlabels(1) title columnheader

