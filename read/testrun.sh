for item in "TEST"
do
	echo "LOOKING AT CUBE $item ..."
	cd ../read/
	./read -c "../Datfiles/$item" 0.0 0.1
	./read -c "../Datfiles/$item" 0.1 0.2
	./read -c "../Datfiles/$item" 0.2 0.3
	./read -c "../Datfiles/$item" 0.3 0.4
	./read -c "../Datfiles/$item" 0.4 0.5
	./read -c "../Datfiles/$item" 0.5 0.6
	./read -c "../Datfiles/$item" 0.6 0.7
	./read -c "../Datfiles/$item" 0.7 0.8
	./read -c "../Datfiles/$item" 0.8 0.9
	./read -c "../Datfiles/$item" 0.9 1.0
	./read -c "../Datfiles/$item" 0.0 1.0 
done
echo "FINISHED"
