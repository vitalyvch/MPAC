
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header


DBG_NET   =  -DDBG_MPAC_NET #-DDBG_MPAC
DBGFLAGS  = $(DBG_XPATH) $(DBG_PARSE)

CFLAGS = -g -pg -c  $(DBGFLAGS) -Wall -O3

LIBDIR = 
INC = $(MPAC_CFLAGS) 
LIBS = $(MPAC_LDFLAGS) -lmpac -lm -lrt -lpthread

OBJ = 	mpac_net.o \
	mpac_net_tcpsender.o \
	mpac_net_udpsender.o \
	mpac_net_tcpreceiver.o \
	mpac_net_udpreceiver.o \
	mpac_net_tcpsender_timed.o \
	mpac_net_udpsender_timed.o \
	mpac_net_tcpreceiver_timed.o \
	mpac_net_udpreceiver_timed.o \
	mpac_net_bm.o 	\
	


all: 	mpac_net_bm 

mpac_net_bm:	\
	$(OBJ) \
	$(MPAC_TARGET_DIR)/lib/libmpac.a
	$(CC) -o mpac_net_bm $(OBJ) $(LDFLAGS) $(LIBS)
	cp -f mpac_net_bm $(MPAC_TARGET_DIR)/bin

clean:
	$(RM) -f *.o *~ \
	mpac_net_bm 


mpac_net.o:	mpac_net.c	mpac_net.h
	$(CC) mpac_net.c $(CFLAGS) $(INC)

mpac_net_tcpsender.o:	mpac_net.c	mpac_net.h
	$(CC) mpac_net_tcpsender.c $(CFLAGS) $(INC)  

mpac_net_udpsender.o:	mpac_net.c	mpac_net.h
	$(CC) mpac_net_udpsender.c $(CFLAGS) $(INC) 

mpac_net_tcpreceiver.o:	mpac_net.c	mpac_net.h
	$(CC) mpac_net_tcpreceiver.c $(CFLAGS) $(INC)  

mpac_net_udpreceiver.o:	mpac_net.c	mpac_net.h
	$(CC) mpac_net_udpreceiver.c $(CFLAGS) $(INC) 

mpac_net_tcpsender_timed.o:	mpac_net.c	mpac_net.h
	$(CC) mpac_net_tcpsender_timed.c $(CFLAGS) $(INC)  

mpac_net_udpsender_timed.o:	mpac_net.c	mpac_net.h
	$(CC) mpac_net_udpsender_timed.c $(CFLAGS) $(INC) 

mpac_net_tcpreceiver_timed.o:	mpac_net.c	mpac_net.h
	$(CC) mpac_net_tcpreceiver_timed.c $(CFLAGS) $(INC)  

mpac_net_udpreceiver_timed.o:	mpac_net.c	mpac_net.h
	$(CC) mpac_net_udpreceiver_timed.c $(CFLAGS) $(INC) 

mpac_net_bm.o:		\
			mpac_net_bm.c	\
			mpac_net.h	\
			mpac_net_tcpsender.c	\
			mpac_net_udpsender.c	\
			mpac_net_tcpreceiver.c	\
			mpac_net_udpreceiver.c	\
			mpac_net_tcpsender_timed.c	\
			mpac_net_udpsender_timed.c	\
			mpac_net_tcpreceiver_timed.c	\
			mpac_net_udpreceiver_timed.c							
		$(CC) mpac_net_bm.c $(CFLAGS) $(INC) $(INC_LIBXML)
