#!/usr/bin/gnuplot
set terminal png size 800,480 enhanced font 'Arial, 10'

set style line 1 linecolor rgb 'red' linetype 1 linewidth 2
set style line 2 linecolor rgb 'blue' linetype 1 linewidth 2

set border linewidth 1
set key top left
set grid
set mytics 1
set format x "%1.0f"
set format y "%.1f"
set xlabel "Threads" font "Arial, 16"
set ylabel "Speedup" font "Arial, 16"
set xtics 2 font "Arial, 10"
set ytics 0.5 font "Arial, 10"
set rmargin 4
set tmargin 2
set mxtics

set output 'result.png'
plot "data.dat" using 1:2 title "counter parallel" with linespoints ls 1, \
	"ideal.dat" using 1:2 title "Linear Speedup" with linespoints ls 2

