
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


#ifndef quantlib_functions_i
#define quantlib_functions_i

%include linearalgebra.i
%include types.i

%{
using QuantLib::ObjectiveFunction;
using QuantLib::Optimization::CostFunction;
%}

#if defined(SWIGPYTHON)

%{
class UnaryFunction {
  public:
	UnaryFunction(PyObject* function) : function_(function) {
	    Py_XINCREF(function_);
    }
    UnaryFunction(const UnaryFunction& f) : function_(f.function_) {
	    Py_XINCREF(function_);
    }
    UnaryFunction& operator=(const UnaryFunction& f) {
        if ((this != &f) && (function_ != f.function_)) {
            Py_XDECREF(function_);
            function_ = f.function_;
    	    Py_XINCREF(function_);
        }
        return *this;
    }
    ~UnaryFunction() {
        Py_XDECREF(function_);
    }
	double operator()(double x) const {
		PyObject* pyResult = PyObject_CallFunction(function_,"d",x);
		QL_ENSURE(pyResult != NULL, "failed to call Python function");
		double result = PyFloat_AsDouble(pyResult);
		Py_XDECREF(pyResult);
		return result;
	}
  private:
	PyObject* function_;
};

class BinaryFunction {
  public:
	BinaryFunction(PyObject* function) : function_(function) {
	    Py_XINCREF(function_);
    }
    BinaryFunction(const BinaryFunction& f)
    : function_(f.function_) {
	    Py_XINCREF(function_);
    }
    BinaryFunction& operator=(const BinaryFunction& f) {
        if ((this != &f) && (function_ != f.function_)) {
            Py_XDECREF(function_);
            function_ = f.function_;
    	    Py_XINCREF(function_);
        }
        return *this;
    }
    ~BinaryFunction() {
        Py_XDECREF(function_);
    }
	double operator()(double x, double y) const {
		PyObject* pyResult = PyObject_CallFunction(function_,"dd",x,y);
		QL_ENSURE(pyResult != NULL, "failed to call Python function");
		double result = PyFloat_AsDouble(pyResult);
		Py_XDECREF(pyResult);
		return result;
	}
  private:
	PyObject* function_;
};

class PyObjectiveFunction : public ObjectiveFunction {
  public:
	PyObjectiveFunction(PyObject* function) : function_(function) {
	    Py_XINCREF(function_);
    }
    PyObjectiveFunction(const PyObjectiveFunction& f)
    : function_(f.function_) {
	    Py_XINCREF(function_);
    }
    PyObjectiveFunction& operator=(const PyObjectiveFunction& f) {
        if ((this != &f) && (function_ != f.function_)) {
            Py_XDECREF(function_);
            function_ = f.function_;
    	    Py_XINCREF(function_);
        }
        return *this;
    }
    ~PyObjectiveFunction() {
        Py_XDECREF(function_);
    }
	double operator()(double x) const {
		PyObject* pyResult = PyObject_CallFunction(function_,"d",x);
		QL_ENSURE(pyResult != NULL, "failed to call Python function");
		double result = PyFloat_AsDouble(pyResult);
		Py_XDECREF(pyResult);
		return result;
	}
	double derivative(double x) const {
		PyObject* pyResult =
		  PyObject_CallMethod(function_,"derivative","d",x);
		QL_ENSURE(pyResult != NULL,
		  "failed to call derivative() on Python object");
		double result = PyFloat_AsDouble(pyResult);
		Py_XDECREF(pyResult);
		return result;
	}
  private:
	PyObject* function_;
};

class PyCostFunction : public CostFunction {
  public:
	PyCostFunction(PyObject* function) : function_(function) {
	    Py_XINCREF(function_);
    }
    PyCostFunction(const PyCostFunction& f)
    : function_(f.function_) {
	    Py_XINCREF(function_);
    }
    PyCostFunction& operator=(const PyCostFunction& f) {
        if ((this != &f) && (function_ != f.function_)) {
            Py_XDECREF(function_);
            function_ = f.function_;
    	    Py_XINCREF(function_);
        }
        return *this;
    }
    ~PyCostFunction() {
        Py_XDECREF(function_);
    }
	double value(const Array& x) const {
        PyObject* tuple = PyTuple_New(x.size());
        for (Size i=0; i<x.size(); i++)
            PyTuple_SetItem(tuple,i,PyFloat_FromDouble(x[i]));
		PyObject* pyResult = PyObject_CallObject(function_,tuple);
        Py_XDECREF(tuple);
		QL_ENSURE(pyResult != NULL, "failed to call Python function");
		double result = PyFloat_AsDouble(pyResult);
		Py_XDECREF(pyResult);
		return result;
	}
  private:
	PyObject* function_;
};
%}

