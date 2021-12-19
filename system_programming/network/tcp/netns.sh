
RED="\e[31m"
GREEN="\e[32m"
END="\e[0m"

function LOG() {
	echo -e "${GREEN}******* $1 *******${END}"
}

function build() {
	gd server.c -o server.out
	gd client.c -o client.out
}

function clean() {
	rm *.out
}

function setup_simple() {
	LOG "create veth pair"
	sudo ip link add veth-red type veth peer name veth-blue
	LOG "set veth-red to red namespace"
	sudo ip link set veth-red netns red
	LOG "set veth-blue to blue namespace"
	sudo ip link set veth-blue netns blue
	LOG "assign ip address to veth-red"
	sudo ip netns exec red ip addr add 192.168.15.1/24 dev veth-red
	LOG "assign ip address to veth-blue"
	sudo ip netns exec blue ip addr add 192.168.15.2/24 dev veth-blue
	LOG "veth-red up"
	sudo ip netns exec red ip link set veth-red up
	LOG "veth-blue up"
	sudo ip netns exec blue ip link set veth-blue up

}

function clean_all()
{
	sudo ip netns delete red
	sudo ip netns delete blue
	sudo ip link delete veth-red-br
	sudo ip link delete veth-blue-br
	sudo ip link delete bridge
}

function create_ns()
{
	sudo ip netns add blue
	sudo ip netns add red
}

function setup_bridge()
{
	echo "Enter the network address of the bridge network in the form: x.y.z"
	bridge_network=''
	while [[ ! $bridge_network =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]]; do
		read -p "Bridge network address: " bridge_network
	done
	
	read -p "Enter bridge name: " bridge_name
	LOG "create bridge interface"
	sudo ip link add ${bridge_name} type bridge
	
	LOG "bring bridge up"
	sudo ip link set dev ${bridge_name} up
	
	declare -a namespaces
	ns_name=''
	while [[ $ns_name != "0" ]]; do
		read -p "Enter namespace name, 0 to stop: " ns_name
		if [ $ns_name == "0" ]; then
			continue
		fi
		
		i=${#namespaces[@]}
		echo "New namespace: $ns_name"
		namespaces[$i]=$ns_name
		
		LOG "creating namespace: $ns_name"
		sudo ip netns add $ns_name
		LOG "create veth pair for namespace $ns_name"
		veth_name="veth-$ns_name"
		sudo ip link add ${veth_name}  \
		type veth peer name ${veth_name}-br
		LOG "setting the ${veth_name} in the namespace"
		sudo ip link set dev ${veth_name} netns ${namespaces[$i]}
		LOG "setting the other end to the bridge"
		sudo ip link set dev ${veth_name}-br master ${bridge_name}
		LOG "assigning IP for the interface in the namespace"
		sudo ip netns exec ${namespaces[$i]} ip addr add dev ${veth_name} \
		${bridge_network}.$((${i}+1))/24
		LOG "setting ${veth_name} up"
		sudo ip netns exec ${namespaces[$i]} ip link set ${veth_name} up
		LOG "setting ${veth_name}-br up"
		sudo ip link set ${veth_name}-br up
		
	done
	
	LOG "assign ip address to the bridge"
	bridge_ip=${bridge_network}.$((${#namespaces[@]}+1))
	sudo ip addr add $bridge_ip/24 dev ${bridge_name}
	
	echo "Please enter the local network address in CIDR notation"
	netaddr=''
	while [[ ! $netaddr =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]/[0-9]{2}$ ]]; do
		read -p "Network address: " netaddr
	done
	
	for((i=0; i<${#namespaces[@]}; i++)); do
		LOG "add entry in routing table of namespace: ${namespaces[$i]}"
		sudo ip netns exec ${namespaces[$i]} ip route add $netaddr via $bridge_ip		
		sudo ip netns exec ${namespaces[$i]} ip route add default via $bridge_ip
	done
	
	echo "Enable IPv4 forwarding on the host"
	sudo sysctl -w net.ipv4.ip_forward=1
	
	echo "set iptable nat rule to masquerade packets coming from the namespaces"
	sudo iptables -t nat -A POSTROUTING -s $bridge_network.0/24 -j MASQUERADE
}



