load 'settings'
set terminal postscript color

set xrange [0:10]
set yrange [0:10]

### DM DENSITY/SKEL PLOT (xy,xz,yz)
xcm=65.0776*10/128
ycm=64.630*10/128
zcm=64.664

DEN='DM10G_0.00_1.00-xygrid.dat'
set output 'DMDenEllipxy'
set title 'DM Density with Inertia Ellipse - xy'
set object 1 ellipse center (65.0776*10/128), (64.6298*10/128) size (71.3631*10/128),(34.5181*10/128) angle 112.05 units xy front fillstyle border lt 4
set object 2 ellipse center (65.0776*10/128), (64.6298*10/128) size (59.7270*10/128),(26.9985*10/128) angle 112.09 units xy front fillstyle border lt 4
set object 3 ellipse center (65.0776*10/128), (64.6298*10/128) size (46.1893*10/128),(20.7739*10/128) angle 111.05 units xy front fillstyle border lt 4
set object 4 ellipse center (65.0776*10/128), (64.6298*10/128) size (30.2292*10/128),(15.7046*10/128) angle 108.34 units xy front fillstyle border lt 4
set object 5 ellipse center (65.0776*10/128), (64.6298*10/128) size (16.2772*10/128),(9.3203*10/128) angle 105.10 units xy front fillstyle border lt 4
set object 6 ellipse center (65.0776*10/128), (64.6298*10/128) size (3.4759*10/128),(2.7418*10/128) angle 95.51 units xy front fillstyle border lt 4
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette
print 'Ellipse plots for DM saved to DMDenEllipxy'

#DEN='DM10G_0.00_1.00-xzgrid.dat'
#set output 'DMDenEllipxz'
#set title 'DM Density with Inertia Ellipse - xz'
#splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$1):(10*$3):7:(10*$4-10*$1):(10*$6-10*$3):7 with vectors nohead lc 5 title 'c0.001'

#DEN='DM10G_0.00_1.00-yzgrid.dat'
#set output 'DMDenSkelyz'
#set title 'DM Density with Skeleton - yz'
#splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$2):(10*$3):7:(10*$5-10*$2):(10*$6-10*$3):7 with vectors nohead lc 5 title 'c0.001'

### GAS DENSITY/SKEL PLOT (xy,xz,yz)

DEN='GAS10G_0.00_1.00-xygrid.dat'
set output 'GasDenEllipxy'
set title 'Gas Density with Inertia Ellipse - xy'
set object 1 ellipse center (63.5478*10/128), (63.4616*10/128) size (63.7908*10/128),(35.4738*10/128) angle 112.76 units xy front fillstyle border lt 4
set object 2 ellipse center (63.5478*10/128), (63.4616*10/128) size (52.5254*10/128),(28.0503*10/128) angle 113.13 units xy front fillstyle border lt 4
set object 3 ellipse center (63.5478*10/128), (63.4616*10/128) size (39.8889*10/128),(21.5929*10/128) angle 111.37 units xy front fillstyle border lt 4
set object 4 ellipse center (63.5478*10/128), (63.4616*10/128) size (25.2057*10/128),(16.2594*10/128) angle 107.80 units xy front fillstyle border lt 4
set object 5 ellipse center (63.5478*10/128), (63.4616*10/128) size (12.9933*10/128),(9.8306*10/128) angle 96.58 units xy front fillstyle border lt 4
set object 6 ellipse center (63.5478*10/128), (63.4616*10/128) size (2.9427*10/128),(2.6844*10/128) angle 101.09 units xy front fillstyle border lt 4
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette
print 'Ellipse plots for GAS saved to GasDenEllipxy'

#DEN='GAS10G_0.00_1.00-xzgrid.dat'
#set output 'GasDenSkelxz'
#set title 'Gas Density with Skeleton - xz'
#splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$1):(10*$3):7:(10*$4-10*$1):(10*$6-10*$3):7 with vectors nohead lc 5 title 'c0.001'

#DEN='GAS10G_0.00_1.00-yzgrid.dat'
#set output 'GasDenSkelyz'
#set title 'Gas Density with Skeleton - yz'
#splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$2):(10*$3):7:(10*$5-10*$2):(10*$6-10*$3):7 with vectors nohead lc 5 title 'c0.001'

### TEMP DENSITY/SKEL PLOT (xy,xz,yz)

set cbrange [7:15]

DEN='TEMP10G_0.00_1.00-xygrid.dat'
set output 'TempDenEllipxy'
set title 'Temperature Density with Inertia Ellipse - xy'
set object 1 ellipse center (63.3605*10/128), (62.7828*10/128) size (53.2352*10/128),(39.5764*10/128) angle 114.37 units xy front fillstyle border lt 4
set object 2 ellipse center (63.3605*10/128), (62.7828*10/128) size (42.9170*10/128),(32.2261*10/128) angle 114.77 units xy front fillstyle border lt 4
set object 3 ellipse center (63.3605*10/128), (62.7828*10/128) size (32.4347*10/128),(25.2551*10/128) angle 111.32 units xy front fillstyle border lt 4
set object 4 ellipse center (63.3605*10/128), (62.7828*10/128) size (21.4011*10/128),(18.9727*10/128) angle 106.92 units xy front fillstyle border lt 4
set object 5 ellipse center (63.3605*10/128), (62.7828*10/128) size (11.8604*10/128),(10.7886*10/128) angle 81.45 units xy front fillstyle border lt 4
set object 6 ellipse center (63.3605*10/128), (62.7828*10/128) size (2.7756*10/128),(2.6599*10/128) angle 18.76 units xy front fillstyle border lt 4
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette
print 'Ellipse plots for TEMP saved to TempDenEllipxy'

#DEN='TEMP10G_0.00_1.00-xzgrid.dat'
#set output 'TempDenEllipxz'
#set title 'Temperature Density with Inertia Ellipse - xz'
#splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette

#DEN='TEMP10G_0.00_1.00-yzgrid.dat'
#set output 'TempDenSkelyz'
#set title 'Temperature Density with Skeleton - yz'
#splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette

unset object 1
unset object 2
unset object 3
unset object 4
unset object 5
unset object 6
