/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SamplerBoostImpl.h
 * Author: rlcancian
 *
 * Created on 21 de Outubro de 2019, 17:24
 */

#ifndef SAMPLERBOOSTIMPL_H
#define SAMPLERBOOSTIMPL_H

#include "Sampler_if.h"
//#include <boost/random.hpp>

class SamplerBoostImpl : public Sampler_if {
public:

	struct BoostImplRNG_Parameters : public RNG_Parameters {
		double param;
	};

public:
	SamplerBoostImpl();
	virtual ~SamplerBoostImpl() = default;
public: // probability distributions
	virtual double random();
    virtual double sampleBeta(double alpha, double beta, double infLimit, double supLimit);
    virtual double sampleBeta(double alpha, double beta);
    virtual double sampleErlang(double mean, int M, double offset = 0.0);
    virtual double sampleExponential(double mean, double offset = 0.0);
    //virtual double sampleGamma(double mean, double alpha, double offset=0.0);
    virtual double sampleGamma(double alpha, double beta, double offset = 0.0);
    virtual double sampleGumbell(double mode, double scale);
    virtual double sampleLogNormal(double mean, double stddev, double offset = 0.0);
    virtual double sampleNormal(double mean, double stddev);
    virtual double sampleTriangular(double min, double mode, double max);
    virtual double sampleUniform(double min, double max);
    virtual double sampleWeibull(double alpha, double scale);
public: // discrete probability distributions
    virtual double sampleBinomial(int trials, double p);
    virtual double sampleBernoulli(double p);
    virtual double sampleDiscrete(double prob, double value, ...);
    virtual double sampleDiscrete(double *prob, double *value, int size);
    virtual double sampleGeometric(double p);
public:
	void reset(); //!< reinitialize seed and other parameters so (pseudo) random number sequence will be generated again.
public:
	virtual void setRNGparameters(Sampler_if::RNG_Parameters* param);
	virtual RNG_Parameters* getRNGparameters() const;
private:
	//boost::random::mt19937 _gen;
};

#endif /* SAMPLERBOOSTIMPL_H */

