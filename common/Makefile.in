
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header

DBG_COMMON = 
DBGFLAGS   = $(DBG_COMMON)

CFLAGS  = -g -pg -c $(MPAC_CFLAGS) $(DBGFLAGS) 

LDFLAGS = -lmpac -lrt -lm -lpthread
LIBS    = -lmpac -lrt -lm -lpthread

OBJ = 	mpac.o   \
	mpac_time.o   \
	mpac_resolution.o \
	mpac_stats.o \
	mpac_io_file.o \
	mpac_io_net.o \
	mpac_io_tcp.o  \
	mpac_io_udp.o \
	mpac_thread_manager.o \
	mpac_cache.o \
	mpac_signal.o

all: 	libmpac
	$(RM) -f *.o 

libmpac:	$(OBJ)
	$(AR) cr libmpac.a $?
	cp -f libmpac.a $(MPAC_TARGET_DIR)/lib
	cp -f *.h $(MPAC_TARGET_DIR)/include

clean:
	rm -f *.o *~  \
	libmpac.a



mpac.o:	\
	mpac.h \
	mpac.c 
	$(CC) -o mpac.o mpac.c $(CFLAGS) 

mpac_time.o:	\
	mpac.h \
	mpac_time.h \
	mpac_time.c
	$(CC) -o mpac_time.o mpac_time.c $(CFLAGS) 

mpac_resolution.o:	\
	mpac.h \
	mpac_resolution.h \
	mpac_resolution.c
	$(CC) -o mpac_resolution.o mpac_resolution.c $(CFLAGS) 

mpac_stats.o:	\
	mpac.h \
	mpac_stats.h \
	mpac_stats.c
	$(CC) -o mpac_stats.o mpac_stats.c $(CFLAGS) 

mpac_io_file.o:	\
	mpac_io_file.h \
	mpac_io_file.c
	$(CC) -o mpac_io_file.o mpac_io_file.c $(CFLAGS) 

mpac_io_net.o:	\
	mpac_io_net.h \
	mpac_io_net.c
	$(CC) -o mpac_io_net.o mpac_io_net.c $(CFLAGS) 

mpac_io_tcp.o:	\
	mpac_io_tcp.h \
	mpac_io_tcp.c
	$(CC) -o mpac_io_tcp.o mpac_io_tcp.c $(CFLAGS) 

mpac_io_udp.o:	\
	mpac_io_udp.h \
	mpac_io_udp.c
	$(CC) -o mpac_io_udp.o mpac_io_udp.c $(CFLAGS) 

mpac_thread_manager.o:	\
	mpac_thread_manager.h \
	mpac_thread_manager.c
	$(CC) -o mpac_thread_manager.o mpac_thread_manager.c $(CFLAGS) 

mpac_signal.o:	\
	mpac_signal.h \
	mpac_signal.c
	$(CC) -o mpac_signal.o mpac_signal.c $(CFLAGS)

mpac_cache.o:	\
	mpac_cache.h \
	mpac_cache.c
	$(CC) -o mpac_cache.o mpac_cache.c $(CFLAGS)  
