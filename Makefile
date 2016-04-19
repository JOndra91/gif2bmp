###############################################
# Common settings
#

CC?=gcc
CXX?=g++
LDFLAGS=
CFLAGS=-g -Wall

SRCDIR=src
BUILDDIR=build
BINDIR=bin
LIBDIR=lib

MKDIR=mkdir
RM=rm -rf

CXXFLAGS=

SOURCECXX=

OBJCXX=$(addprefix $(BUILDDIR)/, $(SOURCECXX:%.cpp=%.o))

###############################################
# Make rules
#
.PHONY: all run

all: gif2bmp

run: gif2bmp
	$(BINDIR)/gif2bmp

gif2bmp: $(OBJC) $(OBJCXX) | $(BINDIR)
	$(CXX) $+ -o $@ $(LDFLAGS)

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
