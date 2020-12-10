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

function setup_bridge()
{
	echo "create bridge interface"
	sudo ip link add bridge type bridge
	echo "bring bridge up"
	sudo ip link set dev bridge up
	echo "create veth-pair for red namespace"
	sudo ip link add veth-red type veth peer name veth-red-br
	echo "create veth-pair for blue namespace"
	sudo ip link add veth-blue type veth peer name veth-blue-br
	echo "set the veth-red interface to red namespace"
	sudo ip link set veth-red netns red
	echo "set the veth-red-br to the bridge"
	sudo ip link set veth-red-br master bridge
	echo "set the veth-blue interface to blue namespace"
	sudo ip link set veth-blue netns blue
	echo "set the veth-blue-be to the bridge"
	sudo ip link set veth-blue-br master bridge
	echo "assign ip for red interface"
	sudo ip netns exec red ip addr add 192.168.15.1/24 dev veth-red
	echo "assing ip for blue interface"
	sudo ip netns exec blue ip addr add 192.168.15.2/24 dev veth-blue
	echo "veth-red up"
	sudo ip netns exec red ip link set veth-red up
	echo "veth-red-br up"
	sudo ip link set veth-red-br up
	echo "veth-blue up"
	sudo ip netns exec blue ip link set veth-blue up
	echo "veth-blue-br up"
	sudo ip link set veth-blue-br up
	echo "assign ip address to the bridge"
	sudo ip addr add 192.168.15.5/24 dev bridge
	
}
