#pragma once

#ifndef TRIG_FUNCTIONS_H

#include <iostream>

class TrigFunction {
public:
	virtual float execute(float) = 0;
};

class Sin : public TrigFunction {
public:
	float execute(float value) override { return sin(value); }
};


class Cos : public TrigFunction {
public:
	float execute(float value) override { return cos(value); }
};


class CosSquared : public TrigFunction {
public:
	float execute(float value) override { return cos(value)*cos(value); }
};

class SinSquared : public TrigFunction {
public:
	float execute(float value) override { return sin(value)*sin(value); }
};

class SinCos : public TrigFunction {
public:
	float execute(float value) override { return cos(value) * sin(value); }
};


class None : public TrigFunction {
public:
	None() {}
	None(float default_value) : d_value(default_value) {}

	float execute(float value) override { return d_value; }
private:
	float d_value = 0;
};

#endif /*TRIG_FUNCTIONS_H*/