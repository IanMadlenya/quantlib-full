#
# Tentative makefile for QuantLib under Borland C++
#

# Directories
OUTPUT_DIR		= .\Release
PYTHON_DIR		= ..\Python
SWIG_DIR		= ..\Swig
SOURCES_DIR		= ..\Sources
PYTHON_INCLUDE	= "C:\Program Files\Python\include"
PYTHON_LIBS		= "C:\Program Files\Python\libs"
BCC_INCLUDE		= "D:\Program Files\Borland\Bcc55\include"
BCC_LIBS		= "D:\Program Files\Borland\Bcc55\lib"

# Object files
CORE_OBJS		= $(OUTPUT_DIR)\calendar.obj $(OUTPUT_DIR)\date.obj $(OUTPUT_DIR)\normaldistribution.obj $(OUTPUT_DIR)\solver1d.obj $(OUTPUT_DIR)\statistics.obj
CALENDAR_OBJS	= $(OUTPUT_DIR)\westerncalendar.obj $(OUTPUT_DIR)\frankfurt.obj $(OUTPUT_DIR)\london.obj $(OUTPUT_DIR)\milan.obj $(OUTPUT_DIR)\newyork.obj $(OUTPUT_DIR)\target.obj 
DAYCOUNT_OBJS	= $(OUTPUT_DIR)\actualactual.obj $(OUTPUT_DIR)\thirty360.obj $(OUTPUT_DIR)\thirty360italian.obj
OPERATOR_OBJS	= $(OUTPUT_DIR)\tridiagonaloperator.obj $(OUTPUT_DIR)\blackscholesmerton.obj
PRICER_OBJS		= $(OUTPUT_DIR)\bsmnumericaloption.obj $(OUTPUT_DIR)\bsmeuropeanoption.obj $(OUTPUT_DIR)\bsmamericanoption.obj
SOLVER1D_OBJS	= $(OUTPUT_DIR)\bisection.obj $(OUTPUT_DIR)\brent.obj $(OUTPUT_DIR)\falseposition.obj $(OUTPUT_DIR)\newton.obj $(OUTPUT_DIR)\newtonsafe.obj $(OUTPUT_DIR)\ridder.obj $(OUTPUT_DIR)\secant.obj
TERMSTRUC_OBJS	= $(OUTPUT_DIR)\piecewiseconstantforwards.obj 
WIN_OBJS		= c0d32.obj 

# Libraries
WIN_LIBS 		= import32.lib cw32mt.lib
PYTHON_BCC_LIB	= bccpython.lib

# Tools to be used
CC			= bcc32
LINK		= ilink32
COFF2OMF	= coff2omf
SWIG		= swig1.3a5

# Options
CC_OPTS		= -q -c -tWM -n$(OUTPUT_DIR) -I"..\Sources" -I"..\Sources\Calendars" -I"..\Sources\Currencies" -I"..\Sources\DayCounters" -I"..\Sources\Instruments" -I"..\Sources\Operators" -I"..\Sources\PDE" -I"..\Sources\Pricers" -I"..\Sources\Solvers1D" -I"..\Sources\TermStructures" -I$(PYTHON_INCLUDE) -I$(BCC_INCLUDE) -w-8027
LINK_OPTS	= -q -x -L$(BCC_LIBS)

# Generic rules
.cpp.obj:
    @$(CC) $(CC_OPTS) $<

# Primary target:
# Python module
$(PYTHON_DIR)\QuantLibc.dll::	$(OUTPUT_DIR)\quantlib_wrap.obj QuantLib $(PYTHON_BCC_LIB)
	@echo Linking Python module...
	@$(LINK) $(LINK_OPTS) -Tpd $(OUTPUT_DIR)\quantlib_wrap.obj $(CORE_OBJS) $(CALENDAR_OBJS) $(DAYCOUNT_OBJS) $(OPERATOR_OBJS) $(PRICER_OBJS) $(SOLVER1D_OBJS) $(TERMSTRUC_OBJS) $(WIN_OBJS),$(PYTHON_DIR)\QuantLibc.dll,, $(PYTHON_BCC_LIB) $(WIN_LIBS), QuantLibc.def
	@del $(PYTHON_DIR)\QuantLibc.ilc
	@del $(PYTHON_DIR)\QuantLibc.ild
	@del $(PYTHON_DIR)\QuantLibc.ilf
	@del $(PYTHON_DIR)\QuantLibc.ils
	@del $(PYTHON_DIR)\QuantLibc.tds
	@echo Build completed

# Python lib in OMF format
$(PYTHON_BCC_LIB):
	@$(COFF2OMF) $(PYTHON_LIBS)\python15.lib $(PYTHON_BCC_LIB)

# Wrapper functions
$(OUTPUT_DIR)\quantlib_wrap.obj:: $(PYTHON_DIR)\quantlib_wrap.cpp
	@echo Compiling wrappers...
	@$(CC) $(CC_OPTS) -w-8057 -w-8004 -w-8060 -D__WIN32__ -DMSC_CORE_BC_EXT $(PYTHON_DIR)\quantlib_wrap.cpp
