#ifndef UNITFUTURE_H
#define UNITFUTURE_H

#include "FutureBase.h"

template<typename T>
class UnitFuture : public FutureBase<T>
{
public:
	UnitFuture(const T& value)
		:m_Value(value)
	{

	}

	T Get() override
	{
		return m_Value;
	}
private:
	T m_Value;
};

#endif