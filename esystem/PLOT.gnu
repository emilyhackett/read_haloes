set size square 1,1
set tmargin 2
set view map
set ticslevel 0
set palette rgb 36,35,34
set xrange [:10]
set yrange [:10]

xmD=62.5355*10/128
ymD=70.5711*10/128

xmG=61.3748*10/128
ymG=69.1903*10/128

set xlabel 'x (Mpc)'
set ylabel 'y (Mpc)'

set title 'TEMP density with ellipse and skel'

# FROM DM10G
#set object 1 ellipse center xmD,ymD size (72.0986*10/128),(29.5002*10/128) angle 114.50 units xy front fillstyle border lt 2
#set object 2 ellipse center xmD,ymD size (50.5134*10/128),(18.3204*10/128) angle 113.61 units xy front fillstyle border lt 4
#set object 3 ellipse center xmD,ymD size (24.9977*10/128),(9.1597*10/128) angle 112.78 units xy front fillstyle border lt 4

# FROM DM05G
#set object 4 ellipse center xmD,ymD size (68.5349*5/128),(31.0987*5/128) angle 115.09 units xy front fillstyle border lt 2
#set object 5 ellipse center xmD,ymD size (44.1280*5/128),(21.4198*5/128) angle 115.62 units xy front fillstyle border lt 2
#set object 6 ellipse center xmD,ymD size (21.5538*5/128),(10.5653*5/128) angle 119.10 units xy front fillstyle border lt 4

# FROM DM02G
#set object 7 ellipse center xmD,ymD size (58.6962*2/128),(33.5406*2/128) angle 101.25 units xy front fillstyle border lt 2
#set object 8 ellipse center xmD,ymD size (37.8205*2/128),(22.8215*2/128) angle 99.65 units xy front fillstyle border lt 2
#set object 9 ellipse center xmD,ymD size (16.3592*2/128),(13.4767*2/128) angle 83.84 units xy front fillstyle border lt 4

# FROM GAS10G
#set object 10 ellipse center xmG,ymG size (66.5325*10/128),(30.2903*10/128) angle 112.94 units xy front fillstyle border lt 4
#set object 11 ellipse center xmG,ymG size (45.9407*10/128),(18.9565*10/128) angle 111.90 units xy front fillstyle border lt 4
#set object 12 ellipse center xmG,ymG size (22.6482*10/128),(8.9802*10/128) angle 109.20 units xy front fillstyle border lt 4

# FROM GAS05G
#set object 13 ellipse center xmG,ymG size (59.5306*5/128),(31.9767*5/128) angle 112.36 units xy front fillstyle border lt 4
#set object 14 ellipse center xmG,ymG size (36.5288*5/128),(22.2634*5/128) angle 110.88 units xy front fillstyle border lt 4
#set object 15 ellipse center xmG,ymG size (17.9316*5/128),(11.3500*5/128) angle 111.21 units xy front fillstyle border lt 4

# FROM GAS02G
#set object 16 ellipse center xmG,ymG size (48.1600*2/128),(37.7076*2/128) angle 90.94 units xy front fillstyle border lt 4
#set object 17 ellipse center xmG,ymG size (30.7445*2/128),(26.2727*2/128) angle 90.14 units xy front fillstyle border lt 4
#set object 18 ellipse center xmG,ymG size (14.6026*2/128),(13.7076*2/128) angle 93.92 units xy front fillstyle border lt 4

# FROM TEMP10G


splot 'GAS10G_0.00_1.00-xygrid.dat' using ($1*10/128):($2*10/128):4 with points palette ps 1 pt 7 notitle
#, 'DM10G_0.00_1.00-segpos.dat' using ($1*10):($2*10):7:(($4-$1)*10):(($5-$2)*10):7 with vectors nohead lc 5 notitle
