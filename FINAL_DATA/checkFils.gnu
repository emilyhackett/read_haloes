load 'settings'
SKEL='c0.001.up_0.00_1.00-segpos.dat'

set xrange [0:10]
set yrange [0:10]

xave=-0.0183
yave=0.9930
zave=0.1166

set terminal postscript color

set output 'Filxy'
set title 'Filament vectors for Gas xy projection'
DEN='GAS10G_0.00_1.00-xygrid.dat'
set arrow from 5,5 to (5+xave),(5+yave) lc 8 front
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$1):(10*$2):7:(10*$4-10*$1):(10*$5-10*$2):7 with vectors nohead lc 5 title 'c0.001', 'FIL.dat' using (5):(5):1:2:3:1 with vectors lc 7 notitle
print 'xy Fil vectors plotted to Filxy'

set output 'Filxz'
set title 'Filament vectors for Gas xz projection'
DEN='GAS10G_0.00_1.00-xzgrid.dat'
unset arrow
set arrow from 5,5 to (5+xave),(5+zave) lc 8 front
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$1):(10*$3):7:(10*$4-10*$1):(10*$6-10*$3):7 with vectors nohead lc 5 title 'c0.001', 'FIL.dat' using (5):(5):1:2:4:1 with vectors lc 7 notitle
print 'xz Fil vectors plotted to Filxz'

set output 'Filyz'
set title 'Filament vectors for radius range in gas yz projection'
DEN='GAS10G_0.00_1.00-yzgrid.dat'
unset arrow
set arrow from 5,5 to (5+yave),(5+zave) lc 8 front
splot DEN using ($1*10/128):($2*10/128):4 notitle with points ps 1 pt 7 palette, SKEL using (10*$2):(10*$3):7:(10*$5-10*$2):(10*$6-10*$3):7 with vectors nohead lc 5 title 'c0.001', 'FIL.dat' using (5):(5):1:3:4:1 with vectors lc 7 notitle
print 'yz Fil vectors plotted to Filyz'

unset arrow
