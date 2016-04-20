###############################################
# Common settings
#

CC?=gcc
CXX?=g++
LDFLAGS=
CFLAGS=-g -Wall

SRCDIR=src
BUILDDIR=build
LIBDIR=lib

MKDIR=mkdir --parent
RM=rm -rf

CXXFLAGS=-std=c++11

SOURCECXX=

OBJCXX=$(addprefix $(BUILDDIR)/, $(SOURCECXX:%.cpp=%.o))

gifinfo_src=gifinfo.cpp \
	$(addprefix gif/, fileReader.cpp header.cpp logicalScreenDescriptor.cpp \
	colorTable.cpp extension.cpp imageDescriptor.cpp)
gifinfo_obj=$(addprefix $(BUILDDIR)/, $(gifinfo_src:%.cpp=%.o))

###############################################
# Make rules
#
.PHONY: all run gifinfo

first: a gifinfo

a:
	echo $(gifinfo_obj)
	echo $(dir $(gifinfo_obj))

all: gif2bmp

run: gif2bmp
	gif2bmp

gif2bmp: $(OBJC) $(OBJCXX)
	$(CXX) $+ -o $@ $(LDFLAGS)

gifinfo: $(gifinfo_obj)
	$(CXX) $+ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@ $(MKDIR) $(dir $@)
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) -o $@ $<

clean:
	$(RM) $(BUILDDIR) $(BINDIR) $(LIBDIR)
