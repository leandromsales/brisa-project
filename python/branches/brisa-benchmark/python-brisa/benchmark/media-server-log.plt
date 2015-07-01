### cpNumber-X-DiffTime
set encoding iso_8859_1
set terminal svg size 800,500
set output 'media-server-cpNumber-X-DiffTime.svg'

#set xrange ["0":"10"]
#set yrange [0:5000]
set format x "%g"
set format y "%g"
set grid
set xtics autofreq 0.5
set mxtics 5
set ytics autofreq 0.0001
set mytics 0
set datafile commentschars "#"
set datafile separator "\t"

plot 'media-server-log.txt' using 1:4 notitle with linespoints lt 3 lw 4 pt 7

### cpNumber-X-BrowserStartTime
set encoding iso_8859_1
set terminal svg size 800,500
set output 'media-server-cpNumber-X-BrowserStartTime.svg'

#set xrange ["0":"10"]
#set yrange [0:5000]
set format x "%g"
set format y "%g"
set grid
set xtics autofreq 0.5
set mxtics 5
set ytics autofreq 0.5
set mytics 0
set datafile commentschars "#"
set datafile separator "\t"

plot 'media-server-log.txt' using 1:2 notitle with linespoints lt 3 lw 4 pt 7
