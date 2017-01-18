for ((n=0;n<9;n++))
do
	i= "scale=2; $n/10" | bc
	j= "scale=2; ($n+1)/10" | bc
	echo "Loop number: $n"
	cd ~/Dropbox/read_haloes/read
	./read -c DM10G $i $j
	mv *-xygrid.dat ../readSkel
	cd ../readSkel
	./readSkel DM10G $i $j
done
