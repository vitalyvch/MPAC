/*
  mpac_io_tcp.h

*/

#ifndef MPAC_IO_TCP_H
#define MPAC_IO_TCP_H


#include "mpac_io_net.h"
#include<netinet/tcp.h>

int mpac_io_tcp_open_sender         (struct mpac_io_net_handler * h);
int mpac_io_tcp_send                (struct mpac_io_net_handler * h);
int mpac_io_tcp_open_receiver       (struct mpac_io_net_handler * h);
int mpac_io_tcp_accept_receiver     (struct mpac_io_net_handler * h);
int mpac_io_tcp_receive             (struct mpac_io_net_handler * h);
int mpac_io_tcp_close               (struct mpac_io_net_handler * h);
int mpac_io_tcp_write               (struct mpac_io_net_handler * h);
int mpac_io_tcp_read                (struct mpac_io_net_handler * h);

#endif
