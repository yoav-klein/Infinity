
namespaces=("red" "blue" "green")

for((i=0; i<${#namespaces[@]}; i++)); do
	echo $i
done
