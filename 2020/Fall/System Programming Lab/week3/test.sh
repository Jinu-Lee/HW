#!/bin/bash
make
s=$(($1+$2))
d=$(($1-$2))
echo $s $d > answer.txt
echo $1 $2 | ./output > output.txt 
diff answer.txt output.txt > result.txt
make clean