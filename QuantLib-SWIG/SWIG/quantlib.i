
/*
 Copyright (C) 2000, 2001, 2002 RiskMap srl

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

// $Id$

#if defined(SWIGPYTHON) || defined(SWIGMZSCHEME) || defined(SWIGGUILE)
%module QuantLib
#elif defined(SWIGRUBY)
%module QuantLibc
#endif

%include exception.i

%exception {
    try {
        $action
    } catch (IndexError& e) {
        SWIG_exception(SWIG_IndexError,const_cast<char*>(e.what()));
    } catch (std::exception& e) {
        SWIG_exception(SWIG_RuntimeError,const_cast<char*>(e.what()));
    } catch (...) {
        SWIG_exception(SWIG_UnknownError,"unknown error");
    }
}

#if defined(SWIGPYTHON)

%{
#include <ql/quantlib.hpp>
const int    __hexversion__ = QL_HEX_VERSION;
const char* __version__    = QL_VERSION;
%}

const int __hexversion__;
%immutable;
const char* __version__;
%mutable;

#endif

#if defined(SWIGGUILE)
// code for loading shared library
%scheme%{
    (define (load-quantlibc-in path)
      (if (null? path)
          (error "QuantLibc.so not found")
          (let ((so-name (string-append (car path) "/QuantLibc.so")))
            (if (file-exists? so-name)
                (dynamic-call "SWIG_init" (dynamic-link so-name))
                (load-quantlibc-in (cdr path))))))
    (load-quantlibc-in %load-path)
%}
#endif



%include ql.i

