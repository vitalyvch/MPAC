/* 
 * File:   mpac_io_udp.h
 * Author: umair
 *
 * Created on July 15, 2008, 11:49 PM
 */

#ifndef _MPAC_IO_UDP_H
#define	_MPAC_IO_UDP_H

#include "mpac.h"
#include "mpac_io_net.h"

int mpac_io_udp_open_sender     (struct mpac_io_net_handler * h);
int mpac_io_udp_send            (struct mpac_io_net_handler * h);
int mpac_io_udp_open_receiver   (struct mpac_io_net_handler * h);
int mpac_io_udp_receive         (struct mpac_io_net_handler * h);
int mpac_io_udp_close           (struct mpac_io_net_handler * h);



#endif	/* _MPAC_IO_UDP_H */

