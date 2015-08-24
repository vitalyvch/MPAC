#
# Top level makefile for mpac benchmarking suite
#
	
MPAC_BASE_DIR:=$(shell until [ -d makeinc ] ; do cd ..; done; pwd)
INSTALL_DIR:= /usr/local
include $(MPAC_BASE_DIR)/makeinc/makefile.header

	
all:	
	cp -f config.h $(MPAC_BASE_DIR)/common
	make source
	make test
	
source:
	cd common; make all
	cd benchmarks; make all
	cd apps; make all

test:
	cd tests; make all
cell:
	cd cbe; make all

install:
	cp $(MPAC_BASE_DIR)/target/lib/*.a $(INSTALL_DIR)/lib
	cp $(MPAC_BASE_DIR)/benchmarks/net/*.h $(INSTALL_DIR)/include
	cp $(MPAC_BASE_DIR)/benchmarks/mem/*.h $(INSTALL_DIR)/include
	cp $(MPAC_BASE_DIR)/benchmarks/cache/*.h $(INSTALL_DIR)/include
	cp $(MPAC_BASE_DIR)/benchmarks/cpu/*.h $(INSTALL_DIR)/include
	cp $(MPAC_BASE_DIR)/benchmarks/disk/*.h $(INSTALL_DIR)/include
	cp $(MPAC_BASE_DIR)/benchmarks/net/mpac_net_bm $(INSTALL_DIR)/bin
	cp $(MPAC_BASE_DIR)/benchmarks/mem/mpac_mem_bm $(INSTALL_DIR)/bin
	cp $(MPAC_BASE_DIR)/benchmarks/cache/mpac_cache_ctc_bm $(INSTALL_DIR)/bin
	cp $(MPAC_BASE_DIR)/benchmarks/cpu/mpac_cpu_bm $(INSTALL_DIR)/bin
	cp $(MPAC_BASE_DIR)/benchmarks/disk/mpac_disk_bm $(INSTALL_DIR)/bin



clean:
	$(RM) -f *~ 
	$(RM) -f $(MPAC_BASE_DIR)/target/include/*.h
	$(RM) -f $(MPAC_BASE_DIR)/target/lib/*.a
	$(RM) -f $(MPAC_BASE_DIR)/target/lib/*.so
	$(RM) -f $(MPAC_BASE_DIR)/target/bin/*
	cd common; make clean
	cd benchmarks; make clean
	cd tests; make clean
	cd apps; make clean
	cd cbe; make clean

