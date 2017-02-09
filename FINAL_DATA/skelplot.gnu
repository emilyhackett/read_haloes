SKEL='temp.ND.0_c100.up-segpos.dat'
SKELNAME='temp0_c100'
load 'settings'

set xrange [0:10]
set yrange [0:10]

set terminal postscript color

# DM DENSITY/SKEL PLOT (xy,xz,yz)

DEN='DM10G_0.00_1.00-xygrid.dat'
set output 'DMDenSkelxy'
set title 'DM Density with Skeleton - xy'
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$1):(10*$2):7:(10*$4-10*$1):(10*$5-10*$2):7 with vectors nohead lc 5 title SKELNAME
print 'DM Density with Skeleton xy plotted to DMDenSkelxy'

DEN='DM10G_0.00_1.00-xzgrid.dat'
set output 'DMDenSkelxz'
set title 'DM Density with Skeleton - xz'
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$1):(10*$3):7:(10*$4-10*$1):(10*$6-10*$3):7 with vectors nohead lc 5 title SKELNAME
print 'DM Density with Skeleton xz plotted to DMDenSkelxz'

DEN='DM10G_0.00_1.00-yzgrid.dat'
set output 'DMDenSkelyz'
set title 'DM Density with Skeleton - yz'
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$2):(10*$3):7:(10*$5-10*$2):(10*$6-10*$3):7 with vectors nohead lc 5 title SKELNAME
print 'DM Density with Skeleton yz plotted to DMDenSkelyz'

# GAS DENSITY/SKEL PLOT (xy,xz,yz)

DEN='GAS10G_0.00_1.00-xygrid.dat'
set output 'GasDenSkelxy'
set title 'Gas Density with Skeleton - xy'
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$1):(10*$2):7:(10*$4-10*$1):(10*$5-10*$2):7 with vectors nohead lc 5 title SKELNAME
print 'Gas Density with Skeleton xy plotted to GasDenSkelxy'

DEN='GAS10G_0.00_1.00-xzgrid.dat'
set output 'GasDenSkelxz'
set title 'Gas Density with Skeleton - xz'
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$1):(10*$3):7:(10*$4-10*$1):(10*$6-10*$3):7 with vectors nohead lc 5 title SKELNAME
print 'Gas Density with Skeleton xz plotted to GasDenSkelxz'

DEN='GAS10G_0.00_1.00-yzgrid.dat'
set output 'GasDenSkelyz'
set title 'Gas Density with Skeleton - yz'
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$2):(10*$3):7:(10*$5-10*$2):(10*$6-10*$3):7 with vectors nohead lc 5 title SKELNAME
print 'Gas Density with Skeleton yz plotted to GasDenSkelyz'

# TEMP DENSITY/SKEL PLOT (xy,xz,yz)

set cbrange [7:15]

DEN='TEMP10G_0.00_1.00-xygrid.dat'
set output 'TempDenSkelxy'
set title 'Temperature Density with Skeleton - xy'
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$1):(10*$2):7:(10*$4-10*$1):(10*$5-10*$2):7 with vectors nohead lc 5 title SKELNAME
print 'Temp Density with Skeleton xy plotted to TempDenSkelxy'

DEN='TEMP10G_0.00_1.00-xzgrid.dat'
set output 'TempDenSkelxz'
set title 'Temperature Density with Skeleton - xz'
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$1):(10*$3):7:(10*$4-10*$1):(10*$6-10*$3):7 with vectors nohead lc 5 title SKELNAME
print 'Temp Density with Skeleton xz plotted to TempDenSkelxz'

DEN='TEMP10G_0.00_1.00-yzgrid.dat'
set output 'TempDenSkelyz'
set title 'Temperature Density with Skeleton - yz'
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$2):(10*$3):7:(10*$5-10*$2):(10*$6-10*$3):7 with vectors nohead lc 5 title SKELNAME
print 'Temp Density with Skeleton yz plotted to TempDenSkelyz'
