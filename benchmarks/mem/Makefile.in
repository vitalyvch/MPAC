
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header


DBG_MEM   = #-DDBG_MPAC -DDBG_MPAC_MEM
DBGFLAGS  = $(DBG_XPATH) $(DBG_PARSE)

CFLAGS = -g -pg -c  $(DBGFLAGS) -Wall -O3

LIBDIR = 
INC = $(MPAC_CFLAGS) 
LIBS = $(MPAC_LDFLAGS) -lmpac -lm -lrt -lpthread

OBJ = 	mpac_mem.o \
	mpac_mem_mtm_gt.o \
	mpac_mem_mtm_lt.o \
	mpac_mem_bm.o \
	


all: 	mpac_mem_bm 

mpac_mem_bm:	\
	$(OBJ) \
	$(MPAC_TARGET_DIR)/lib/libmpac.a
	$(CC) -o mpac_mem_bm $(OBJ) $(LDFLAGS) $(LIBS)
	cp -f mpac_mem_bm $(MPAC_TARGET_DIR)/bin

clean:
	$(RM) -f *.o *~ \
	mpac_mem_bm 


mpac_mem.o:	mpac_mem.c	mpac_mem.h
	$(CC) mpac_mem.c $(CFLAGS) $(INC)

mpac_mem_mtm_gt.o:	mpac_mem.c	mpac_mem.h
	$(CC) mpac_mem_mtm_gt.c $(CFLAGS) $(INC)  

mpac_mem_mtm_lt.o:	mpac_mem.c	mpac_mem.h
	$(CC) mpac_mem_mtm_lt.c $(CFLAGS) $(INC) 

mpac_mem_bm.o:	\
			mpac_mem_bm.c	\
			mpac_mem.h	\
			mpac_mem_mtm_gt.c \
			mpac_mem_mtm_lt.c
	$(CC) mpac_mem_bm.c $(CFLAGS) $(INC) $(INC_LIBXML)


