
/* 
Copyright (C) 2000 Ferdinando Ametrano, Luigi Ballabio, Adolfo Benin, Marco Marchioro
See the file LICENSE.TXT for information on usage and distribution
Contact ferdinando@ametrano.net if LICENSE.TXT was not distributed with this file
*/

/*! \file dataformatters.h
	\brief Classes used to format data for output
*/

#ifndef quantlib_data_formatters_h
#define quantlib_data_formatters_h

#include "qldefines.h"
#include "date.h"
#include "null.h"
#include <string>

namespace QuantLib {

	/*! \defgroup dataformatters Data Formatters 
		These classes are used to format various kind of data
		as strings for presentation.
	*/
	
	//! Formats integers for output
	/*! \ingroup dataformatters */
	class IntegerFormatter {
	  public:
		static std::string toString(int i, int digits = Null<int>());
	};

	//! Formats doubles for output
	/*! \ingroup dataformatters */
	class DoubleFormatter {
	  public:
		static std::string toString(double x, int precision = Null<int>(), int digits = Null<int>());
	};

	//! Formats amounts in Euro for output
	/*! Formatting follows Euro convention (x,xxx,xxx.xx)
		\ingroup dataformatters
	*/
	class EuroFormatter {
	  public:
		static std::string toString(double amount);
	};
	
	//! Formats rates for output
	/*! Formatting is in percentage form (xx.xxxxx%)
		\ingroup dataformatters
	*/
	class RateFormatter {
	  public:
		static std::string toString(double rate, int precision = 5);
	};
	
	//! Formats dates for output
	/*! \ingroup dataformatters
		Formatting can be in short (mm/dd/yyyy) or long (Month ddth, yyyy) form
	*/
	class DateFormatter {
	  public:
		static std::string toString(const Date& d, bool shortFormat = false);
	};

	//! Formats strings as lower- or uppercase
	/*! \ingroup dataformatters */
	class StringFormatter {
	  public:
		static std::string toLowercase(const std::string& s);
		static std::string toUppercase(const std::string& s);
	};
	
}


#endif
