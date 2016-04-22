IMAGES=$(patsubst %.svg,%.pdf, $(wildcard img/*.svg))
DIAGRAMS=$(patsubst %.dia,%.eps, $(wildcard img/*.dia))

all: doc

doc: $(DIAGRAMS) $(IMAGES)
	$(MAKE) -B dokumentace.pdf
	# $(MAKE) -B dokumentace.bbl
	# $(MAKE) -B dokumentace.pdf
	$(MAKE) -B dokumentace.pdf

pres: $(DIAGRAMS) $(IMAGES)
	$(MAKE) -B prezentace.pdf
	$(MAKE) -B prezentace.pdf

include Makefile.mk

clean::
	$(RM) $(IMAGES) $(DIAGRAMS)
