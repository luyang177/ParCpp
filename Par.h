#ifndef PAR_H
#define PAR_H

#include "UnitFuture.h"
#include "ExecutorContext.h"
#include <utility>
#include <functional>
#include <memory>

//Par<T> is ExecutorContext => std::shared_ptr<FutureBase<T>>
template<typename T>
class Par
{
	typedef std::function<std::shared_ptr<FutureBase<T>>(ExecutorContext)> ParType;
public:
	Par(ParType parValue)
		:m_ParValue(parValue)
	{

	}

	std::shared_ptr<FutureBase<T>> Run(ExecutorContext ctx) const
	{
		return m_ParValue(ctx);
	}

	template<typename TResult>
	Par<TResult> Flatmap(std::function<Par<TResult>(T)> func) const
	{
		auto par = this;
		Par<TResult> resultPar([par, func](ExecutorContext ctx) {
			T k = par->Run(ctx)->Get();
			return func(k).Run(ctx);
		});
		return resultPar;
	}
private:
	ParType m_ParValue;
};

class ParHelper
{
public:
	template<typename T>
	static Par<T> Unit(T&& value)
	{
		Par<T> par([value](ExecutorContext ctx) {
			return std::make_shared<UnitFuture<T>>(value);
		});
		return par;
	}

	template<typename T, typename F>
	static Par<T> Fork(F lazyPar)
	{
		Par<T> par([lazyPar](ExecutorContext ctx) {
			return ctx.Start<T>([lazyPar, &ctx]() {
				return lazyPar().Run(ctx)->Get();
			});
		});
		return par;
	}
};

#endif
