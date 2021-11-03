function build() {
	gd server.c -o server.out
	gd client.c -o client.out
}

function clean() {
	rm *.out
}

function setup() {
	echo "create veth pair"
	sudo ip link add veth-red type veth peer name veth-blue
	echo "set veth-red to red namespace"
	sudo ip link set veth-red netns red
	echo "set veth-blue to blue namespace"
	sudo ip link set veth-blue netns blue
	echo "assign ip address to veth-red"
	sudo ip netns exec red ip addr add 192.168.15.1/24 dev veth-red
	echo "assign ip address to veth-blue"
	sudo ip netns exec blue ip addr add 192.168.15.2/24 dev veth-blue
	echo "veth-red up"
	sudo ip netns exec red ip link set veth-red up
	echo "veth-blue up"
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
	IP_NS=192.168.15
	IP_RED=$IP_NS.1
	IP_BLUE=$IP_NS.2
	IP_BRIDGE=$IP_NS.5
	
	echo "create bridge interface"
	sudo ip link add bridge type bridge
	
	echo "bring bridge up"
	sudo ip link set dev bridge up
	
	echo "create veth-pairs"
	sudo ip link add veth-red type veth peer name veth-red-br
	sudo ip link add veth-blue type veth peer name veth-blue-br
	
	echo "set the veths to the namespaces"
	sudo ip link set dev veth-red netns red
	sudo ip link set dev veth-blue netns blue
	
	echo "set the other ends of the veths to the bridge"
	sudo ip link set dev veth-red-br  master bridge
	sudo ip link set dev veth-blue-br master bridge
	
	echo "assign ip for interfaces in the namespaces"
	sudo ip netns exec red  ip addr add $IP_RED/24 dev veth-red
	sudo ip netns exec blue ip addr add $IP_BLUE/24 dev veth-blue
	
	echo "set veths up"
	sudo ip netns exec red  ip link set veth-red up
	sudo ip netns exec blue ip link set veth-blue up
	
	echo "set veth peers up"
	sudo ip link set veth-red-br  up
	sudo ip link set veth-blue-br up
	
	echo "assign ip address to the bridge"
	sudo ip addr add $IP_BRIDGE/24 dev bridge
	
	echo "Please enter the network address in CIDR notation"
	netaddr=''
	while [[ ! $netaddr =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]/[0-9]{2}$ ]]; do
		read -p "Network address: " netaddr
	done
	
	echo "add entry in routing tables of namespaces for the local network"
	sudo ip netns exec red ip route add $netaddr via $IP_BRIDGE 
	sudo ip netns exec blue ip route add $netaddr via $IP_BRIDGE
	
	echo "add entry in routing table of namespaces for default"
	sudo ip netns exec red ip route add default via $IP_BRIDGE
	sudo ip netns exec blue ip route add default via $IP_BRIDGE
	
	echo "Enable IPv4 forwarding on the host"
	sudo sysctl -w net.ipv4.ip_forward=1
	
	
	echo "set iptable nat rule to masquerade packets coming from the namespaces"
	sudo iptables -t nat -A POSTROUTING -s $IP_NS.0/24 -j MASQUERADE
}



