
/* 
Copyright (C) 2000 Ferdinando Ametrano, Luigi Ballabio, Adolfo Benin, Marco Marchioro
See the file LICENSE.TXT for information on usage and distribution
Contact ferdinando@ametrano.net if LICENSE.TXT was not distributed with this file
*/

#include "bsmnumericaloption.h"
#include "qlerrors.h"

QL_BEGIN_NAMESPACE(QuantLib)

QL_BEGIN_NAMESPACE(Pricers)

QL_USING(PDE,BoundaryCondition)
QL_USING(Operators,BSMOperator)

double BSMNumericalOption::dVolMultiplier=0.0001; 
double BSMNumericalOption::dRMultiplier=0.0001; 

BSMNumericalOption::BSMNumericalOption(BSMOption::Type type, double underlying, double strike, 
  Rate underlyingGrowthRate, Rate riskFreeRate, Time residualTime, double volatility, int gridPoints)
: BSMOption(type,underlying,strike,underlyingGrowthRate,riskFreeRate,residualTime,volatility), 
  theGridPoints(gridPoints),rhoComputed(false), vegaComputed(false) {
	// common setup
	setGridLimits();
	initializeGrid();
	initializeInitialCondition();
	initializeOperator();
	hasBeenCalculated = false;
}

double BSMNumericalOption::delta() const {
	if(!hasBeenCalculated)  
		value();
	return theDelta;
}

double BSMNumericalOption::gamma() const {
	if(!hasBeenCalculated) 
		value();
	return theGamma;
}

double BSMNumericalOption::theta() const {
	if(!hasBeenCalculated) 
		value();
	return theTheta;
}
double BSMNumericalOption::vega() const {
	return 0.0;
}

double BSMNumericalOption::rho() const {
	return 0.0;
}

/*
double BSMNumericalOption::vega() const {

	if(!vegaComputed){
		if(!hasBeenCalculated) 
			value();
		Handle<BSMOption> brandNewFD = clone();
		double volMinus=theVolatility*(1.0-dVolMultiplier);
		brandNewFD->setVolatility(volMinus);		
		theVega=(value() - brandNewFD->value())/(theVolatility*dVolMultiplier);		  
		vegaComputed = true;
	}
	return theVega;
}

double BSMNumericalOption::rho() const {

	if(!rhoComputed){
		if(!hasBeenCalculated) 
			value();
		Handle<BSMOption> brandNewFD = clone();
		Rate rMinus=theExerciseRate*(1.0-dRMultiplier);		
		brandNewFD->setExerciseRate(rMinus);
		theRho=(value() - brandNewFD->value())/(theExerciseRate*dRMultiplier);
  		rhoComputed  = true;
	}
	return theRho;
}

void BSMNumericalOption::setVolatility(double volatility) {
	BSMOption::setVolatility(volatility);
	hasBeenCalculated=false;
}

void BSMNumericalOption::setExerciseRate(Rate newExerciseRate) {
	BSMOption::setExerciseRate(newExerciseRate);
	hasBeenCalculated=false;
}
*/



void BSMNumericalOption::setGridLimits() {
	double prefactor = 1.0+0.05/theVolatility;		// correction for small volatilities
	double minMaxFactor = QL_EXP(4.0*prefactor*theVolatility*QL_SQRT(theResidualTime));

	sMin = theUnderlying/minMaxFactor;				// underlying grid min value
	sMax = theUnderlying*minMaxFactor;				// underlying grid max value

	double safetyZoneFactor = 1.1;					// to insure strike is included in the grid
	if(sMin > theStrike/safetyZoneFactor){
		sMin = theStrike/safetyZoneFactor;  
		sMax = theUnderlying/(sMin/theUnderlying);	// to enforce central placement of the underlying
	}
	if(sMax < theStrike*safetyZoneFactor){
		sMax = theStrike*safetyZoneFactor;
		sMin = theUnderlying/(sMax/theUnderlying);	// to enforce central placement of the underlying
	}
}

