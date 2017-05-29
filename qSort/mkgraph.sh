#!/usr/bin/gnuplot
set terminal png size 800,480 enhanced font 'Arial, 10'

set style line 1 linecolor rgb 'red' linetype 1 linewidth 2
set style line 2 linecolor rgb 'blue' linetype 1 linewidth 2
set style line 3 linecolor rgb 'green' linetype 1 linewidth 2
set style line 4 linecolor rgb 'cyan' linetype 1 linewidth 2

set border linewidth 1
set key top left
set grid
set mytics 1
set format x "%1.0f"
set format y "%.1f"
set xlabel "Number of Threads" font "Arial, 16"
set ylabel "Speedup" font "Arial, 16"
set xtics 2 font "Arial, 10"
set ytics 0.5 font "Arial, 10"
set rmargin 4
set tmargin 2
set mxtics

set output 'result.png'
plot "v1.dat" using 1:2 title "N = 1000000" with linespoints ls 3, \
	"ideal.dat" using 1:2 title "Linear Speedup" with linespoints ls 1, \
	"v1.dat" using 1:3 title "N = 2000000" with linespoints ls 2, \
	"v1.dat" using 1:4 title "N = 3000000" with linespoints ls 4

set output 'result2.png'
plot "v2.dat" using 1:2 title "Treshold = 10000" with linespoints ls 3, \
	"ideal.dat" using 1:2 title "Linear Speedup" with linespoints ls 1, \
	"v2.dat" using 1:3 title "Treshold = 50000" with linespoints ls 2, \
	"v2.dat" using 1:4 title "Treshold = 100000" with linespoints ls 4
