
netaddr=''

IP_NS=192.168.15
IP_RED=$IP_NS.2

echo $IP_RED

while [[ ! $netaddr =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]$ ]]; do
	read -p "Network address: " netaddr
done



