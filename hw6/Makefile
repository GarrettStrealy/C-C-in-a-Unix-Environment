# Filename:		Makefile
# Date:			11/20/2020
# Author:		Garrett Strealy
# Email:		gjs160430@utdallas.edu
# Version:		1.0
# Copyright:		2020, All Rights Reserved
#
# Description:
#
#	Read a binary file and display it using CDK.
#


# Set up needed values to support make's implicit commands
# for compiling C++ source files.

# The compiler to use
CXX = g++

# C pre-processor flags
CPPFLAGS = -I /scratch/perkins/include -std=c++11

# compiler flags
CXXFLAGS = -Wall -Wno-write-strings

# linker flags
LDFLAGS = -L /scratch/perkins/lib

# More linker flags (order dependent)
LDLIBS = -lcdk -lcurses

# Project name.  This is used in building the file name for the backup target
PROJECTNAME = 3377hw6

# Enter the name for the executable to be created
EXEC = hw6

#
# ======================================================
#   Traditionally, we don't need to modify much below here.
# =====================================================
#
SRCS = $(wildcard *.cc)
HEADERS = $(wildcard *.h)
OBJS := $(patsubst %.cc,%.o,$(SRCS))

# These targets don't create files that Make should inspect
.PHONY: all clean backup

# The default target.  Its a good idea to use 'all' as the default
# so that the commands 'make' and 'make all' do the same thing.

all: $(EXEC)

#
# Clean up files that we can regenerate or that we don't care about.

clean:
	rm -f $(OBJS) *.d *~ $(EXEC)


#
# This is more sophisticated than most.  Here, we use the preprocessor
# to create "dependency" files.  The dependency files include all the
# header files that any given .cc file #includes.    The idea is that
# if any header file changes, we need to recompile all the .cc files
# that included the header file.

# Pattern for .d files.
%.d:%.cc
	@echo Updating .d Dependency File
	@set -e; rm -f $@; \
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


#  This is a rule to link the files.  Pretty standard

$(EXEC): $(OBJS)
	$(CXX) -o $(EXEC) $(OBJS) $(LDFLAGS) $(LDLIBS)

# There is magic here.  Read the man page to learn how a make handles
# a target named Makefile.  This is the magic that allows make to "restart"
# if it had to change (or create) any .d files.  The "restart" allows make
# to re-read the dependency files after that were created or changed.

Makefile: $(SRCS:.cc=.d)

# Backup Target.   You should perform a "make backup" command often

backup:	clean
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename `pwd`))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcfv $(MKBKUPNAME) ../$(CURDIRNAME)
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!


# Include the dependency files created by the PreProcessor.  The
# dash in front of the command keeps the system from complaining
# if the files do not exist.  This rule is used in conjunction
# with the magic "Makefile" target above.

-include $(SRCS:.cc=.d)
