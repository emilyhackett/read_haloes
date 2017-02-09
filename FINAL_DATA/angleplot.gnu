set ylabel 'theta (rad)'
set xlabel 'radius (Mpc)'
set xrange [0.5:5]

xave=0.0261
yave=0.9917
zave=0.1257

angle(x1,y1,z1,x2,y2,z2)=acos((x1*x2+y1*y2+z1*z2)/(sqrt(x1*x1+y1*y1+z1*z1)*sqrt(x2*x2+y2*y2+z2*z2)))

set terminal postscript color

# PLOT FOR AVERAGED FILAMENT DIRECTION
set output 'MisAvec0-001'
set title 'Misalignment Angle (Filament Average)'
plot 'DM-FILE.dat' using ($1/2):(angle($9,$10,$11,xave,yave,zave)) with lines title 'DM', 'GAS-FILE.dat' using ($1/2):(angle($9,$10,$11,xave,yave,zave)) with lines title 'GAS'
print 'Angle btwn filament (average) and major axis plotted to MisalignmentAve'

# PLOT FOR DIFFERENT FILAMENT DIRECTIONS (DEPENDENT ON RADIUS)
set output 'MisRadc0-001'
set title 'Misalignment Angle (Radius-Dependent Filament)'
plot 'DM-FILE.dat' using ($1/2):(angle($9,$10,$11,$2,$3,$4)) with lines title 'DM', 'GAS-FILE.dat' using ($1/2):(angle($9,$10,$11,$2,$3,$4)) with lines title 'GAS'
print 'Angle btwn filament (radius) and major axis plotted to MisalignmentRad'
