
MPAC_BASE_DIR:=$(shell until [ -d makeinc ]; do cd ..; done; pwd)
include $(MPAC_BASE_DIR)/makeinc/makefile.header

all:
	make tests

tests:
	cd common_tests; make all

clean:
	$(RM) -f *~ 
	cd common_tests; make clean
