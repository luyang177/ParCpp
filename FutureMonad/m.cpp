#include "Par.h"
#include <string>

int f1()
{
	return 1;
}

int f2()
{
	return 2;
}

std::string f3()
{
	return " abcd ";
}


void TestUnit()
{
	auto& ctx = ExecutorContext::Instance();
	auto par = ParHelper::Unit(f1());
	auto r = par.Run(ctx)->Get();
}

void TestFork()
{
	auto& ctx = ExecutorContext::Instance();

	auto par_fork = ParHelper::Fork<int>([] {
		return ParHelper::Unit(f1());
	});
	auto r = par_fork.Run(ctx)->Get();
}

void TestMap()
{
	auto& ctx = ExecutorContext::Instance();

	auto par1 = ParHelper::Fork<int>([] {
		return ParHelper::Unit(f1());
	});
	auto par2 = par1.Map<std::string>([](int value) {
		return std::to_string(value) + "haha";
	});

	auto r = par2.Run(ctx)->Get();
}

void TestMap2()
{
	auto& ctx = ExecutorContext::Instance();

	auto par1 = ParHelper::Fork<int>([] {
		return ParHelper::Unit(f1());
	});

	auto par2 = ParHelper::Fork<std::string>([] {
		return ParHelper::Unit(f3());
	});

	auto par3 = par1.Map2<std::string, std::string>(par2, [](int a, std::string b) {
		return std::to_string(a) + b + " haha";
	});

	auto r = par3.Run(ctx)->Get();
}

void TestFlatmap()
{
	auto& ctx = ExecutorContext::Instance();

	auto par1 = ParHelper::Fork<int>([] {
		return ParHelper::Unit(f1());
	});
	auto par2 = par1.Flatmap<int>([](int value){
		return ParHelper::Fork<int>([] {
			return ParHelper::Unit(f2());
		});
		//return ParHelper::Unit(f2());
	});
	auto r = par2.Run(ctx)->Get();
}

void TestLazyUnit()
{
	auto& ctx = ExecutorContext::Instance();

	auto par = ParHelper::LazyUnit<int>([] {
		return 20;
	});
	auto r = par.Run(ctx)->Get();
}

void TestAsyncF()
{
	auto& ctx = ExecutorContext::Instance();

	auto f = ParHelper::AsyncF<int, std::string>([](int n) {
		return std::to_string(n) + " count";
	});
	auto r = f(12).Run(ctx)->Get();
}

int main()
{
	//TestUnit();
	//TestFork();
	TestMap();
	//TestMap2();
	//TestFlatmap();

	//TestLazyUnit();
	//TestAsyncF();

	return 0;
}