$(PYTHON_DIR)\quantlib_wrap.cpp:: $(SWIG_DIR)\QuantLib.i
	@echo Generating wrappers...
	@$(SWIG) -python -c++ -shadow -keyword -opt -I$(SWIG_DIR) -o $(PYTHON_DIR)\quantlib_wrap.cpp $(SWIG_DIR)\QuantLib.i
	@copy .\QuantLib.py $(PYTHON_DIR)\QuantLib.py
	@del .\QuantLib.py
$(SWIG_DIR)\QuantLib.i: $(SOURCES_DIR)\qldefines.h $(SWIG_DIR)\Date.i $(SWIG_DIR)\Calendars.i $(SWIG_DIR)\DayCounters.i $(SWIG_DIR)\Currencies.i $(SWIG_DIR)\Financial.i $(SWIG_DIR)\Options.i $(SWIG_DIR)\Instruments.i $(SWIG_DIR)\Operators.i $(SWIG_DIR)\Pricers.i $(SWIG_DIR)\Solvers1D.i $(SWIG_DIR)\TermStructures.i
	@touch $(SWIG_DIR)\QuantLib.i
$(SWIG_DIR)\Date.i: $(SOURCES_DIR)\date.h $(SOURCES_DIR)\stringconverters.h $(SOURCES_DIR)\formats.h
	@touch $(SWIG_DIR)\Date.i
$(SWIG_DIR)\Calendars.i: $(SWIG_DIR)\Date.i $(SOURCES_DIR)\calendar.h $(SOURCES_DIR)\Calendars\target.h $(SOURCES_DIR)\Calendars\newyork.h $(SOURCES_DIR)\Calendars\london.h $(SOURCES_DIR)\Calendars\frankfurt.h $(SOURCES_DIR)\Calendars\milan.h 
	@touch $(SWIG_DIR)\Calendars.i
$(SWIG_DIR)\DayCounters.i: $(SWIG_DIR)\Date.i $(SOURCES_DIR)\daycounter.h $(SOURCES_DIR)\DayCounters\actual360.h $(SOURCES_DIR)\DayCounters\actual365.h $(SOURCES_DIR)\DayCounters\actualactual.h $(SOURCES_DIR)\DayCounters\thirty360.h $(SOURCES_DIR)\DayCounters\thirty360european.h $(SOURCES_DIR)\DayCounters\thirty360italian.h
	@touch $(SWIG_DIR)\DayCounters.i
$(SWIG_DIR)\Currencies.i: $(SWIG_DIR)\Date.i $(SWIG_DIR)\Calendars.i $(SOURCES_DIR)\currency.h $(SOURCES_DIR)\Currencies\eur.h $(SOURCES_DIR)\Currencies\usd.h $(SOURCES_DIR)\Currencies\gbp.h $(SOURCES_DIR)\Currencies\dem.h $(SOURCES_DIR)\Currencies\itl.h
	@touch $(SWIG_DIR)\Currencies.i
$(SWIG_DIR)\Financial.i: $(SOURCES_DIR)\yield.h $(SOURCES_DIR)\spread.h $(SOURCES_DIR)\discountfactor.h
	@touch $(SWIG_DIR)\Financial.i
$(SWIG_DIR)\Options.i: $(SOURCES_DIR)\options.h $(SOURCES_DIR)\stringconverters.h
	@touch $(SWIG_DIR)\Options.i
$(SWIG_DIR)\Instruments.i: $(SWIG_DIR)\Financial.i $(SWIG_DIR)\TermStructures.i $(SOURCES_DIR)\instrument.h $(SOURCES_DIR)\Instruments\stock.h
	@touch $(SWIG_DIR)\Instruments.i
$(SWIG_DIR)\Operators.i: $(SWIG_DIR)\Vectors.i $(SWIG_DIR)\BoundaryConditions.i $(SOURCES_DIR)\Operators\tridiagonaloperator.h
	@touch $(SWIG_DIR)\Operators.i
$(SWIG_DIR)\Pricers.i: $(SWIG_DIR)\Date.i $(SWIG_DIR)\Options.i $(SWIG_DIR)\Financial.i $(SOURCES_DIR)\Pricers\bsmeuropeanoption.h $(SOURCES_DIR)\Pricers\bsmamericanoption.h
	@touch $(SWIG_DIR)\Pricers.i
$(SWIG_DIR)\Solvers1D.i: $(SOURCES_DIR)\solver1d.h $(SOURCES_DIR)\Solvers1D\bisection.h $(SOURCES_DIR)\Solvers1D\brent.h $(SOURCES_DIR)\Solvers1D\falseposition.h $(SOURCES_DIR)\Solvers1D\newton.h $(SOURCES_DIR)\Solvers1D\newtonsafe.h $(SOURCES_DIR)\Solvers1D\ridder.h $(SOURCES_DIR)\Solvers1D\secant.h 
	@touch $(SWIG_DIR)\Solvers1D.i
