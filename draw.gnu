set title "Random numbers generator\n Tomasz Nowak (232187)"
set xlabel "Number"
set ylabel "Value"
set style function linespoints
set xrange [0:19]
set grid xtics ytics
set key top left
plot 'file.txt' u 1:4, 'file.txt' u 1:5, 'file.txt' u 1:6, 'file.txt' u 1:7
minY = GPVAL_DATA_Y_MIN
maxY = GPVAL_DATA_Y_MAX
set yrange [minY-10:maxY+10]
set terminal jpeg
set output "graph.jpeg"
plot 'file.txt' u 1:2 t "calkowite", 'file.txt' u 1:3 t "rzeczywiste", \
'file.txt' u 1:4 with lines linestyle 1 notitle, 'file.txt' u 1:5 with lines linestyle 1 notitle, \
'file.txt' u 1:6 with lines linestyle 2 notitle, 'file.txt' u 1:7 with lines linestyle 2 notitle