#include "Par.h"

int f1()
{
	return 1;
}

int f2()
{
	return 2;
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

int main()
{
	//TestUnit();
	//TestFork();
	TestFlatmap();

	return 0;
}