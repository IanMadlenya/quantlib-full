
/*
 Copyright (C) 2001, 2002, 2003 Sadruddin Rejeb
 Copyright (C) 2004 StatPro Italia srl

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

/*! \file discretizedswaption.hpp
    \brief Discretized swaption class
*/

#ifndef quantlib_pricers_swaption_pricer_h
#define quantlib_pricers_swaption_pricer_h

#include <ql/Instruments/swaption.hpp>
#include <ql/discretizedasset.hpp>

namespace QuantLib {

    class DiscretizedSwap : public DiscretizedAsset {
      public:
        #ifndef QL_DISABLE_DEPRECATED
        /*! \deprecated use the constructor with a single argument */
        DiscretizedSwap(const boost::shared_ptr<NumericalMethod>& method,
                        const SimpleSwap::arguments& args)
        : DiscretizedAsset(method), arguments_(args) {}
        #endif

        DiscretizedSwap(const SimpleSwap::arguments& args)
        : arguments_(args) {}

        void reset(Size size) {
            values_ = Array(size, 0.0);
            adjustValues();
        }

        void addTimesTo(std::list<Time>& times) const {
            Time t;
            Size i;
            for (i=0; i<arguments_.fixedPayTimes.size(); i++) {
                t = arguments_.fixedPayTimes[i];
                if (t >= 0.0)
                    times.push_back(t);
            }
            for (i=0; i<arguments_.floatingResetTimes.size(); i++) {
                t = arguments_.floatingResetTimes[i];
                if (t >= 0.0)
                    times.push_back(t);
            }
            for (i=0; i<arguments_.floatingPayTimes.size(); i++) {
                t = arguments_.floatingPayTimes[i];
                if (t >= 0.0)
                    times.push_back(t);
            }
        }
      protected:
        void preAdjustValuesImpl();
        void postAdjustValuesImpl();
      private:
        SimpleSwap::arguments arguments_;
    };

    class DiscretizedSwaption : public DiscretizedOption {
      public:
        #ifndef QL_DISABLE_DEPRECATED
        /*! \deprecated use the constructor with a single argument */
        DiscretizedSwaption(const boost::shared_ptr<DiscretizedSwap>&,
                            const Swaption::arguments& args)
        : DiscretizedOption(boost::shared_ptr<DiscretizedAsset>(
                                                   new DiscretizedSwap(args)),
                            args.exercise->type(),
                            args.stoppingTimes),
          arguments_(args) {}
        #endif
        DiscretizedSwaption(const Swaption::arguments& args)
        : DiscretizedOption(boost::shared_ptr<DiscretizedAsset>(
                                                   new DiscretizedSwap(args)),
                            args.exercise->type(),
                            args.stoppingTimes),
          arguments_(args) {}
        void reset(Size size);
      private:
        Swaption::arguments arguments_;
    };

}


#endif

