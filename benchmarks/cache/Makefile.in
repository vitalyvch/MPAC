
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header


DBG_MEM   = #-DDBG_MPAC -DDBG_MPAC_MEM
DBGFLAGS  = $(DBG_XPATH) $(DBG_PARSE)

CFLAGS = -g -pg -c  $(DBGFLAGS) -Wall -O3

LIBDIR = 
INC = $(MPAC_CFLAGS) 
LIBS = $(MPAC_LDFLAGS) -lmpac -lm -lrt -lpthread

OBJ = 	mpac_cache_ctc.o \
	mpac_cache_ctc_producer.o \
	mpac_cache_ctc_consumer.o \
	mpac_cache_ctc_bm.o \
	


all: 	mpac_cache_ctc_bm 

mpac_cache_ctc_bm:	\
	$(OBJ) \
	$(MPAC_TARGET_DIR)/lib/libmpac.a
	$(CC) -o mpac_cache_ctc_bm $(OBJ) $(LDFLAGS) $(LIBS)
	cp -f mpac_cache_ctc_bm $(MPAC_TARGET_DIR)/bin

clean:
	$(RM) -f *.o *~ \
	mpac_cache_ctc_bm 


mpac_cache_ctc.o:	mpac_cache_ctc.c	mpac_cache_ctc.h
	$(CC) mpac_cache_ctc.c $(CFLAGS) $(INC)

mpac_cache_ctc_producer.o:	mpac_cache_ctc.c	mpac_cache_ctc.h
	$(CC) mpac_cache_ctc_producer.c $(CFLAGS) $(INC)  

mpac_cache_ctc_consumer.o:	mpac_cache_ctc.c	mpac_cache_ctc.h
	$(CC) mpac_cache_ctc_consumer.c $(CFLAGS) $(INC) 

mpac_cache_ctc_bm.o:	\
			mpac_cache_ctc_bm.c	\
			mpac_cache_ctc.h	\
			mpac_cache_ctc_producer.c \
			mpac_cache_ctc_consumer.c
	$(CC) mpac_cache_ctc_bm.c $(CFLAGS) $(INC) $(INC_LIBXML)


