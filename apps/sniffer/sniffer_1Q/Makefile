
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header


DBG_NET   =  -DDBG_MPAC_NET #-DDBG_MPAC
DBGFLAGS  = $(DBG_XPATH) $(DBG_PARSE)

CFLAGS = -g -pg -c  $(DBGFLAGS) -Wall -O3

LIBDIR = 
INC = $(MPAC_CFLAGS) 
LIBS = $(MPAC_LDFLAGS) -lmpac -lm -lrt -lpthread

OBJ = 	mpac_sniffer.o \
	mpac_sniffer_app.o 	\
	

all: 	mpac_sniffer_app 

mpac_sniffer_app:	\
	$(OBJ) \
	$(MPAC_TARGET_DIR)/lib/libmpac.a
	$(CC) -o mpac_sniffer_app $(OBJ) $(LDFLAGS) $(LIBS)
	cp -f mpac_sniffer_app $(MPAC_TARGET_DIR)/bin

clean:
	$(RM) -f *.o *~ \
	mpac_sniffer_app 


mpac_sniffer.o:	mpac_sniffer.c	mpac_sniffer.h
	$(CC) mpac_sniffer.c $(CFLAGS) $(INC)



mpac_sniffer_app.o:		\
			mpac_sniffer_app.c	\
			mpac_sniffer.h		
			$(CC) mpac_sniffer_app.c $(CFLAGS) $(INC) $(INC_LIBXML)