void BSMNumericalOption::initializeGrid() {
	theGrid = Array(theGridPoints);
	theGridLogSpacing = (QL_LOG(sMax)-QL_LOG(sMin))/(theGridPoints-1);
	double edx = QL_EXP(theGridLogSpacing);
	theGrid[0] = sMin;
	int j;
	for (j=1; j<theGridPoints; j++)
		theGrid[j] = theGrid[j-1]*edx;
}

void BSMNumericalOption::initializeInitialCondition() {
	thePrices = Array(theGridPoints);
	int j;
	switch (theType) {
	  case Call:
		for(j=0; j<theGridPoints; j++)
			thePrices[j] = QL_MAX(theGrid[j]-theStrike,0.0);
		break;
	  case Put:
		for(j=0; j<theGridPoints; j++)
			thePrices[j] = QL_MAX(theStrike-theGrid[j],0.0);
		break;
	  case Straddle:
		for(j=0; j<theGridPoints; j++)
			thePrices[j] = QL_FABS(theStrike-theGrid[j]);
		break;
	  default:
		throw IllegalArgumentError("BSMEuropeanOption: invalid option type");  
	}
}

void BSMNumericalOption::initializeOperator() {
	theOperator = BSMOperator(theGridPoints, theGridLogSpacing, theRiskFreeRate, theUnderlyingGrowthRate, theVolatility);
	theOperator.setLowerBC(BoundaryCondition(BoundaryCondition::Neumann,thePrices[1]-thePrices[0]));
	theOperator.setHigherBC(BoundaryCondition(BoundaryCondition::Neumann,thePrices[theGridPoints-1]-thePrices[theGridPoints-2]));
}

// Useful functions

double BSMNumericalOption::valueAtCenter(const Array& a) const {
	int jmid = a.size()/2;
	if (a.size() % 2 == 1)
		return a[jmid];
	else 
		return (a[jmid]+a[jmid-1])/2.0;
}


double BSMNumericalOption::firstDerivativeAtCenter(const Array& a, const Array& g) const{
	Require(a.size()==g.size(),"BSMNumericalOption::firstDerivativeAtCenter, a and g must be of the same size");
	Require(a.size()>=3,"BSMNumericalOption::firstDerivativeAtCenter, the size of the two vectors must be at least 3");
	int jmid = a.size()/2;
	if(a.size() % 2 == 1)
		return (a[jmid+1]-a[jmid-1])/(g[jmid+1]-g[jmid-1]);
	else
		return (a[jmid]-a[jmid-1])/(g[jmid]-g[jmid-1]);
}

double BSMNumericalOption::secondDerivativeAtCenter(const Array& a, const Array& g) const{
	Require(a.size()==g.size(),"BSMNumericalOption::secondDerivativeAtCenter, a and g must be of the same size");
	Require(a.size()>=4,"BSMNumericalOption::secondDerivativeAtCenter, the size of the two vectors must be at least 4");
	int jmid = a.size()/2;
	if(a.size() % 2 == 1){
		double deltaPlus = (a[jmid+1]-a[jmid])/(g[jmid+1]-g[jmid]);
		double deltaMinus = (a[jmid]-a[jmid-1])/(g[jmid]-g[jmid-1]);
		double dS = (g[jmid+1]-g[jmid-1])/2.0;
		return (deltaPlus-deltaMinus)/dS;	
	}		  
	else{
		double deltaPlus = (a[jmid+1]-a[jmid-1])/(g[jmid+1]-g[jmid-1]);
		double deltaMinus = (a[jmid]-a[jmid-2])/(g[jmid]-g[jmid-2]);
		return (deltaPlus-deltaMinus)/(g[jmid]-g[jmid-1]);	
	}
}


QL_END_NAMESPACE(Pricers)

QL_END_NAMESPACE(QuantLib)
