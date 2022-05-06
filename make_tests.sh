#!/bin/bash
ios="`seq 1000 1000 10000`"
ms="`seq 100 100 2000`"
SCHEDS="i j s c f"
numi=0

for i in ${ios}; do
	for m in ${ms}; do
		/home/frankeh/Public/iomake -v -t ${m} -i ${i} > input_gen/input${numi}
		((numi+=1))
	done
done

