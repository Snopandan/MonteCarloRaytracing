######################################################################
#      MAKE CHANGES TO THESE VARIABLES TO ADAPT TO YOUR PROJECT      #
######################################################################
# Specify name of executable (don't include .exe if on windows).
EXECUTABLE = main

# Specify file extension for source files.
FILE_EXT = cpp

# Specify which compiler to use.
ifeq ($(OS), Windows_NT)
	CXX = g++
else
	CXX = g++
endif

# Specify if you want to use gccfilter or not (YES or NO)
GCCFILTER = NO

# Specify include directories.
INCDIR = -I"include" -I"src"

# Specify directory for source files.
SRCDIR = src

# Specify directory for object files. Must be created manually. (NOT CURRENTLY BEING USED)
OBJDIR = obj

# Specify directory for dependency files. Must be created manually.
DEPDIR = dep

# Specify compiler flags.
CXXFLAGS = -c -std=c++11 -pedantic -Wall -Wextra $(INCDIR)

# Specify more compiler flags. MMD and MP for dependency generation.
CPPFLAGS = -MMD -MP

# Specify linker flags.
LDFLAGS = -L"lib"

# Specify libraries.
ifeq ($(OS), Windows_NT)
	LDLIBS =
else
	LDLIBS =
endif

######################################################################
#                  NO NEED TO CHANGE THE FOLLOWING                   #
######################################################################
# Add gccfilter if it is specified
ifeq ($(GCCFILTER), YES)
	CXX := $(addprefix gccfilter -c -a , $(CXX))
endif

# Set current directory.
SELF_DIR = $(dir $(lastword $(MAKEFILE_LIST)))

#Define a function for recursive wildcard.
rwildcard = $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

# Set source files.
SRCFILES = $(call rwildcard,,$(SRCDIR)/*.$(FILE_EXT))
SRCFILESNOPATH = $(foreach file,${SRCFILES},$(notdir ${file}))

# Set object files filetage.
OBJTAG = o

# Set object files.
OBJFILES = $(SRCFILES:.$(FILE_EXT)=.$(OBJTAG))

# Set dependency files filetage.
DEPTAG = d

# Set dependency files.
DEPFILES = $(DEPDIR)/*.$(DEPTAG)

# Adds .exe extension if compiling on windows.
ifeq ($(OS), Windows_NT)
	EXECUTABLE := $(addsuffix .exe,$(EXECUTABLE))
endif

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJFILES)
	$(CXX) $(LDFLAGS) $(OBJFILES) $(LDLIBS) -o $@

%.$(OBJTAG): %.$(FILE_EXT)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MF $(addprefix $(DEPDIR)/, $(notdir $*.$(DEPTAG))) $< -c -o $@

-include $(DEPFILES)

ifeq ($(OS), Windows_NT)
.PHONY: clean
clean:
	@clean
	@echo All .$(OBJTAG) files and executables erased.
else
.PHONY: clean
clean:
	@$(RM) $(OBJFILES) *.$(OBJTAG)
	@$(RM) $(DEPFILES) *.$(DEPTAG)
	@$(RM) $(EXECUTABLE)
	@echo All .$(OBJTAG) files and executables erased.
endif

.PHONY: dist
dist:
	@zip -q dist.zip $(EXECUTABLE)

.PHONY: archive
archive:
	@cd $(SRCDIR); zip -q -r ../archive.zip .
	@zip -q -d archive.zip *.$(OBJTAG)

.PHONY: doc
doc:
	@doxygen Doxyfile
