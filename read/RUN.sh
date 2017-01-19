dmfiles=("DM02G" "DM05G" "DM10G")
gasAfiles=("GAS02A" "GAS10A")
gasvrfiles=("GASvr02A" "GASvr10A")
gasGfiles=("GAS" "GAS02G" "GAS05G" "GAS10G")
rvirfiles=("Rvir1G" "Rvir1Vx" "Rvir1Vy" "Rvir1Vz" "Rvir3G" "Rvir3Vx" "Rvir3Vy" "Rvir3Vz")
tempfiles=("TEMP02A" "TEMP02G" "TEMP10A" "TEMP10G")
for item in ${tempfiles[@]}
do
	echo "LOOKING AT CUBE $item ..."
	cd ../read/
	./read -d "../Datfiles/$item" 0.0 0.1
	./read -d "../Datfiles/$item" 0.1 0.2
	./read -d "../Datfiles/$item" 0.2 0.3
	./read -d "../Datfiles/$item" 0.3 0.4
	./read -d "../Datfiles/$item" 0.4 0.5
	./read -d "../Datfiles/$item" 0.5 0.6
	./read -d "../Datfiles/$item" 0.6 0.7
	./read -d "../Datfiles/$item" 0.7 0.8
	./read -d "../Datfiles/$item" 0.8 0.9
	./read -d "../Datfiles/$item" 0.9 1.0
	./read -d "../Datfiles/$item" 0.0 1.0 
done
echo "FINISHED"
