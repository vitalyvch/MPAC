
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header


DBG_CPU   = #-DDBG_MPAC -DDBG_MPAC_CPU
DBGFLAGS  = $(DBG_XPATH) $(DBG_PARSE)

CFLAGS = -g -c  $(DBGFLAGS) -Wall

LIBDIR = 
INC = $(MPAC_CFLAGS) 
LIBS = $(MPAC_LDFLAGS) -lmpac -lm -lrt -lpthread

OBJ = 	mpac_cpu.o \
	mpac_cpu_bm.o \
	mpac_cpu_bm_fl.o \
	mpac_cpu_bm_int.o \
	mpac_cpu_bm_lo.o \


all: 	mpac_cpu_bm 

mpac_cpu_bm:	\
	$(OBJ) \
	$(MPAC_TARGET_DIR)/lib/libmpac.a
	$(CC) -o mpac_cpu_bm $(OBJ) $(LDFLAGS) $(LIBS)
	cp -f mpac_cpu_bm $(MPAC_TARGET_DIR)/bin

clean:
	$(RM) -f *.o *~ \
	mpac_cpu_bm 


mpac_cpu.o:	mpac_cpu.c	mpac_cpu.h
	$(CC) mpac_cpu.c $(CFLAGS) $(INC) 

mpac_cpu_bm_fl.o:	mpac_cpu.c	mpac_cpu.h
	$(CC) mpac_cpu_bm_fl.c $(CFLAGS) $(INC) $(INC_LIBXML)

mpac_cpu_bm_int.o:	mpac_cpu.c	mpac_cpu.h
	$(CC) mpac_cpu_bm_int.c $(CFLAGS) $(INC) $(INC_LIBXML)

mpac_cpu_bm_lo.o:	mpac_cpu.c	mpac_cpu.h
	$(CC) mpac_cpu_bm_lo.c $(CFLAGS) $(INC) $(INC_LIBXML)

mpac_cpu_bm.o:	\
			mpac_cpu_bm.c	\
			mpac_cpu.h	\
			mpac_cpu_bm_fl.c \
			mpac_cpu_bm_int.c \
			mpac_cpu_bm_lo.c
	$(CC) mpac_cpu_bm.c $(CFLAGS) $(INC) $(INC_LIBXML)