$(SWIG_DIR)\TermStructures.i: $(SWIG_DIR)\Date.i $(SWIG_DIR)\Calendars.i $(SWIG_DIR)\DayCounters.i $(SWIG_DIR)\Financial.i $(SWIG_DIR)\Currencies.i $(SOURCES_DIR)\termstructure.h $(SOURCES_DIR)\handle.h $(SOURCES_DIR)\TermStructures\piecewiseconstantforwards.h 
	@touch $(SWIG_DIR)\TermStructures.i
$(SWIG_DIR)\Vectors.i: $(SOURCES_DIR)\array.h
	@touch $(SWIG_DIR)\Vectors.i
$(SWIG_DIR)\BoundaryConditions.i: $(SOURCES_DIR)\PDE\boundarycondition.h $(SOURCES_DIR)\stringconverters.h
	@touch $(SWIG_DIR)\BoundaryConditions.i

# QuantLib files
QuantLib: Core Calendars DayCounters Operators Pricers Solvers1D TermStructures

# Core
Core: $(CORE_OBJS)
$(OUTPUT_DIR)\calendar.obj: $(SOURCES_DIR)\calendar.cpp
$(OUTPUT_DIR)\date.obj: $(SOURCES_DIR)\date.cpp
$(OUTPUT_DIR)\normaldistribution.obj: $(SOURCES_DIR)\normaldistribution.cpp
$(OUTPUT_DIR)\solver1d.obj: $(SOURCES_DIR)\solver1d.cpp
$(OUTPUT_DIR)\statistics.obj: $(SOURCES_DIR)\statistics.cpp

# Calendars
Calendars: $(CALENDAR_OBJS)
$(OUTPUT_DIR)\westerncalendar.obj: $(SOURCES_DIR)\Calendars\westerncalendar.cpp
$(OUTPUT_DIR)\frankfurt.obj: $(SOURCES_DIR)\Calendars\frankfurt.cpp
$(OUTPUT_DIR)\london.obj: $(SOURCES_DIR)\Calendars\london.cpp
$(OUTPUT_DIR)\milan.obj: $(SOURCES_DIR)\Calendars\milan.cpp
$(OUTPUT_DIR)\newyork.obj: $(SOURCES_DIR)\Calendars\newyork.cpp
$(OUTPUT_DIR)\target.obj: $(SOURCES_DIR)\Calendars\target.cpp

# Day counters
DayCounters: $(DAYCOUNT_OBJS)
$(OUTPUT_DIR)\actualactual.obj: $(SOURCES_DIR)\DayCounters\actualactual.cpp
$(OUTPUT_DIR)\thirty360.obj: $(SOURCES_DIR)\DayCounters\thirty360.cpp
$(OUTPUT_DIR)\thirty360italian.obj: $(SOURCES_DIR)\DayCounters\thirty360italian.cpp

# Operators
Operators: $(OPERATOR_OBJS)
$(OUTPUT_DIR)\tridiagonaloperator.obj: $(SOURCES_DIR)\Operators\tridiagonaloperator.cpp
$(OUTPUT_DIR)\blackscholesmerton.obj: $(SOURCES_DIR)\Operators\blackscholesmerton.cpp

# Pricers
Pricers: $(PRICER_OBJS)
$(OUTPUT_DIR)\bsmnumericaloption.obj: $(SOURCES_DIR)\Pricers\bsmnumericaloption.cpp
$(OUTPUT_DIR)\bsmeuropeanoption.obj: $(SOURCES_DIR)\Pricers\bsmeuropeanoption.cpp
$(OUTPUT_DIR)\bsmamericanoption.obj: $(SOURCES_DIR)\Pricers\bsmamericanoption.cpp

# 1D solvers
Solvers1D: $(SOLVER1D_OBJS)
$(OUTPUT_DIR)\bisection.obj: $(SOURCES_DIR)\Solvers1D\bisection.cpp
$(OUTPUT_DIR)\brent.obj: $(SOURCES_DIR)\Solvers1D\brent.cpp
$(OUTPUT_DIR)\falseposition.obj: $(SOURCES_DIR)\Solvers1D\falseposition.cpp
$(OUTPUT_DIR)\newton.obj: $(SOURCES_DIR)\Solvers1D\newton.cpp
$(OUTPUT_DIR)\newtonsafe.obj: $(SOURCES_DIR)\Solvers1D\newtonsafe.cpp
$(OUTPUT_DIR)\ridder.obj: $(SOURCES_DIR)\Solvers1D\ridder.cpp
$(OUTPUT_DIR)\secant.obj: $(SOURCES_DIR)\Solvers1D\secant.cpp

# Term structures
TermStructures: $(TERMSTRUC_OBJS)
$(OUTPUT_DIR)\piecewiseconstantforwards.obj: $(SOURCES_DIR)\TermStructures\piecewiseconstantforwards.cpp

