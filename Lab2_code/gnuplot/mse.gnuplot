# We consider that we have a file, named stats_mse.txt,
# in which, on each line we have two values
# frameNumber MSE
#
# This file, passed as parameter to gnuplot :
# gnuplot mse.gnuplot
# will display the curve of the MSE against time
# to the file mse.png

set terminal png
set output "mse.png"

set xlabel "Frames"
set ylabel "MSE"
set xtics 10
set ytics 50
set style line 1 lw 5
set style line 2 lw 5

plot 'stats_mse_1.txt' using 1:2 with lines title 'MSE lvl 1', 'stats_mse_2.txt' using 1:2 with lines title 'MSE lvl 2', 'stats_mse_3.txt' using 1:2 with lines title 'MSE lvl 3'
