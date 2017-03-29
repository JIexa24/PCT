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
set xlabel "Threads" font "Arial, 16"
set ylabel "Speedup" font "Arial, 16"
set xtics 2 font "Arial, 10"
set ytics 0.5 font "Arial, 10"
set rmargin 4
set tmargin 2
set mxtics

set output 'result.png'
plot "parallel.dat" using 1:2 title "parallel" with linespoints ls 3, \
	"parallel (FS).dat" using 1:2 title "parallel (FS)" with linespoints ls 2, \
	"ideal.dat" using 1:2 title "Linear Speedup" with linespoints ls 1

set output 'serial.png'
plot "serial.dat" using 1:2 title "serial" with linespoints ls 1

set output 'parallel (FS).png'
plot "parallel (FS).dat" using 1:2 title "parallel (FS)" with linespoints ls 3

set output 'parallel.png'
plot "parallel.dat" using 1:2 title "parallel" with linespoints ls 2
