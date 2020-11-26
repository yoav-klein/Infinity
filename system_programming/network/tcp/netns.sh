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
	sudo ip netns exec red ip addr add 192.168.15.1 dev veth-red
	echo "assign ip address to veth-blue"
	sudo ip netns exec blue ip addr add 192.168.15.2 dev veth-blue
	echo "veth-red up"
	sudo ip netns exec red ip link set veth-red up
	echo "veth-blue up"
	sudo ip netns exec blue ip link set veth-blue up

}


