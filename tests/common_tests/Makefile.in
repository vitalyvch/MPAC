
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header

DBGFLAGS = 

CFLAGS  = -g -c $(DBGFLAGS) $(MPAC_CFLAGS)

LDFLAGS = $(MPAC_LDFLAGS)
LIBS    = -lmpac -lrt -lm -lpthread

all:	tests

tests:	mpac_init_test \
	mpac_time_test \
	mpac_io_file_test \
	mpac_thr_test

clean:	
	rm -f *.o *~ \
	mpac_init_test \
	mpac_time_test \
	mpac_io_file_test \
	mpac_thr_test



mpac_init_test:	\
	mpac_init_test.o \
	$(MPAC_TARGET_DIR)/lib/libmpac.a
	$(CC) -o mpac_init_test mpac_init_test.o $(LDFLAGS) $(LIBS)

mpac_time_test:	\
	mpac_time_test.o \
	$(MPAC_TARGET_DIR)/lib/libmpac.a
	$(CC) -o mpac_time_test mpac_time_test.o $(LDFLAGS) $(LIBS)

mpac_io_file_test:	\
	mpac_io_file_test.o \
	$(MPAC_TARGET_DIR)/lib/libmpac.a
	$(CC) -o mpac_io_file_test mpac_io_file_test.o $(LDFLAGS) $(LIBS)

mpac_thr_test:	\
	mpac_thr_test.o \
	$(MPAC_TARGET_DIR)/lib/libmpac.a
	$(CC) -o mpac_thr_test mpac_thr_test.o $(LDFLAGS) $(LIBS) -lm -lpthread



mpac_init_test.o:	\
	mpac_init_test.c 
	$(CC) -o mpac_init_test.o $(CFLAGS) mpac_init_test.c

mpac_time_test.o:	\
	mpac_time_test.c 
	$(CC) -o mpac_time_test.o $(CFLAGS) mpac_time_test.c

mpac_io_file_test.o:	\
	mpac_io_file_test.c 
	$(CC) -o mpac_io_file_test.o $(CFLAGS) mpac_io_file_test.c

mpac_thr_test.o:	\
	mpac_thr_test.c 
	$(CC) -o mpac_thr_test.o $(CFLAGS) mpac_thr_test.c
