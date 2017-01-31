set size square 1,1
set tmargin 2
set view map
set ticslevel 0
set palette rgb 36,35,34
set xrange [:10]
set yrange [:10]
set term postscript color
set output 'DM10-ellipseplots.ps'
xm=65.078*10/128
ym=64.630*10/128
set object 1 ellipse center xm,ym size 72.0986*10/128,29.5002*10/128 angle 114.50 units xy front fillstyle empty
set object 2 ellipse center xm,ym size 50.5134*10/128,18.3204*10/128 angle 113.61 units xy front fillstyle empty
set object 3 ellipse center xm,ym size 24.9977*10/128,9.1597*10/128 angle 112.78 units xy front fillstyle empty
set object 4 circle at xm,ym size first 0.1 front fillstyle border
set title 'E-vector ellipse for DM particles with skeleton, 10Mpc box'
splot 'DM10G_0.00_1.00-xygrid.dat' using (10*$1/128):(10*$2/128):4 with points palette ps 1 pt 7 notitle, 'DM10G_0.00_1.00-segpos.dat' using ($1*10):($2*10):7:(($4-$1)*10):(($5-$2)*10):7 with vectors nohead lc 5 notitle
