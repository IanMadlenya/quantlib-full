
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

#ifndef quantlib_montecarlo_tools_i
#define quantlib_montecarlo_tools_i

%include linearalgebra.i
%include randomnumbers.i
%include types.i

%{
using QuantLib::MonteCarlo::getCovariance;
%}
%rename(covariance) getCovariance;
Matrix getCovariance(const Array& volatilities, const Matrix& correlations);


%{
using QuantLib::MonteCarlo::Path;
%}

#if defined(SWIGRUBY)
%mixin Path "Enumerable";
#endif
class Path {
    #if defined(SWIGPYTHON) || defined(SWIGRUBY)
    %rename(__len__) size;
    #elif defined(SWIGMZSCHEME) || defined(SWIGGUILE)
    %rename(length)  size;
    #endif
  private:
    Path();
  public:
    Size size() const;
    const Array& drift() const;
    const Array& diffusion() const;
    %extend {
        #if defined(SWIGPYTHON) || defined(SWIGRUBY)
        double __getitem__(int i) {
            int size_ = int(self->size());
            if (i>=0 && i<size_) {
                return (*self)[i];
            } else if (i<0 && -i<=size_) {
                return (*self)[size_+i];
            } else {
                throw IndexError("Path index out of range");
            }
            QL_DUMMY_RETURN(0.0)
        }
        #elif defined(SWIGMZSCHEME) || defined(SWIGGUILE)
        double ref(int i) {
            int size_ = int(self->size());
            if (i>=0 && i<size_) {
                return (*self)[i];
            } else {
                throw IndexError("Path index out of range");
            }
            QL_DUMMY_RETURN(0.0)
        }
        #endif
        #if defined(SWIGRUBY)
        void each() {
            for (Size i=0; i<self->size(); i++)
                rb_yield(rb_float_new((*self)[i]));
        }
        #elif defined(SWIGMZSCHEME)
        void for_each(Scheme_Object* proc) {
            for (Size i=0; i<self->size(); ++i) {
                Scheme_Object* x = scheme_make_double((*self)[i]);
                scheme_apply(proc,1,&x);
            }
        }
        #elif defined(SWIGGUILE)
        void for_each(SCM proc) {
            for (Size i=0; i<self->size(); ++i) {
                SCM x = gh_double2scm((*self)[i]);
                gh_call1(proc,x);
            }
        }
        #endif
    }
};

%{
using QuantLib::MonteCarlo::GaussianPathGenerator_old;
%}
%template(SamplePath) Sample<Path>;
%rename(GaussianPathGenerator) GaussianPathGenerator_old;
class GaussianPathGenerator_old {
  public:
    GaussianPathGenerator_old(double drift, double variance,
                              Time length, Size steps, long seed = 0);
	Sample<Path> next() const;
};


%{
using QuantLib::MonteCarlo::MultiPath;
%}

class MultiPath {
    #if defined(SWIGPYTHON) || defined(SWIGRUBY)
    %rename(__len__)        pathSize;
    #elif defined(SWIGMZSCHEME) || defined(SWIGGUILE)
    %rename("length")       pathSize;
    %rename("asset-number") assetNumber;
    #endif
  private:
    MultiPath();
  public:
    Size pathSize() const;
    Size assetNumber() const;
    %extend {
        #if defined(SWIGPYTHON) || defined(SWIGRUBY)
        const Path& __getitem__(int i) {
            int assets_ = int(self->assetNumber());
            if (i>=0 && i<assets_) {
                return (*self)[i];
            } else if (i<0 && -i<=assets_) {
                return (*self)[assets_+i];
            } else {
                throw IndexError("MultiPath index out of range");
            }
            QL_DUMMY_RETURN((*self)[0])
        }
        #elif defined(SWIGMZSCHEME) || defined(SWIGGUILE)
        double ref(int i, int j) {
            int assets_ = int(self->assetNumber());
            int size_ = int(self->pathSize());
            if (i>=0 && i<assets_ && j>=0 && j<size_) {
                return (*self)[i][j];
            } else {
                throw IndexError("MultiPath index out of range");
            }
            QL_DUMMY_RETURN(0.0)
        }
        #endif
        #if defined(SWIGRUBY)
        void each_path() {
            swig_type_info* type = SWIG_TypeQuery("Path *");
            for (Size i=0; i<self->assetNumber(); i++)
                rb_yield(SWIG_NewPointerObj((void *) &((*self)[i]), type, 0));
        }
        void each_step() {
            for (Size j=0; j<self->pathSize(); j++) {
                VALUE v = rb_ary_new2(self->assetNumber());
                for (Size i=0; i<self->assetNumber(); i++)
                    rb_ary_store(v,i,rb_float_new((*self)[i][j]));
                rb_yield(v);
            }
        }
        #elif defined(SWIGMZSCHEME)
        void for_each_path(Scheme_Object* proc) {
            for (Size i=0; i<self->assetNumber(); i++) {
                Scheme_Object* x = SWIG_MakePtr(&((*self)[i]), 
                                                $descriptor(Path *));
                scheme_apply(proc,1,&x);
            }
        }
        void for_each_step(Scheme_Object* proc) {
            for (Size j=0; j<self->pathSize(); j++) {
                Scheme_Object* v = scheme_make_vector(self->assetNumber(),
                                                      scheme_undefined);
                Scheme_Object** els = SCHEME_VEC_ELS(v);
                for (Size i=0; i<self->assetNumber(); i++)
                    els[i] = scheme_make_double((*self)[i][j]);
                scheme_apply(proc,1,&v);
            }
        }
        #elif defined(SWIGGUILE)
        void for_each_path(SCM proc) {
            for (Size i=0; i<self->assetNumber(); ++i) {
                SCM x = SWIG_Guile_MakePtr(&((*self)[i]), $descriptor(Path *));
                gh_call1(proc,x);
            }
        }
        void for_each_step(SCM proc) {
            for (Size j=0; j<self->pathSize(); ++j) {
                SCM v = gh_make_vector(gh_long2scm(self->assetNumber()),
                                       SCM_UNSPECIFIED);
                for (Size i=0; i<self->assetNumber(); i++) {
                    gh_vector_set_x(v,gh_long2scm(i),
                                    gh_double2scm((*self)[i][j]));
                }
                gh_call1(proc,v);
            }
        }
        #endif
    }
};

%{
using QuantLib::MonteCarlo::GaussianMultiPathGenerator;
%}
%template(SampleMultiPath) Sample<MultiPath>;
class GaussianMultiPathGenerator {
  public:
    %extend {
      GaussianMultiPathGenerator(const Array& drifts,
                                 const Matrix& covariance,
                                 const std::vector<double>& timeDelays,
                                 long seed=0) {
          return new GaussianMultiPathGenerator(drifts,
                                                covariance,
                                                QL::TimeGrid(
                                                    timeDelays.begin(),
                                                    timeDelays.end()),
                                                seed);
      }
    }
	Sample<MultiPath> next() const;
};


#endif
