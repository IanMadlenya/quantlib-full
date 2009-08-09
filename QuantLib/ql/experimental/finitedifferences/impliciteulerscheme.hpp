/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2009 Andreas Gaida
 Copyright (C) 2009 Ralph Schreyer
 Copyright (C) 2009 Klaus Spanderen

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file impliciteulerscheme.hpp
    \brief Implicit-Euler scheme
*/

#ifndef quantlib_implicit_euler_scheme_hpp
#define quantlib_implicit_euler_scheme_hpp

#include <ql/methods/finitedifferences/operatortraits.hpp>
#include <ql/experimental/finitedifferences/fdmdirichletboundary.hpp>
#include <ql/experimental/finitedifferences/fdmlinearopcomposite.hpp>

namespace QuantLib {

    class ImplicitEulerScheme  {
      public:
        // typedefs
        typedef OperatorTraits<FdmLinearOp> traits;
        typedef traits::operator_type operator_type;
        typedef traits::array_type array_type;
        typedef traits::bc_set bc_set;
        typedef traits::condition_type condition_type;

        // constructors
        ImplicitEulerScheme(
            const boost::shared_ptr<FdmLinearOpComposite>& map,
            const std::vector<boost::shared_ptr<FdmDirichletBoundary> >& bc_set
                = std::vector<boost::shared_ptr<FdmDirichletBoundary> >(),
            Real relTol = 1e-6);

        void step(array_type& a, Time t);
        void setStep(Time dt);

      protected:
        Disposable<Array> apply(const Array& r) const;   
          
        Time dt_;
        const Real relTol_;
        const boost::shared_ptr<FdmLinearOpComposite> & map_;
        const std::vector<boost::shared_ptr<FdmDirichletBoundary> > bcSet_;
    };
}

#endif
