###############################################
# Common settings
#

CC?=gcc
CXX?=g++
AR=ar rc

SRCDIR=src
BUILDDIR=build
LIBDIR=lib

MKDIR=mkdir --parent
RM=rm -rf

CFLAGS=-g -Wall
CXXFLAGS=-g -Wall -std=c++11

gifinfo_src=gifinfo.cpp \
	$(addprefix gif/, fStreamReader.cpp header.cpp logicalScreenDescriptor.cpp \
	colorTable.cpp extension.cpp imageDescriptor.cpp imageData.cpp)
gifinfo_obj=$(addprefix $(BUILDDIR)/, $(gifinfo_src:%.cpp=%.o))

gif2ppm_src=gif2ppm.cpp \
	$(addprefix gif/, fStreamReader.cpp header.cpp logicalScreenDescriptor.cpp \
	colorTable.cpp extension.cpp imageDescriptor.cpp imageData.cpp)
gif2ppm_obj=$(addprefix $(BUILDDIR)/, $(gif2ppm_src:%.cpp=%.o))

# gif2bmp_a_src_c=gif2bmp.c
gif2bmp_a_src=gif2bmp.cpp\
	$(addprefix gif/, fileReader.cpp header.cpp logicalScreenDescriptor.cpp \
	colorTable.cpp extension.cpp imageDescriptor.cpp imageData.cpp)
gif2bmp_a_obj=$(addprefix $(BUILDDIR)/, $(gif2bmp_a_src:%.cpp=%.o) \
	$(gif2bmp_a_src_c:%.c=%.o))

gif2bmp_src=main.c
gif2bmp_obj=$(addprefix $(BUILDDIR)/, $(gif2bmp_src:%.c=%.o))

###############################################
# Make rules
#
.PHONY: all run

first: all

all: gifinfo gif2ppm $(LIBDIR)/libgif2bmp.a gif2bmp

run: gif2bmp
	gif2bmp

$(LIBDIR)/libgif2bmp.a: $(gif2bmp_a_obj)
	@ $(MKDIR) $(dir $@)
	$(AR) $@ $+

gif2bmp: $(LIBDIR)/libgif2bmp.a $(gif2bmp_obj)
	$(CC) $(gif2bmp_obj) -L$(LIBDIR) -lgif2bmp -lstdc++ -o $@

gifinfo: $(gifinfo_obj)
	$(CXX) $+ -o $@

gif2ppm: $(gif2ppm_obj)
	$(CXX) $+ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@ $(MKDIR) $(dir $@)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

# $(BUILDDIR)/gif2bmp.o: $(SRCDIR)/gif2bmp.c
# 	@ $(MKDIR) $(dir $@)
# 	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@ $(MKDIR) $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	$(RM) $(BUILDDIR) $(BINDIR) $(LIBDIR)
