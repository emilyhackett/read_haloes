set size square 1,1
set tmargin 2
set view map
set ticslevel 0
set palette rgb 36,35,34
set xrange [:10]
set yrange [:10]

xm=62.5355*10/128
ym=70.5711*10/128

set title 'Ellipse projections for DM particles'
set xlabel 'x (Mpc)'
set ylabel 'y (Mpc)'

set object 1 ellipse center xm,ym size (72.0986*10/128),(29.5002*10/128) angle 114.50 units xy front fillstyle empty
#set object 2 ellipse center xm,ym size (50.5134*10/128),(18.3204*10/128) angle 113.61 units xy front fillstyle empty
#set object 3 ellipse center xm,ym size (24.9977*10/128),(9.1597*10/128) angle 112.78 units xy front fillstyle empty

set object 4 ellipse center xm,ym size (68.5349*5/128),(31.0987*5/128) angle 115.09 units xy front fillstyle empty
set object 5 ellipse center xm,ym size (44.1280*5/128),(21.4198*5/128) angle 115.62 units xy front fillstyle empty
#set object 6 ellipse center xm,ym size (21.5538*5/128),(10.5653*5/128) angle 119.10 units xy front fillstyle empty

set object 7 ellipse center xm,ym size (58.6962*2/128),(33.5406*2/128) angle 101.25 units xy front fillstyle empty
set object 8 ellipse center xm,ym size (37.8205*2/128),(22.8215*2/128) angle 99.65 units xy front fillstyle empty
#set object 9 ellipse center xm,ym size (16.3592*2/128),(13.4767*2/128) angle 83.84 units xy front fillstyle empty

splot 'DM10G_0.00_1.00-xygrid.dat' using ($1*10/128):($2*10/128):4 with points palette ps 1 pt 7 notitle
#, 'DM10G_0.00_1.00-segpos.dat' using ($1*10):($2*10):7:(($4-$1)*10):(($5-$2)*10):7 with vectors nohead lc 5 notitle
