
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

#ifndef quantlib_discount_curve_i
#define quantlib_discount_curve_i

%include date.i
%include termstructures.i

%{
using QuantLib::DiscountCurve;
using QuantLib::ExtendedDiscountCurve;
typedef Handle<TermStructure> DiscountCurveHandle;
typedef Handle<TermStructure> ExtendedDiscountCurveHandle;
%}

%rename(DiscountCurve) DiscountCurveHandle;
class DiscountCurveHandle : public Handle<TermStructure> {
  public:
    %extend {
      DiscountCurveHandle(const Date& todaysDate, 
                          const std::vector<Date>& dates,
                          const std::vector<double>& discounts,
                          const DayCounter& dayCounter = Actual365()) {
          return new DiscountCurveHandle(
              new DiscountCurve(todaysDate, dates, discounts, dayCounter));
      }
      const std::vector<Date>& dates() {
          return Handle<DiscountCurve>(*self)->dates();
      }
    }
};


%rename(ExtendedDiscountCurve) ExtendedDiscountCurveHandle;
class ExtendedDiscountCurveHandle : public DiscountCurveHandle {
  public:
    %extend {
      ExtendedDiscountCurveHandle(const Date& todaysDate, 
                                  const std::vector<Date>& dates,
                                  const std::vector<double>& discounts,
                                  const Calendar& calendar,
                                  RollingConvention roll,
                                  const DayCounter& dayCounter = Actual365()) {
          return new ExtendedDiscountCurveHandle(
              new ExtendedDiscountCurve(todaysDate, dates, discounts, 
                                        calendar, roll, dayCounter));
      }
    }
};


#endif
