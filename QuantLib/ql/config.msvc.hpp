
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

// $Id$

#ifndef quantlib_config_msvc_h
#define quantlib_config_msvc_h

// disable useless warning
// 'identifier' : decorated name length exceeded,
//                name was truncated in debug info
#pragma warning(disable: 4786)

#if _MSC_VER == 1200
// move inside here configs specific to VC++ 6.0
#define GARBLED_REVERSE_ITERATOR


#elif _MSC_VER == 1300
// move inside here configs specific to VC++ .Net
#define QL_ITERATOR         std::iterator
#define QL_ITERATOR_TRAITS  std::iterator_traits
#define SPECIALIZE_MS_ITERATOR_TRAITS  // always new ways to surprise us...


#endif


// STLPort or VC++ implementation?
#if defined(__SGI_STL)
    #define QL_MIN      _STL::min
    #define QL_MAX      _STL::max
#else
    #define QL_MIN      std::_cpp_min
    #define QL_MAX      std::_cpp_max
#endif


// leave outside here common configs


// force undefining min and max macros
#ifndef NOMINMAX
    #error Add NOMINMAX to preprocessor definitions
#endif

#define REQUIRES_DUMMY_RETURN

#define HAVE_CSTDLIB
#define QL_SIZE_T size_t
#define QL_ATOI   atoi

#define HAVE_CMATH
#define QL_SQRT   sqrt
#define QL_FABS   fabs
#define QL_EXP    exp
#define QL_LOG    log
#define QL_SIN    sin
#define QL_COS    cos
#define QL_POW    pow
#define QL_MODF   modf
#define QL_FLOOR  floor

#define HAVE_LIMITS

#define HAVE_CTIME
#define QL_TIME_T   time_t
#define QL_TM       tm
#define QL_CLOCK    clock
#define QL_TIME     time
#define QL_GMTIME   gmtime

#define HAVE_CCTYPE
#define QL_TOLOWER  tolower
#define QL_TOUPPER  toupper

#define HAVE_CSTRING
#define QL_STRLEN   strlen

#define HAVE_CSTDIO
#define QL_SPRINTF  sprintf

#define BROKEN_TEMPLATE_SPECIALIZATION
#define BROKEN_TEMPLATE_METHOD_CALLS
#define CHOKES_ON_TYPENAME
#define HAVE_EXPRESSION_TEMPLATES
#define HAVE_TEMPLATE_METAPROGRAMMING

#define HAVE_INCOMPLETE_ITERATOR_SUPPORT

#define QL_PATCH_MICROSOFT_BUGS

/*** libraries to be linked***/

#ifdef _DEBUG
    #pragma comment(lib,"QuantLib_d.lib")
#else
    #pragma comment(lib,"QuantLib.lib")
#endif

#if !defined (_MT) || !defined(_DLL)
    #ifdef _DEBUG
        #error Set 'Debug Multithreaded DLL' under C/C++ | Code Generation
    #else
        #error Set 'Multithreaded DLL' under C/C++ | Code Generation
    #endif
#endif

#ifndef _CPPRTTI
    #error Check the 'C/C++ | C++ Language | Use RTTI' option
#endif

#endif
