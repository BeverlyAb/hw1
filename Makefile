<<<<<<< HEAD
CC = g++-7 -fopenmp -lc++
=======
CC = g++ -fopenmp #HPC #gcc-7 -fopenmp -lc++ #(MAC)
>>>>>>> 63d6f0fe2460780df03597a8bd0180ddbd42de77
CFLAGS =
COPTFLAGS = -O3 -g
LDFLAGS =

default:
	@echo "=================================================="
	@echo "To build your OpenMP code, use:"
	@echo "  make mergesort-omp    # For Mergesort"
	@echo ""
	@echo "To clean this subdirectory (remove object files"
	@echo "and other junk), use:"
	@echo "  make clean"
	@echo "=================================================="

# Mergesort driver using OpenMP
mergesort-omp: driver.o sort.o parallel-mergesort.o sequential-mergesort.o
	$(CC) $(COPTFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) $(COPTFLAGS) -o $@ -c $<

clean:
	rm -f core *.o *~ mergesort-omp 

cleanA:
	rm core* mergesort.e*

run:
	make clean
	make mergesort-omp
<<<<<<< HEAD
	./mergesort-omp 100
=======
	qsub ./mergesort.sh
>>>>>>> 63d6f0fe2460780df03597a8bd0180ddbd42de77

show:
	cat mergesort.o*
# eof
