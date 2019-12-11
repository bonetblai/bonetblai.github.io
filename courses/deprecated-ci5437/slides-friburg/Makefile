TERM          = ws1112
LECTURE       = aip

SCP-USER      = ki
SCP-HOST      = jimballa.informatik.uni-freiburg.de
SCP-DIR       = .public_html/teaching/$(TERM)/$(LECTURE)
SCP-DEST      = $(SCP-USER)@$(SCP-HOST):$(SCP-DIR)/

HANDOUTS4-SUFFIX = -handout4
HANDOUTS1-SUFFIX  = -handout

SHELL = /bin/bash

SCP-USER        = ki
SCP-HOST        = jimballa.informatik.uni-freiburg.de
SCP-DIR         = .public_html/teaching/$(TERM)/$(LECTURE)
SCP-DEST        = $(SCP-USER)@$(SCP-HOST):$(SCP-DIR)/

SLIDES-TEX      = $(wildcard $(LECTURE)[0-9].tex $(LECTURE)[0-9][0-9].tex)
SLIDES          = $(SLIDES-TEX:%.tex=%)
HANDOUTS4       = $(SLIDES:%=%$(HANDOUTS4-SUFFIX))
HANDOUTS1       = $(SLIDES:%=%$(HANDOUTS1-SUFFIX))
SLIDES-DRAFT    = $(SLIDES:%=%-draft)
HANDOUTS4-DRAFT = $(HANDOUTS4:%=%-draft)
HANDOUTS1-DRAFT = $(HANDOUTS1:%=%-draft)
WWW             = $(SLIDES:%=%-www)
CLEAN           = $(SLIDES:%=%-clean)
DISTCLEAN       = $(SLIDES:%=%-distclean)

TEMP         = aux log nav out snm toc

default:
	@echo "No default target. Please type 'make $(LECTURE)11.pdf' or similar."

all: $(SLIDES)

www: $(WWW)

$(WWW): %-www: %.pdf %$(HANDOUTS4-SUFFIX).pdf %$(HANDOUTS1-SUFFIX).pdf
	scp $^ $(SCP-DEST)

$(SLIDES): %: %.pdf %$(HANDOUTS4-SUFFIX).pdf %$(HANDOUTS1-SUFFIX).pdf

$(SLIDES:%=%.pdf): %.pdf: %.tex
	pdflatex $*.tex
	pdflatex $*.tex

$(SLIDES-DRAFT): %-draft:
	pdflatex $*.tex

$(HANDOUTS4:%=%.pdf): %$(HANDOUTS4-SUFFIX).pdf: %.tex
	pdflatex --jobname $*$(HANDOUTS4-SUFFIX) \
	  '\PassOptionsToClass{handout,4-on-1}{gkibeamer}' \
	  '\input{$*}'
	pdflatex --jobname $*$(HANDOUTS4-SUFFIX) \
	  '\PassOptionsToClass{handout,4-on-1}{gkibeamer}' \
	  '\input{$*}'

$(HANDOUTS4-DRAFT): %$(HANDOUTS4-SUFFIX)-draft:
	pdflatex --jobname $*$(HANDOUTS4-SUFFIX) \
	  '\PassOptionsToClass{handout,4-on-1}{gkibeamer}' \
	  '\input{$*}'

$(HANDOUTS1:%=%.pdf): %$(HANDOUTS1-SUFFIX).pdf: %.tex
	pdflatex --jobname $*$(HANDOUTS1-SUFFIX) \
	  '\PassOptionsToClass{handout}{gkibeamer}' \
	  '\input{$*}'
	pdflatex --jobname $*$(HANDOUTS1-SUFFIX) \
	  '\PassOptionsToClass{handout}{gkibeamer}' \
	  '\input{$*}'

$(HANDOUTS1-DRAFT): %$(HANDOUTS1-SUFFIX)-draft:
	pdflatex --jobname $*$(HANDOUTS1-SUFFIX) \
	  '\PassOptionsToClass{handout}{gkibeamer}' \
	  '\input{$*}'

clean: $(CLEAN)
	rm -f *~ \#*

$(CLEAN): %-clean:
	rm -f $(TEMP:%=$*.%)
	rm -f $(TEMP:%=$*$(HANDOUTS4-SUFFIX).%)
	rm -f $(TEMP:%=$*$(HANDOUTS1-SUFFIX).%)
	rm -f *~ \#*

distclean: clean $(DISTCLEAN)

$(DISTCLEAN): %-distclean: %-clean
	rm -f $*.pdf $*$(HANDOUTS4-SUFFIX).pdf $*$(HANDOUTS1-SUFFIX).pdf

$(SLIDES-TEX): gkibeamer.cls macros.tex

dump:
	@echo SLIDES-TEX: $(SLIDES-TEX)
	@echo SLIDES: $(SLIDES)
	@echo HANDOUTS4: $(HANDOUTS4)
	@echo HANDOUTS1: $(HANDOUTS1)
	@echo WWW: $(WWW)
	@echo CLEAN: $(CLEAN)
	@echo DISTCLEAN: $(DISTCLEAN)

.PHONY: default all www clean distclean dump
.PHONY: $(SLIDES) $(WWW) $(CLEAN) $(DISTCLEAN)
.PHONY: $(SLIDES-DRAFT) $(HANDOUTS4-DRAFT) $(HANDOUTS1-DRAFT)
