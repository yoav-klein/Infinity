/******************************************
*                _____  __     _   ___    *
*               (  _  )(  )   / ) / _ \   *
*    |\---/|     )(_)(  )(__ / _ \\_  /   *
*    | ,_, |    (_____)(____)\___/ (_/    *
*     \_`_/-..----.                       *
*  ___/ `   ' ,""+ \                      *
* (__...'   __\    |`.___.';              *
*  (_,...'(_,.`__)/'.....+                *
*                                         *
*             Exercise: DHCP              *
******************************************/
#ifndef OL69_DHCP_H
#define OL69_DHCP_H

#define IPV (4) /* implementation of 4 byte IP address space */

#include <stddef.h> /* size_t */
#include <stdint.h> /* uint32_t */

typedef struct
{
	unsigned char ip_addr[IPV];
} ip_t;



enum dhcp_status
{
	OK, /* user got the IP he asked for */
	ADDR_TAKEN, /* user got an IP, not one he asked for */
	NO_FREE_ADDR, /* user didn't get an IP */
	E_ALLOC /* couldn't allocate memory */
};

typedef struct dhcp dhcp_t;

/* user is assumed to have passed legal network address */
dhcp_t *DHCPCreate(ip_t network_addr, uint32_t subnet_mask);

/* undefined behavior for NULL parameter */
void DHCPDestroy(dhcp_t *dhcp);

/* undefined behavior for NULL parameter,
user is assumed to have passed ip_req of the correct subnet */
enum dhcp_status DHCPGetAddr(dhcp_t *dhcp, ip_t ip_req, ip_t *ip_res);

/* undefined behavior for NULL parameter,
return status: 0 if found and removed IP */
int DHCPFreeAddr(dhcp_t *dhcp, ip_t ip_addr);

/* undefined behavior for NULL parameter */
size_t DHCPCountFree(const dhcp_t *dhcp);

#endif /* OL69_DHCP_H */
