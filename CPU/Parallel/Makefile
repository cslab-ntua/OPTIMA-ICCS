SHELL = /bin/bash

.PHONY: all clean

all: spmv

CC          = gcc
LD          = ld
CFLAGS      = -g -Wall -Winline -O3 -Wdisabled-optimization -fPIC -fopenmp

# Always add the following flags
MAKE_CPPFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -D__STDC_FORMAT_MACROS
MAKE_LDFLAGS  += 

override CPPFLAGS += $(MAKE_CPPFLAGS)
override LDFLAGS += $(MAKE_LDFLAGS)

LIBS         = -lm -lpthread -lgomp
COMPILE      = $(CC) $(CPPFLAGS) $(CFLAGS)

vector.o: vector.c vector.h
	$(COMPILE) -c $< -o $@

mmf.o: mmf.c mmf.h
	$(COMPILE) -c $< -o $@

spm_csr.o:  spm_csr.c spm_csr.h vector.h mmf.h
	$(COMPILE) -c $< -o $@

spm_csr_omp.o:  spm_csr_omp.c spm_csr_omp.h spm_csr.h vector.h mmf.h
	$(COMPILE) -c $< -o $@

spmv_loops.o: spmv_loops.c spmv_loops.h vector.h
	$(COMPILE) -c $< -o $@

spmv.o: spmv.c spmv_loops.h spm_csr.h spm_csr_omp.h
	$(COMPILE) -c $< -o $@

spmv: spmv.o vector.o mmf.o spm_csr.o spm_csr_omp.o spmv_loops.o
	$(CC) -openmp-link -fopenmp $(LDFLAGS) $(LIBS) $^ -o $@

clean:
	rm -f *.o spmv 
