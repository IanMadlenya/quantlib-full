/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2006 StatPro Italia srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/reference/license.html>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/


#ifndef quantlib_sobol_brownian_generator_hpp
#define quantlib_sobol_brownian_generator_hpp

#include <ql/MarketModels/browniangenerator.hpp>
#include <ql/RandomNumbers/inversecumulativersg.hpp>
#include <ql/RandomNumbers/sobolrsg.hpp>
#include <ql/MonteCarlo/brownianbridge.hpp>
#include <ql/Math/normaldistribution.hpp>
#include <vector>

namespace QuantLib {

    /*! Incremental Brownian generator using a Sobol generator,
        inverse-cumulative Gaussian method, and Brownian bridging.
    */
    class SobolBrownianGenerator : public BrownianGenerator {
      public:
        enum Ordering {
            Factors,  /*!< The variates with the best quality will be
                           used for the evolution of the first factor. */
            Steps,    /*!< The variates with the best quality will be
                           used for the largest steps of all factors. */
            Diagonal  /*!< A diagonal schema will be used to assign
                           the variates with the best quality to the
                           most important factors and the largest
                           steps. */
        };
        SobolBrownianGenerator(
                           Size factors,
                           Size steps,
                           Ordering ordering,
                           unsigned long seed = 0,
                           SobolRsg::DirectionIntegers directionIntegers
                                                         = SobolRsg::Jaeckel);

        Real nextPath();
        Real nextStep(Array&);

        Size numberOfFactors() const;
        Size numberOfSteps() const;
      private:
        Size factors_, steps_;
        Ordering ordering_;
        InverseCumulativeRsg<SobolRsg,InverseCumulativeNormal> generator_;
        BrownianBridge bridge_;
        // work variables
        Size lastStep_;
        std::vector<std::vector<Size> > orderedIndices_;
        std::vector<std::vector<Real> > bridgedVariates_;
    };

    class SobolBrownianGeneratorFactory : public BrownianGeneratorFactory {
      public:
        SobolBrownianGeneratorFactory(
                           SobolBrownianGenerator::Ordering ordering,
                           unsigned long seed = 0,
                           SobolRsg::DirectionIntegers directionIntegers
                                                         = SobolRsg::Jaeckel);
        boost::shared_ptr<BrownianGenerator> create(Size factors,
                                                    Size steps) const;
      private:
        SobolBrownianGenerator::Ordering ordering_;
        unsigned long seed_;
        SobolRsg::DirectionIntegers integers_;
    };

}


#endif
