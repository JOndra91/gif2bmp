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

MKDIR=mkdir
RM=rm -rf

CXXFLAGS=-std=c++11

SOURCECXX=

OBJCXX=$(addprefix $(BUILDDIR)/, $(SOURCECXX:%.cpp=%.o))

gifinfo_src=gifinfo.cpp fileReader.cpp header.cpp logicalScreenDescriptor.cpp \
	colorTable.cpp
gifinfo_obj=$(addprefix $(BUILDDIR)/, $(gifinfo_src:%.cpp=%.o))

###############################################
# Make rules
#
.PHONY: all run gifinfo

first: gifinfo

all: gif2bmp

run: gif2bmp
	gif2bmp

gif2bmp: $(OBJC) $(OBJCXX) | $(BINDIR)
	$(CXX) $+ -o $@ $(LDFLAGS)

gifinfo: $(gifinfo_obj) | $(BUILDDIR)
	$(CXX) $+ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) -o $@ $<

$(BUILDDIR):
	$(MKDIR) $(BUILDDIR)

$(BINDIR):
	$(MKDIR) $(BINDIR)

$(LIBDIR):
	$(MKDIR) $(LIBDIR)

clean:
	$(RM) $(BUILDDIR) $(BINDIR) $(LIBDIR)
