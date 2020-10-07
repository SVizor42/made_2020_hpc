#!/bin/bash

N=(512 1024 2048 4096)

make OPT= DBGCFLAGS=-g EXECUTABLE=matmul_g
for i in ${N[@]} 
do
	echo "matmul_g " $i
	./matmul_g $i
done

make clean
make OPT=-O3 DBGCFLAGS= EXECUTABLE=matmul_O3
for i in ${N[@]} 
do
	echo "matmul_O3 " $i
	./matmul_O3 $i
done

