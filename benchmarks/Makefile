
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header


all:
	make benchmarks


benchmarks:
	cd cpu; make all
	cd mem; make all
	cd net; make all
	cd cache; make all
	cd disk; make all

clean:
	$(RM) -f *~ 
	cd cpu; make clean
	cd mem; make clean
	cd net; make clean
	cd cache; make clean
	cd disk; make clean

