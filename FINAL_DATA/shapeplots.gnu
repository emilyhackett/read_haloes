DM='DM10-f1new.dat'
GAS='GAS10-f1new.dat'
TEMP='TEMP10-f1new.dat'

set xrange [0:5]
set yrange [0:1]

set xlabel 'radius (Mpc)'
set terminal postscript color

# SPHERICITY
set output 'Sphericity'
set ylabel 'S'
set title 'Sphericity'
plot DM using ($1*10/64):8 with lines title 'DM', GAS using ($1*10/64):8 with lines title 'Gas', TEMP using ($1*10/64):8 with lines title 'Temp'
print 'Sphericity plotted to file'

# TRIAXIALITY
set output 'Triaxiality'
set ylabel 'T'
set title 'Triaxiality'
plot DM using ($1*10/64):9 with lines title 'DM', GAS using ($1*10/64):9 with lines title 'Gas', TEMP using ($1*10/64):9 with lines title 'Temp'
print 'Triaxiality plotted to file'

# ELLIPTICITY
set output 'Ellipticity'
set ylabel 'E'
set title 'Ellipticity'
plot DM using ($1*10/64):10 with lines title 'DM', GAS using ($1*10/64):10 with lines title 'Gas', TEMP using ($1*10/64):10 with lines title 'Temp'
print 'Ellipticity plotted to file'

