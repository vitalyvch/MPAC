
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header


all:
	make apps


apps:
	cd sniffer_1Q; make all
	cd sniffer_MQ; make all
	cd sniffer_MQ_optimized; make all

clean:
	$(RM) -f *~ 
	cd sniffer_1Q; make clean
	cd sniffer_MQ; make clean
	cd sniffer_MQ_optimized; make clean

