
/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email quantlib-dev@lists.sf.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#ifndef quantlib_volatilities_i
#define quantlib_volatilities_i

%include common.i
%include date.i
%include daycounters.i
%include types.i
%include currencies.i
%include observer.i
%include marketelements.i

%{
using QuantLib::BlackVolTermStructure;
using QuantLib::LocalVolTermStructure;
%}

%ignore BlackVolTermStructure;
class BlackVolTermStructure {
    #if defined(SWIGMZSCHEME) || defined(SWIGGUILE)
    %rename("reference-date") referenceDate;
    %rename("day-counter")    dayCounter;
    %rename("max-date")       maxDate;
    %rename("max-time")       maxTime;
    %rename("black-vol")      blackVol;
    %rename("black-variance") blackVariance;
    %rename("black-forward-vol")      blackVol;
    %rename("black-forward-variance") blackVariance;
    #endif
  public:
    Date referenceDate() const;
    DayCounter dayCounter() const;
    Date maxDate() const;
    Time maxTime() const;
    double blackVol(const Date&, double strike, 
                    bool extrapolate = false) const;
    double blackVol(Time, double strike, 
                    bool extrapolate = false) const;
    double blackVariance(const Date&, double strike, 
                         bool extrapolate = false) const;
    double blackVariance(Time, double strike, 
                         bool extrapolate = false) const;
    double blackForwardVol(const Date&, const Date&, 
                           double strike, bool extrapolate = false) const;
    double blackForwardVol(Time, Time, double strike, 
                           bool extrapolate = false) const;
    double blackForwardVariance(const Date&, const Date&, 
                                double strike, bool extrapolate = false) const;
    double blackForwardVariance(Time, Time, double strike, 
                                bool extrapolate = false) const;
};

%template(BlackVolTermStructure) Handle<BlackVolTermStructure>;
IsObservable(Handle<BlackVolTermStructure>);

%template(BlackVolTermStructureHandle) RelinkableHandle<BlackVolTermStructure>;
IsObservable(RelinkableHandle<BlackVolTermStructure>);


%ignore LocalVolTermStructure;
class LocalVolTermStructure {
    #if defined(SWIGMZSCHEME) || defined(SWIGGUILE)
    %rename("reference-date") referenceDate;
    %rename("day-counter")    dayCounter;
    %rename("max-date")       maxDate;
    %rename("max-time")       maxTime;
    %rename("local-vol")      localVol;
    #endif
  public:
    Date referenceDate() const;
    DayCounter dayCounter() const;
    Date maxDate() const;
    Time maxTime() const;
    double localVol(const Date&, double u,
                    bool extrapolate = false) const;
    double localVol(Time, double u,
                    bool extrapolate = false) const;
};

%template(LocalVolTermStructure) Handle<LocalVolTermStructure>;
IsObservable(Handle<LocalVolTermStructure>);

%template(LocalVolTermStructureHandle) RelinkableHandle<LocalVolTermStructure>;
IsObservable(RelinkableHandle<LocalVolTermStructure>);


// actual term structures below

// constant Black vol term structure
%{
using QuantLib::BlackConstantVol;
typedef Handle<BlackVolTermStructure> BlackConstantVolHandle;
%}

%rename(BlackConstantVol) BlackConstantVolHandle;
class BlackConstantVolHandle : public Handle<BlackVolTermStructure> {
  public:
    %extend {
        BlackConstantVolHandle(
                const Date& referenceDate, double volatility,
                const DayCounter& dayCounter = Actual365()) {
            return new BlackConstantVolHandle(
                new BlackConstantVol(referenceDate, volatility, dayCounter));
        }
        BlackConstantVolHandle(
                const Date& referenceDate,
                const RelinkableHandle<Quote>& volatility,
                const DayCounter& dayCounter = Actual365()) {
            return new BlackConstantVolHandle(
                new BlackConstantVol(referenceDate, volatility, dayCounter));
        }
    }
};

// Black smiled surface
%{
using QuantLib::BlackVarianceSurface;
typedef BlackVarianceSurface::Extrapolation VolExtrapolationType;
typedef Handle<BlackVolTermStructure> BlackVarianceSurfaceHandle;

VolExtrapolationType volExTypeFromString(std::string s) {
    s = StringFormatter::toLowercase(s);
    if (s == "const" || s == "constant")
        return BlackVarianceSurface::ConstantExtrapolation;
    else if (s == "" || s == "default")
        return BlackVarianceSurface::InterpolatorDefaultExtrapolation;
    else
        throw Error("unknown extrapolation type: "+s);
}

std::string volExTypeToString(VolExtrapolationType t) {
    switch (t) {
      case BlackVarianceSurface::ConstantExtrapolation:
        return "constant";
      case BlackVarianceSurface::InterpolatorDefaultExtrapolation:
        return "default";
      default:
        throw Error("unknown extrapolation type");
    }
}
%}

MapToString(VolExtrapolationType,volExTypeFromString,volExTypeToString);

%rename(BlackVarianceSurface) BlackVarianceSurfaceHandle;
class BlackVarianceSurfaceHandle : public Handle<BlackVolTermStructure> {
  public:
    %extend {
        BlackVarianceSurfaceHandle(
                const Date& referenceDate, 
                const std::vector<Date>& dates,
                const std::vector<double>& strikes,
                const Matrix& blackVols,
                VolExtrapolationType lower = 
                    BlackVarianceSurface::InterpolatorDefaultExtrapolation,
                VolExtrapolationType upper = 
                    BlackVarianceSurface::InterpolatorDefaultExtrapolation,
                const DayCounter& dayCounter = Actual365()) {
            return new BlackVarianceSurfaceHandle(
                new BlackVarianceSurface(referenceDate,dates,strikes,
                                         blackVols,lower,upper,dayCounter));
        }
    }
};



// constant local vol term structure
%{
using QuantLib::LocalConstantVol;
typedef Handle<LocalVolTermStructure> LocalConstantVolHandle;
%}

%rename(LocalConstantVol) LocalConstantVolHandle;
class LocalConstantVolHandle : public Handle<LocalVolTermStructure> {
  public:
    %extend {
        LocalConstantVolHandle(
                const Date& referenceDate, double volatility,
                const DayCounter& dayCounter = Actual365()) {
            return new LocalConstantVolHandle(
                new LocalConstantVol(referenceDate, volatility, dayCounter));
        }
        LocalConstantVolHandle(
                const Date& referenceDate,
                const RelinkableHandle<Quote>& volatility,
                const DayCounter& dayCounter = Actual365()) {
            return new LocalConstantVolHandle(
                new LocalConstantVol(referenceDate, volatility, dayCounter));
        }
    }
};




#endif
