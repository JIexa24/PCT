#!/usr/bin/gnuplot
set terminal png size 800,480 enhanced font 'Arial, 10'

set style line 1 linecolor rgb 'red' linetype 2 linewidth 2
set style line 3 linecolor rgb 'blue' linetype 1 linewidth 2
set style line 2 linecolor rgb 'green' linetype 1 linewidth 2


set border linewidth 1
set key top left
set grid
set mytics 10
set format x "%1.0f"
set format y "%.1f"
set xlabel "Количество процессов" font "Arial, 16"
set ylabel "Относительное ускорение" font "Arial, 16"
set xtics 8 font "Arial, 10"
set ytics 1 font "Arial, 10"
set rmargin 4
set tmargin 2
set mxtics

set output 'result.png'
plot "ideal.dat" using 1:2 title "Линейное ускорение" with linespoints ls 1, \
	"test.dat" using 1:2 title "Параллельная версия, N = 30650" with linespoints ls 3, \
