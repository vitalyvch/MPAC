PROJ=mem
PCC=ppu-gcc
SCC=spu-gcc
EMBED=ppu-embedspu
LIBSPE=-lspe2

mpac_cell_mem: ppu.c spu.o
	$(PCC)  $^ -lspe2 -o $@
spu.o: spu
	$(EMBED) spu_$(PROJ) spu $@
     
spu: spu.c
	$(SCC) $^ -o $@
	
.PHONY: clean

clean:
	rm *.o mpac_cell_mem spu 
