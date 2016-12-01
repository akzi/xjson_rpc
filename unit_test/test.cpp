#include "../include/dispatcher.hpp"
#include <iostream>
#include <cassert>

xtest_run;

int func1(int a, int b)
{
	return 0;
}

void func2(int a)
{
}

void func3(int a,int b)
{
}
struct func4
{
	void operator()()
	{
	}
};

struct func5
{
	int operator()(int a)
	{
		return a;
	}
};

struct func7 
{
	void call()
	{
	}
};

using namespace xjson_rpc;

XTEST_SUITE(dispatcher)
{
	XUNIT_TEST(regist)
	{
		dispatcher disp;
		func4 f4;
		func5 f5;
		func7 f7;

		disp.regist("func1", func1);
		disp.regist("func2", func2);
		disp.regist("func3", func3);
		disp.regist("func4", f4);
		disp.regist("func5", f5);
		disp.regist("func7", &func7::call, f7);
		xjson::obj_t vec;
		vec.add(1);
		vec.add(2);
	}

	XUNIT_TEST(invoke)
	{
		dispatcher dispth;

		dispth.regist("func5", func5());

		xjson::obj_t o;
		o.add(1);
		auto ret =  dispth.invoke("func5",o);
		xassert(ret.type() == xjson::obj_t::e_num);
		xassert(ret.get<int>() == 1);
	}
}
