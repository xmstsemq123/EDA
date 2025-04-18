set title "DEF ROW Layout"
set xlabel "X (micron)"
set ylabel "Y (micron)"
set size ratio -1
set xrange [-20000:470000]
set yrange [-20000:170000]
set style fill solid 0.3

plot \
'rows.dat' using 1:2:3:4 with boxxyerrorbars lc rgb "light-blue", \
'NCC.dat' using 1:2:3:4 with boxxyerrorbars lc rgb "gray", \
'components.dat' using 1:2:3:4 with boxxyerrorbars lc rgb "red"
set terminal png size 2048,1024
set output "output.png"
replot