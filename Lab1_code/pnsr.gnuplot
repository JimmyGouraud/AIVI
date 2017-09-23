# We consider that we have a file, named stats_psnr.txt,
# in which, on each line we have two values
# frameNumber PSNR
#
# This file, passed as parameter to gnuplot :
# gnuplot pnsr.gnuplot
# will display the curve of the MSE against time
# to the file psnr.png

set terminal png
set output "psnr.png"

set xlabel "Frames"
set ylabel "PSNR"
#set xrange [0:110]
#set yrange [0:700]
set xtics 10
set ytics 50
set style line 1 lw 5
set style line 2 lw 5
plot 'stats_psnr.txt' using 1:2 with lines title 'PSNR'
