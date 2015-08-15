
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header


DBG_SORT   = #-DDBG_MPAC -DDBG_MPAC_SORT
DBGFLAGS  = $(DBG_XPATH) $(DBG_PARSE)

CFLAGS = -g -c  $(DBGFLAGS) -Wall

LIBDIR = 
INC = $(MPAC_CFLAGS) 
LIBS = $(MPAC_LDFLAGS) -lmpac -lm -lrt -lpthread

OBJ = 	mpac_sort.o \
	mpac_sort_app.o \

all: 	mpac_sort_app 

mpac_sort_app:	\
	$(OBJ) \
	$(MPAC_TARGET_DIR)/lib/libmpac.a
	$(CC) -o mpac_sort_app $(OBJ) $(LDFLAGS) $(LIBS)
	cp -f mpac_sort_app $(MPAC_TARGET_DIR)/bin

clean:
	$(RM) -f *.o *~ \
	mpac_sort_app 

mpac_sort.o:	mpac_sort.c	mpac_sort.h
	$(CC) mpac_sort.c $(CFLAGS) $(INC) 

mpac_sort_app.o:	\
			mpac_sort_app.c	\
			mpac_sort.h	
	$(CC) mpac_sort_app.c $(CFLAGS) $(INC) $(INC_LIBXML)