#elif defined(SWIGRUBY)

%{
class UnaryFunction {
  public:
    double operator()(double x) const {
        return NUM2DBL(rb_yield(rb_float_new(x)));
    }
};

class RubyObjectiveFunction : public ObjectiveFunction {
  public:
    double operator()(double x) const {
        return NUM2DBL(rb_yield(rb_float_new(x)));
    }
};

class RubyCostFunction : public CostFunction {
  public:
    double value(const Array& x) const {
        VALUE a = rb_ary_new2(x.size());
        for (Size i=0; i<x.size(); i++)
            rb_ary_store(a,i,rb_float_new(x[i]));
        return NUM2DBL(rb_yield(a));
    }
};
%}

#elif defined(SWIGMZSCHEME)

%{
class UnaryFunction {
  public:
	UnaryFunction(Scheme_Object* function) : function_(function) {
        if (!SCHEME_PROCP(function))
            throw Error("procedure expected");
	    scheme_dont_gc_ptr(function_);
    }
    UnaryFunction(const UnaryFunction& f) : function_(f.function_) {
	    scheme_dont_gc_ptr(function_);
    }
    UnaryFunction& operator=(const UnaryFunction& f) {
        if ((this != &f) && (function_ != f.function_)) {
            scheme_gc_ptr_ok(function_);
            function_ = f.function_;
    	    scheme_dont_gc_ptr(function_);
        }
        return *this;
    }
    ~UnaryFunction() {
        scheme_gc_ptr_ok(function_);
    }
	double operator()(double x) const {
		Scheme_Object* arg = scheme_make_double(x);
        Scheme_Object* mzResult = scheme_apply(function_,1,&arg);
        QL_ENSURE(SCHEME_REALP(mzResult),
                  "the function did not return a double");
		double result = scheme_real_to_double(mzResult);
		return result;
	}
  private:
	Scheme_Object* function_;
};

class BinaryFunction {
  public:
	BinaryFunction(Scheme_Object* function) : function_(function) {
        if (!SCHEME_PROCP(function))
            throw Error("procedure expected");
	    scheme_dont_gc_ptr(function_);
    }
    BinaryFunction(const BinaryFunction& f) : function_(f.function_) {
	    scheme_dont_gc_ptr(function_);
    }
    BinaryFunction& operator=(const BinaryFunction& f) {
        if ((this != &f) && (function_ != f.function_)) {
            scheme_gc_ptr_ok(function_);
            function_ = f.function_;
    	    scheme_dont_gc_ptr(function_);
        }
        return *this;
    }
    ~BinaryFunction() {
        scheme_gc_ptr_ok(function_);
    }
	double operator()(double x, double y) const {
		Scheme_Object* arg1 = scheme_make_double(x);
        Scheme_Object* arg2 = scheme_make_double(y);
        Scheme_Object* arg  = scheme_make_pair(arg1,
                                               scheme_make_pair(arg2,
                                                                scheme_null));
        Scheme_Object* mzResult = scheme_apply_to_list(function_,arg);
        QL_ENSURE(SCHEME_REALP(mzResult),
                  "the function did not return a double");
		double result = scheme_real_to_double(mzResult);
		return result;
	}
  private:
	Scheme_Object* function_;
};

class MzObjectiveFunction : public ObjectiveFunction {
  public:
	MzObjectiveFunction(Scheme_Object* function) : function_(function) {
        if (!SCHEME_PROCP(function))
            throw Error("procedure expected");
	    scheme_dont_gc_ptr(function_);
    }
    MzObjectiveFunction(const MzObjectiveFunction& f) 
    : function_(f.function_) {
	    scheme_dont_gc_ptr(function_);
    }
    MzObjectiveFunction& operator=(const MzObjectiveFunction& f) {
        if ((this != &f) && (function_ != f.function_)) {
            scheme_gc_ptr_ok(function_);
            function_ = f.function_;
    	    scheme_dont_gc_ptr(function_);
        }
        return *this;
    }
    ~MzObjectiveFunction() {
        scheme_gc_ptr_ok(function_);
    }
	double operator()(double x) const {
		Scheme_Object* arg = scheme_make_double(x);
        Scheme_Object* mzResult = scheme_apply(function_,1,&arg);
        QL_ENSURE(SCHEME_REALP(mzResult),
                  "the function did not return a double");
		double result = scheme_real_to_double(mzResult);
		return result;
	}
  private:
	Scheme_Object* function_;
};

class MzCostFunction : public CostFunction {
  public:
	MzCostFunction(Scheme_Object* function) : function_(function) {
        if (!SCHEME_PROCP(function))
            throw Error("procedure expected");
	    scheme_dont_gc_ptr(function_);
    }
    MzCostFunction(const MzCostFunction& f) 
    : function_(f.function_) {
	    scheme_dont_gc_ptr(function_);
    }
    MzCostFunction& operator=(const MzCostFunction& f) {
        if ((this != &f) && (function_ != f.function_)) {
            scheme_gc_ptr_ok(function_);
            function_ = f.function_;
    	    scheme_dont_gc_ptr(function_);
        }
        return *this;
    }
    ~MzCostFunction() {
        scheme_gc_ptr_ok(function_);
    }
	double value(const Array& x) const {
        Scheme_Object** args = new Scheme_Object*[x.size()];
        for (Size i=0; i<x.size(); i++)
            args[i] = scheme_make_double(x[i]);
        Scheme_Object* mzResult = scheme_apply(function_,x.size(),args);
        delete[] args;
        QL_ENSURE(SCHEME_REALP(mzResult),
                  "the function did not return a double");
		double result = scheme_real_to_double(mzResult);
		return result;
	}
  private:
	Scheme_Object* function_;
};
%}

