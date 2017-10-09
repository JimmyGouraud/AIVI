# We consider that we have a file, named stats_entropy.txt,
# in which, on each line we have three values
# frameNumber MSE PSNR
#
#This file, passed as parameter to gnuplot :
# gnuplot entropy.gnuplot
# will save the curves of the entropy Current against time, and entropy Error against time
# to the file out.png

set terminal png
set output "entropy.png"

set xlabel "Frames"
set ylabel "entropy Current, entropy Error"
set xrange [0:100]
set yrange [1:8]
set xtics 10
set ytics 1
set style line 1 lw 5
set style line 2 lw 5

plot 'stats_entropy.txt' using 1:2 with lines title 'entropy Current', 'stats_entropy.txt' using 1:3 with lines title 'entropy Error lvl 3', 'stats_entropy.txt' using 1:4 with lines title 'entropy Error lvl 2', 'stats_entropy.txt' using 1:5 with lines title 'entropy Error lvl 1'
