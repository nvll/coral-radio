#!/bin/sh

for i in `ls projects/*.mk`;
    do make `basename $i .mk` -j;
done
