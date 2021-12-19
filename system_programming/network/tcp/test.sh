
declare -a namespaces

ns_name=''
while [[ $ns_name != "0" ]]; do
	read -p "New namespace name: " ns_name
	if [ $ns_name == "0" ]; then
		continue
	fi
	echo "New namespace: $ns_name"
	namespaces[${#namespaces[@]}]=$ns_name
done

for ns in ${namespaces[@]}; do
	echo $ns
done

