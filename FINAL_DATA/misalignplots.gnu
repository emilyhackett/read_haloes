set ylabel 'theta (rad)'
set xlabel 'radius (Mpc)'
set xrange [0:5]
unset yrange

set terminal postscript color
angle(x1,y1,z1,x2,y2,z2)=acos((x1*x2+y1*y2+z1*z2)/(sqrt(x1*x1+y1*y1+z1*z1)*sqrt(x2*x2+y2*y2+z2*z2)))

set output 'GasDMAlign'
set title 'Misalignment Angle between DM and Stellar matter'
plot 'GASDM-FILE.dat' using ($1*10/128):(angle($5,$6,$7,$15,$16,$17)) with lines notitle
print 'Misalignment angle btwn DM and GAS plotted to GasDMAlign'
