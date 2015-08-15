PROJ=cpu
PCC=ppu-gcc
SCC=spu-gcc
EMBED=ppu-embedspu
LIBSPE=-lspe2

mpac_cell_cpu: ppu.c spu.o
	$(PCC)  $^ -lspe2 -o $@
spu.o: spu
	$(EMBED) spu_$(PROJ) spu $@
     
spu: spu.c
	$(SCC) $^ -o $@ -lm
	
.PHONY: clean

clean:
	rm *.o mpac_cell_cpu spu 
