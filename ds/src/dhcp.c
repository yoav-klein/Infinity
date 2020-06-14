/**********************************************
*		DHCP
*		Author: Yoav Klein
*		Reviewer: Andrei Plak... something
*		Date: 29.7.19
**********************************************/

#include <stdlib.h> /* malloc */
#include <math.h> /* log */
#include <assert.h> /* assert */

#include "trie.h" /* TrieCreate */

#include "dhcp.h"

#define BYTE 8
#define HOST(x, y) ((x) & (~(y)))

struct dhcp
{
	trie_t *ip_pool;
	ip_t network_addr;
	uint32_t subnet_mask;
};

static enum trie_status InitTrie(trie_t *trie, uint32_t max_address);
static size_t CalculateNumBits(uint32_t mask);
static void CopyIpAddresses(unsigned char *dest, unsigned char *src);
static uint32_t ConvertStructToNum(ip_t ip);
static ip_t ConvertNumToStruct(uint32_t ip);

static uint32_t ConvertStructToNum(ip_t ip)
{
	uint32_t result = 0;
	int i = 0;
	
	for (i = 0; i < IPV - 1; ++i)
	{
		result = result | (ip.ip_addr[i]);
		result <<= BYTE;
	}
	result = result | (ip.ip_addr[i]);
	
	return result;
}

static ip_t ConvertNumToStruct(uint32_t ip)
{
	ip_t result;
	int i = 0;
	
	for (i = IPV - 1; i >= 0; --i)
	{
		char temp = 0;
		temp = temp | ip;
		ip >>= BYTE;
		result.ip_addr[i] = temp;
	}
	
	return result;
}

static enum trie_status InitTrie(trie_t *trie, uint32_t max_address)
{
	uint32_t ret_val = 0;
	
	enum trie_status status = TrieInsert(trie, max_address, &ret_val);
	
	if (T_OK != status)
	{
		return status;
	}
	
	#define ZERO_ADDR ((uint32_t)0)
	status = TrieInsert(trie, ZERO_ADDR, &ret_val);
	
	return status;	
}

static size_t CalculateNumBits(uint32_t mask)
{
	return (size_t)(log(~mask + 1) / log(2));
}

static void CopyIpAddresses(unsigned char *dest, unsigned char *src)
{
	int i = 0;
	
	for (i = 0; i < IPV; ++i)
	{
		dest[i] = src[i];
	}
}

dhcp_t *DHCPCreate(ip_t network_addr, uint32_t subnet_mask)
{
	size_t num_of_bits = CalculateNumBits(subnet_mask);
	enum trie_status status = 0;
	
	dhcp_t *dhcp = malloc(sizeof(*dhcp));
	if (NULL == dhcp)
	{
		return NULL;
	}
	
	dhcp->ip_pool = TrieCreate(num_of_bits);
	if (NULL == dhcp->ip_pool)
	{
		free(dhcp);
		
		return NULL;
	}
	
	dhcp->subnet_mask = subnet_mask;
	CopyIpAddresses((dhcp->network_addr).ip_addr, network_addr.ip_addr);
	
	status = InitTrie(dhcp->ip_pool, ~subnet_mask);
	if (T_OK != status)
	{
		TrieDestroy(dhcp->ip_pool);
		free(dhcp);
		
		return NULL;
	}
	
	return dhcp;
}

void DHCPDestroy(dhcp_t *dhcp)
{
	assert(dhcp);
	
	TrieDestroy(dhcp->ip_pool);
	
	free(dhcp);
}

enum dhcp_status DHCPGetAddr(dhcp_t *dhcp, ip_t ip_req, ip_t *ip_res)
{
	uint32_t ip_int_res = 0;
	uint32_t ip_int = ConvertStructToNum(ip_req);
	uint32_t mask = 0;
	enum trie_status status;
	
	assert(dhcp && ip_res);
	
	mask = dhcp->subnet_mask;
	
	status = TrieInsert(dhcp->ip_pool, HOST(ip_int, mask), &ip_int_res);
	
	*ip_res = ConvertNumToStruct((ip_int & mask) | ip_int_res);
	
	return (enum dhcp_status)status;
}

int DHCPFreeAddr(dhcp_t *dhcp, ip_t ip_addr)
{
	uint32_t ip_int = ConvertStructToNum(ip_addr);
	uint32_t mask = 0;
	
	assert(dhcp);
	
	mask = dhcp->subnet_mask;
	
	return TrieRemove(dhcp->ip_pool, HOST(ip_int, mask));
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
	assert(dhcp);
	
	return TrieCount(dhcp->ip_pool);
}



