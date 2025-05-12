set terminal pngcairo size 600,600 enhanced font 'Arial,12'
set output 'routing_plot.png'
set title "PIC Routing Result"
set size ratio -1
set grid
set xlabel "X"
set ylabel "Y"
plot for [i=0:2] 'routing_lines.dat' index i using 1:2 with lines lw 2