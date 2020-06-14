#include <stdio.h>
#include <stdint.h>
#include "dhcp.h"
#include "trie.h"

#define RUN_TEST(x) if(1 == x()) { printf("%-22s\033[1;32m Success!\033[0m\n", #x); }\
								else { printf("%-22s\033[0;31m Failed!\033[0m\n", #x); } 



int TrieFlowTest()
{
	int flag = 1;
	uint32_t test = 0;
	enum trie_status status = 0;
	int i = 0;
	int res = 0;
	size_t count = 0;
	
	trie_t *trie = TrieCreate(4);
	
	count = TrieCount(trie);
	if(16 != count)
	{
		flag = 0;
	}
	status = TrieInsert(trie, 1, &test);
	
	/* Insert some addresses, check if ok */
	if(1 != test || T_OK != status)
	{
		flag = 0;
	}
	
	count = TrieCount(trie);
	if(15 != count)
	{
		flag = 0;
	}
	
	status = TrieInsert(trie, 4, &test);
	if(4 != test || T_OK != status)
	{
		flag = 0;
	}
	
	
	count = TrieCount(trie);
	if(14 != count)
	{
		flag = 0;
	}
	
	status = TrieInsert(trie, 15, &test);
	if(15 != test || T_OK != status)
	{
		flag = 0;
	}
	
	count = TrieCount(trie);
	if(13 != count)
	{
		flag = 0;
	}
	
	status = TrieInsert(trie, 0, &test);
	if(0 != test || T_OK != status)
	{
		flag = 0;
	}
	
	count = TrieCount(trie);
	if(12 != count)
	{
		flag = 0;
	}
	
	/* Ask for a taken address */
	
	status = TrieInsert(trie, 15, &test);
	if(T_ADDR_TAKEN != status || 14 != test)
	{	
		flag = 0;
	}
	
	count = TrieCount(trie);
	if(11 != count)
	{
		flag = 0;
	}
	
	/* Take all adresses */
	for(i = 0; i < 11; ++i)
	{
		status = TrieInsert(trie, 0, &test);
		if(T_ADDR_TAKEN != status)
		{
			flag = 0;
		}
		
		count = TrieCount(trie);
		if((10 - i) != (int)count)
		{
			flag = 0;
		}
	}
	
	/* Try inserting now */
	status = TrieInsert(trie, 0, &test);
	if(T_NO_FREE_ADDR != status)
	{
		flag = 0;
	}
	
	/*  Try to remove B"H */
	res = TrieRemove(trie, 15); 
	if(0 != res)
	{
		flag = 0;
	}
	
	count = TrieCount(trie);
	if(1 != count)
	{
		flag = 0;
	}
	
	/*insert 15 again */
	status = TrieInsert(trie, 15, &test);
	if(15 != test || T_OK != status)
	{
		flag = 0;
	}
	
	TrieDestroy(trie);
	
	return flag;
}

int TrieFlowTest1()
{
	int flag = 1;
	uint32_t test = 0;
	enum trie_status status = 0;
	
	size_t count = 0;
	
	trie_t *trie = TrieCreate(2);
	
	count = TrieCount(trie);
	if(4 != count)
	{
		flag = 0;
	}
	
	status = TrieInsert(trie, 1, &test);
	if(T_OK != status)
	{
		flag = 0;
	}
	status = TrieInsert(trie, 0, &test);
	if(T_OK != status)
	{
		flag = 0;
	}
	
	status = TrieInsert(trie, 2, &test);
	if(T_OK != status)
	{
		flag = 0;
	}
	status = TrieInsert(trie, 3, &test);
	if(T_OK != status)
	{
		flag = 0;
	}

	TrieDestroy(trie);
	
	return flag;
}

int CompareIPs(ip_t ip1, ip_t ip2)
{
	int i = 0;
	
	for(i = 0; i < IPV; ++i)
	{
		if(ip1.ip_addr[i] != ip2.ip_addr[i])
		{
			return 0;
		}
	}
	
	return 1;
}

void DisplayIP(ip_t ip)
{
	int i = 0;
	
	for(i = 0; i < IPV - 1; ++i)
	{
		printf("%d.", ip.ip_addr[i]);
	}	
	printf("%d\n", ip.ip_addr[IPV - 1]);
}

int DHCPTest()
{
	int flag = 1;
	int i = 0;
	size_t count = 0;
	int free = 0;
	
	ip_t network_addr;
	ip_t result;
	ip_t req;
	enum dhcp_status status = 0;
	
	dhcp_t *dhcp = NULL;
	
	network_addr.ip_addr[0] = 255;
	network_addr.ip_addr[1] = 255;
	network_addr.ip_addr[2] = 255;
	network_addr.ip_addr[3] = 0;
	
	/* subnet mask: 255.255.255.0 */
	dhcp = DHCPCreate(network_addr, 0xFFFFFF00);
	
	/* Try getting the 0th address, should be taken */
	req.ip_addr[0] = 255;
	req.ip_addr[1] = 255;
	req.ip_addr[2] = 255;
	req.ip_addr[3] = 0;
	
	/* check count */
	count = DHCPCountFree(dhcp);
	if(254 != count)
	{
		flag = 0;
	}	
	
	status = DHCPGetAddr(dhcp, req, &result);
	
	if (ADDR_TAKEN != status)
	{
		flag = 0;
	}
	
	if(result.ip_addr[3] != 1)
	{
		flag = 0;
	}
	
	count = DHCPCountFree(dhcp);
	if (253 != count)
	{
		flag = 0;
	}	
	
	/* Get address 2 */
	req.ip_addr[3] = 2;
	status = DHCPGetAddr(dhcp, req, &result);
	
	if(OK != status || 2 != result.ip_addr[3])
	{
		flag = 0;
	}
	
	count = DHCPCountFree(dhcp);
	if(252 != count)
	{
		flag = 0;
	}
	
	
	for(i = 3; i < 255; ++i)
	{
		req.ip_addr[3] = i;
		status = DHCPGetAddr(dhcp, req, &result);
		
		if(OK != status || i != result.ip_addr[3])
		{
			flag = 0;
		}
	}
	
	/* Check count */
	count = DHCPCountFree(dhcp);
	if(0 != count)
	{
		flag = 0;
	}
	
	/* Try getting address */
	status = DHCPGetAddr(dhcp, req, &result);
	if(NO_FREE_ADDR != status)
	{
		flag = 0;
	}
	
	/* Try Free */
	req.ip_addr[3] = 10;
	
	free = DHCPFreeAddr(dhcp, req);
	
	count = DHCPCountFree(dhcp);
	
	if(0 != free || 1 != count)
	{
		flag = 0;
	}
	/* Get address, should be 10 */
	req.ip_addr[3] = 50;
	
	status = DHCPGetAddr(dhcp, req, &result);
	
	if(ADDR_TAKEN != status || 10 != result.ip_addr[3])
	{
		flag = 0;
	} 
	
	count = DHCPCountFree(dhcp);
	if(0 != count)
	{
		flag = 0;
	}
	
	DHCPDestroy(dhcp);
	
	return flag;

}

int main()
{
/*	RUN_TEST(IPTest);*/
/*	RUN_TEST(TrieFlowTest);*/
	RUN_TEST(DHCPTest);
	
	return 0;
}
