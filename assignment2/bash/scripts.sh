#!/bin/bash
echo 'script #1:'
for i in `seq 100 2 1000`; do echo $i; done

echo 'script #2:'
my_array=(1 2 3 four five Six Se7en "eight" "9" 'TEN')
for i in ${my_array[@]}; do echo $i; done

echo 'script #3:'
bc -l <<< "100 + 0.5"

echo 'script #4:'
FILE=Linux
if test -f "$FILE"; then
    echo 'course'
else
    echo 'very easy'
    echo 'course is easy' > $FILE
fi
