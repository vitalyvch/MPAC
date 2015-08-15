
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header


all:
	make apps


apps:
	cd sniffer; make all
	cd sort; make all
	cd hello; make all				

clean:
	$(RM) -f *~ 
	cd sniffer; make clean
	cd sort; make clean
	cd hello; make clean

