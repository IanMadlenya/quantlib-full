

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email ferdinando@ametrano.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/
/*! \file cashflow.hpp
    \brief Base class for cash flows

    \fullpath
    ql/%cashflow.hpp
*/

// $Id$

#ifndef quantlib_cash_flow_hpp
#define quantlib_cash_flow_hpp

#include <ql/date.hpp>
#include <ql/Patterns/observable.hpp>

/*! \namespace QuantLib::CashFlows
    \brief Concrete implementations of the CashFlow interface

    See sect. \ref cashflows
*/

namespace QuantLib {

    //! Base class for cash flows
    /*! This class is purely virtual and acts as a base class for the actual
        cash flow implementations.
    */
    class CashFlow : public Patterns::Observable {
      public:
        virtual ~CashFlow() {}
        //! returns the amount of the cash flow
        /*! \note The amount is not discounted, i.e., it is the actual amount
            paid at the cash flow date.
        */
        virtual double amount() const = 0;
        //! returns the date at which the cash flow is settled
        virtual Date date() const = 0;
    };

}


#endif
