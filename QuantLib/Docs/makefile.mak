
# makefile for QuantLib documentation under Borland C++
#
# $Id$

.autodepend
.silent

# Tools to be used
SED       = sed
DOXYGEN   = doxygen
LATEX     = latex
PDFLATEX  = pdflatex
MAKEINDEX = makeindex
DVIPS     = dvips

# Options
TEX_OPTS = --quiet --pool-size=1000000

# Primary target:
# all docs
all:: tex-files
    cd latex
    $(PDFLATEX) $(TEX_OPTS) refman
    $(MAKEINDEX) refman.idx
    $(PDFLATEX) $(TEX_OPTS) refman
    $(LATEX) $(TEX_OPTS) refman
    $(DVIPS) refman
    cd ..

# HTML documentation only
html: html-offline

html-offline::
    $(DOXYGEN) quantlib.doxy
    copy images\*.jpg html
    copy images\*.png html
    copy images\*.pdf latex
    copy images\*.eps latex

html-online::
    $(SED) -e "s/quantlibfooter.html/quantlibfooteronline.html/" \
           quantlib.doxy > quantlib.doxy.temp
    $(DOXYGEN) quantlib.doxy.temp
    del /Q quantlib.doxy.temp
    copy images\*.jpg html
    copy images\*.png html
    copy images\*.pdf latex
    copy images\*.eps latex

# PDF documentation
pdf:: tex-files
    cd latex
    $(PDFLATEX) $(TEX_OPTS) refman
    $(MAKEINDEX) refman.idx
    $(PDFLATEX) $(TEX_OPTS) refman
    cd ..

# PostScript documentation
ps:: tex-files
    cd latex
    $(LATEX) $(TEX_OPTS) refman
    $(MAKEINDEX) refman.idx
    $(LATEX) $(TEX_OPTS) refman
    $(DVIPS) refman
    cd ..

# Correct LaTeX files to get the right layout
tex-files:: html
    cd latex
    ren refman.tex oldrefman.tex
    $(SED) -e "/Page Index/d" \
           -e "/input{pages}/d" \
           -e "/Page Documentation/d" \
           -e "64,$s/input{index}/ /" \
           -e "64,$s/include{index}/ /" \
           -e "64,$s/input{overview}/ /" \
           -e "64,$s/include{overview}/ /" \
           -e "64,$s/input{where}/ /" \
           -e "64,$s/include{where}/ /" \
           -e "64,$s/input{install}/ /" \
           -e "64,$s/include{install}/ /" \
           -e "64,$s/input{usage}/ /" \
           -e "64,$s/include{usage}/ /" \
           -e "64,$s/input{platforms}/ /" \
           -e "64,$s/include{platforms}/ /" \
           -e "64,$s/input{history}/ /" \
           -e "64,$s/include{history}/ /" \
           -e "64,$s/input{todo}/ /" \
           -e "64,$s/include{todo}/ /" \
           -e "64,$s/input{resources}/ /" \
           -e "64,$s/include{resources}/ /" \
           -e "64,$s/input{group}/ /" \
           -e "64,$s/include{group}/ /" \
           -e "64,$s/input{license}/ /" \
           -e "64,$s/include{license}/ /" \
           -e "64,$s/input{coreclasses}/ /" \
           -e "64,$s/include{coreclasses}/ /" \
           -e "64,$s/input{findiff}/ /" \
           -e "64,$s/include{findiff}/ /" \
           -e "64,$s/input{mcarlo}/ /" \
           -e "64,$s/include{mcarlo}/ /" \
           -e "s/ple Documentation}/ple Documentation}\\\\label{exchap}/" \
           oldrefman.tex > refman.tex
    del oldrefman.tex
    cd ..

# Clean up
clean::
    if exist html  rmdir /S /Q html
    if exist latex rmdir /S /Q latex
