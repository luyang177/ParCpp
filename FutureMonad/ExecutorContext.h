#ifndef EXECUTORCONTEXT_H
#define EXECUTORCONTEXT_H

#include "StdFuture.h"
#include <functional>
#include <future>
#include <utility>
#include <memory>

class ExecutorContext
{
public:	
	static ExecutorContext& Instance()
	{
		static ExecutorContext ctx;
		return ctx;
	}

	template<typename T>
	std::shared_ptr<FutureBase<T>> Start(std::function<T()> action)
	{
		auto future = std::async(std::launch::async, action);
		return std::make_shared<StdFuture<T>>(std::move(future));
	}
};

#endif
