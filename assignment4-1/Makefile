#!/usr/bin/make -f
#
# Filename:		Makefile
# Date:			10/19/2020
# Author:		Garrett Strealy
# Email:		gjs160430@utdallas.edu
# Version:		1.0
# Copyright:		2020, All Rights Reserved
#
# Description:
#
#	Builds scan.l and parse.y into two executables, ./scanner and ./parser

#
# Flags for the C++ implicit rules
CC = gcc
CCFLAGS = -Werror
CPPFLAGS =


# Flags for the Lex implicit rules
#
# Implicit Makefile rule to use Lex for C programs
#
#   n.c is made automatically from n.l by running Lex. The actual recipe is
#	$(LEX) $(LFLAGS)
LEX = /bin/flex
LFLAGS = 

#
# Implicit Makefile rule to use YACC for C programs
#
#   n.c is made automatically from n.y by running Yacc with the recipe
#	$(YACC) $(YFLAGS)
YACC = /bin/bison
YFLAGS = -dy


#
# PROJECTNAME is a descriptive name used for the backup target
# This should not contain spaces or special characters
PROJECTNAME = CS3377.assignment4

EXECFILE = hw4

OBJS = parse.o scan.o hw4.o

# Because we are insane, keep the intermediate files so we can look at
# them If we comment this out (or remove it), the intermediate files
# are  automatically deleted.
.PRECIOUS: scan.c parse.c

all: $(EXECFILE)

clean:
	rm -f $(OBJS) $(EXECFILE) y.tab.h parse.c scan.c scanner parser *~ \#*

$(EXECFILE):	$(OBJS)
	$(CC) -o $@ $(OBJS)
	ln -s ./$(EXECFILE) scanner
	ln -s ./$(EXECFILE) parser

backup:
	@make clean
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename "`pwd`"))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME) 2> /dev/null
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!
