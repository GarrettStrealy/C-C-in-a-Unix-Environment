#!/usr/bin/make -f
#
# Filename: Makefile
# Date: 11/09/2020
# Author: Garrett Strealy
# Email: gjs160430@utdallas.edu
# Version: 1.0
# Copyright: 2020, All Rights Reserved
# Description: A moderately complex Makefile that takes care of creating dependencies.
# Additionally it has preprocessor, linker, and library flags

CCFLAGS = -Wall
CPPFLAGS = -I /home/010/g/gj/gjs160430/include
CC = g++

LDFLAGS = -L /home/010/g/gj/gjs160430/lib
LDLIBS = -lrudeconfig

SRCS = test.c
OBJS = $(SRCS:.c=.o)

PROJECTNAME = unixHw5
EXECFILE = program5

$(EXECFILE):$(OBJS)
    $(CC) -static $(CPPFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)
all: $(EXECFILE)
clean:
    rm -f $(OBJS) *.d *~ $(EXECFILE) \#*
backup:
    @make clean
    @mkdir -p ~/backups; chmod 700 ~/backups
    @$(eval CURDIRNAME:=$(shell basename "pwd"))
    @$(eval MKBKUPNAME:=~/backups/$(PROJECTNAME)-$(shell date
+'%Y.%m.%d-%H:%M:%S').tar.gz)
    @echo Writing Backup file to: $(MKBKUPNAME)
    @echo
    @-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME) 2> /dev/null
    @chmod 600 $(MKBKUPNAME)
    @echo
    @echo Done!