#elif defined(SWIGGUILE)

%{
class UnaryFunction {
  public:
	UnaryFunction(SCM function) : function_(function) {
        if (!gh_procedure_p(function))
            throw Error("procedure expected");
	    scm_protect_object(function_);
    }
    ~UnaryFunction() {
        scm_unprotect_object(function_);
    }
	double operator()(double x) const {
		SCM arg = gh_double2scm(x);
        SCM guileResult = gh_call1(function_,arg);
		double result = gh_scm2double(guileResult);
		return result;
	}
  private:
	SCM function_;
    // inhibit copy
    UnaryFunction(const UnaryFunction& f) {}
    UnaryFunction& operator=(const UnaryFunction& f) {
        return *this;
    }
};

class GuileObjectiveFunction : public ObjectiveFunction {
  public:
	GuileObjectiveFunction(SCM function) : function_(function) {
        if (!gh_procedure_p(function))
            throw Error("procedure expected");
	    scm_protect_object(function_);
    }
    ~GuileObjectiveFunction() {
        scm_unprotect_object(function_);
    }
	double operator()(double x) const {
		SCM arg = gh_double2scm(x);
        SCM guileResult = gh_call1(function_,arg);
		double result = gh_scm2double(guileResult);
		return result;
	}
  private:
	SCM function_;
    // inhibit copy
    GuileObjectiveFunction(const GuileObjectiveFunction& f) {}
    GuileObjectiveFunction& operator=(const GuileObjectiveFunction& f) {
        return *this;
    }
};

class GuileCostFunction : public CostFunction {
  public:
	GuileCostFunction(SCM function) : function_(function) {
        if (!gh_procedure_p(function))
            throw Error("procedure expected");
	    scm_protect_object(function_);
    }
    ~GuileCostFunction() {
        scm_unprotect_object(function_);
    }
	double value(const Array& x) const {
        SCM v = gh_make_vector(gh_long2scm(x.size()),SCM_UNSPECIFIED);
        for (Size i=0; i<x.size(); i++)
            gh_vector_set_x(v,gh_long2scm(i),gh_double2scm(x[i]));
        SCM guileResult = gh_apply(function_,gh_vector_to_list(v));
		double result = gh_scm2double(guileResult);
		return result;
	}
  private:
	SCM function_;
    // inhibit copy
    GuileCostFunction(const GuileCostFunction& f) {}
    GuileCostFunction& operator=(const GuileCostFunction& f) {
        return *this;
    }
};
%}


#endif





#endif
