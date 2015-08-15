
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header


DBG_HELLO   = #-DDBG_MPAC -DDBG_MPAC_HELLO
DBGFLAGS  = $(DBG_XPATH) $(DBG_PARSE)

CFLAGS = -g -c  $(DBGFLAGS) -Wall

LIBDIR = 
INC = $(MPAC_CFLAGS) 
LIBS = $(MPAC_LDFLAGS) -lmpac -lm -lrt -lpthread

OBJ = 	mpac_hello.o \
	mpac_hello_app.o \
	mpac_hello_app_hw.o \

all: 	mpac_hello_app 

mpac_hello_app:	\
	$(OBJ) \
	$(MPAC_TARGET_DIR)/lib/libmpac.a
	$(CC) -o mpac_hello_app $(OBJ) $(LDFLAGS) $(LIBS)
	cp -f mpac_hello_app $(MPAC_TARGET_DIR)/bin

clean:
	$(RM) -f *.o *~ \
	mpac_hello_app 


mpac_hello.o:	mpac_hello.c	mpac_hello.h
	$(CC) mpac_hello.c $(CFLAGS) $(INC) 

mpac_hello_app_hw.o:	mpac_hello.c	mpac_hello.h
	$(CC) mpac_hello_app_hw.c $(CFLAGS) $(INC) $(INC_LIBXML)

mpac_hello_app.o:	\
			mpac_hello_app.c	\
			mpac_hello.h	\
			mpac_hello_app_hw.c
	$(CC) mpac_hello_app.c $(CFLAGS) $(INC) $(INC_LIBXML)
