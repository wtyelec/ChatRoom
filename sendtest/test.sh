#!/bin/bash
for ((i=0;i<1000;i++));do
    ./client.o 3 $i &
    sleep 0.1
done
