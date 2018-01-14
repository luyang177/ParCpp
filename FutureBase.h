#ifndef FUTUREBASE_H
#define FUTUREBASE_H

template<typename T>
class FutureBase
{
public:
	virtual ~FutureBase() { }	
	virtual T Get() = 0;
};

#endif
