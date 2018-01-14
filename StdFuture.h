#ifndef STDFUTURE_H
#define STDFUTURE_H

#include "FutureBase.h"
#include <future>
#include <utility>

template<typename T>
class StdFuture : public FutureBase<T>
{
public:
	StdFuture(std::future<T>&& future)
		:m_Future(std::move(future))
	{

	}

	T Get() override
	{
		auto r = m_Future.get();
		return r;
	}
private:
	std::future<T> m_Future;
};

#endif