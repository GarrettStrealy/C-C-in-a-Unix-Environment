#!/bin/bash

# Filename      run.sh
# Date          10/19/2020
# Author        Garrett Strealy
# Email         gjs160430@utdallas.edu
# Course        CS 3377.001 Fall 2020
# Version       1.0
# Copyright     2020, All Rights Reserved

# Description: Run hw4

echo "Running 'hw4' with ./scanner"
./scanner < myinput.txt >> stdout.log

echo "Running 'hw4' with ./parser"
./parser < myinput.txt 1>> stdout.log 2>> stderr.log

echo "Running complete"























