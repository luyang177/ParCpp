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

	//TODO: use Map2 implement Map
	template<typename TResult>
	Par<TResult> Map(std::function<TResult(T)> func) const
	{
		auto par = this;
		Par<TResult> resultPar([par, func](ExecutorContext ctx) {
			auto k = par->Run(ctx)->Get();
			return std::make_shared<UnitFuture<TResult>>(func(k));
		});
		return resultPar;
	}

	template<typename TOther, typename TResult>
	Par<TResult> Map2(Par<TOther> other, std::function<TResult(T, TOther)> func) const
	{
		auto par = this;
		Par<TResult> resultPar([par, other, func](ExecutorContext ctx) {
			auto result = par->Run(ctx)->Get();
			auto otherResult = other.Run(ctx)->Get();
			return std::make_shared<UnitFuture<TResult>>(func(result, otherResult));
		});
		return resultPar;
	}

	template<typename TResult>
	Par<TResult> Flatmap(std::function<Par<TResult>(T)> func) const
	{
		auto par = this;
		Par<TResult> resultPar([par, func](ExecutorContext ctx) {
			auto k = par->Run(ctx)->Get();
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

	template<typename T>
	static Par<T> LazyUnit(std::function<T()> lazyValue)
	{
		return Fork<T>([lazyValue] {
			return Unit(lazyValue());
		});
	}

	template<typename T>
	static Par<T> Fork(std::function<Par<T>()> lazyPar)
	{
		Par<T> par([lazyPar](ExecutorContext ctx) {
			return ctx.Start<T>([lazyPar, &ctx] {
				return lazyPar().Run(ctx)->Get();
			});
		});
		return par;
	}

	template<typename A, typename B>
	static std::function<Par<B>(A)> AsyncF(std::function<B(A)> func)
	{
		return [func](A a) {
			return ParHelper::LazyUnit<B>([func, a] {
				return func(a);
			});
		};
	}
};

#endif
