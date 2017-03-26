#!/usr/bin/gnuplot
set terminal png size 800,480 enhanced font 'Arial, 16'

set style line 1 linecolor rgb 'red' linetype 1 linewidth 2
set style line 2 linecolor rgb 'blue' linetype 1 linewidth 2
set style line 3 linecolor rgb 'green' linetype 1 linewidth 2
set style line 4 linecolor rgb 'cyan' linetype 1 linewidth 2

set border linewidth 1
set key top left
set grid
set mytics 1
set format x "%1.0f"
set format y "%.6f"
set xlabel "Number of threads" font "Arial, 16"
set ylabel "Speedup (sec)" font "Arial, 16"
set xtics 2 font "Arial, 12"
set ytics font "Arial, 12"
set rmargin 4
set tmargin 2
set mxtics

set output 'result.png'
plot "15000.dat" using 1:2 title "15000" with linespoints ls 1, \
	"20000.dat" using 1:2 title "20000" with linespoints ls 3, \
	"25000.dat" using 1:2 title "25000" with linespoints ls 2, \
	"ideal.dat" using 1:2 title "ideal" with linespoints ls 4

set output '15000_elements.png'
plot "15000.dat" using 1:2 title "15000" with linespoints ls 1

set output '20000_elements.png'
plot "20000.dat" using 1:2 title "20000" with linespoints ls 3

set output '25000_elements.png'
plot "25000.dat" using 1:2 title "25000" with linespoints ls 2
