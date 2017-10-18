
set terminal png
set output "mse.png"

set xlabel "Frames"
set ylabel "MSE"
set xtics 10
set ytics 50
set style line 1 lw 5
set style line 2 lw 5

#plot 'bm.txt' using 1:2 with lines title 'Block Matching', 'hs.txt' using 1:2 with lines title 'Horn\&Schunk', 'f.txt' using 1:2 with lines title 'Farneback', 'mit.txt' using 1:2 with lines title 'MIT', 'gme.txt' using 1:7 with lines title 'GME'
#plot 'bm.txt' using 1:2 with lines title 'Block Matching', 'hs.txt' using 1:2 with lines title 'Horn\&Schunk'
#plot 'bm.txt' using 1:2 with lines title 'Block Matching', 'hs.txt' using 1:2 with lines title 'Horn\&Schunk', 'f_lvl1.txt' using 1:2 with lines title 'Farneback lvl 1', 'f_lvl3.txt' using 1:2 with lines title 'Farneback lvl 3'
plot 'bm.txt' using 1:2 with lines title 'Block Matching', 'hs.txt' using 1:2 with lines title 'Horn\&Schunk', 'f.txt' using 1:2 with lines title 'Farneback', 'mit.txt' using 1:2 with lines title 'MIT'
