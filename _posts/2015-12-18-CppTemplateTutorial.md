---
layout: post
title: "CppTemplateTourial学习"
date: 2015-12-18-15:38
categories: C++ template
---
 
	偶然在知乎看到大神写的这篇cpp template的文章，觉得受益匪浅，可惜作者尚未写完，很是期待。。。
这是我看文章的过程中，顺手摘抄作者所写的代码...

文章链接：<https://github.com/wuye9036/CppTemplateTutorial>

	#include <iostream>
	using namespace std;

	template <typename T>
	class TypeToID
	{
	public:
	    static int const NotID = -1;
	};

	template <>
	class TypeToID<float>
	{
	public:
	    static int const ID = 1;
	};

	template <typename T>
	class TypeToID<T*>
	{
	public:
	    typedef T SameAsT;
	    static int const ID = 0x80000000;
	};

	void PrintID()
	{
	    cout << "ID of float*:" << TypeToID< TypeToID<float*>::SameAsT >::ID << endl;
	}


	template <typename T>
	class RemovePointer
	{

	};

	template <typename T>
	class RemovePointer<T*>
	{
	public:
	    typedef T Result;
	};
	void Foo()
	{
	    RemovePointer<float*>::Result x = 5.0f;
	    std::cout << x << std::endl;
	}

	template <typename T>
	struct X
	{

	};

	template <typename T>
	struct Y
	{
	    typedef X<T> reboundType;
	    typedef typename X<T>::MemberType MemberType;
	    //typedef UnknownType MemberType3;


	    void foo()
	    {
		X<T> instance0;
		typename X<T>::MemberType instance1;
		//WTF instance2;
		//大王叫我来巡山 - + &
	    }
	};


	/*
	// overload
	void doWork( int );
	void doWork(float);
	void doWork(int, int);
	void f ()
	{
	    doWork(0);
	    doWork(0.5f);
	    doWork(1,1);
	}
	*/

	template <typename T> struct DoWork;  // (0) 这是原型
	template <> struct DoWork<int> {}     // (1) 这是int类型的"重载“"
	template <> struct DoWork<float> {};  // (2) 这是float类型的重载
	template <> struct DoWork<int, int> {} // (3) 这是int int类型的重载

	template <typename T0, typename T1 = void> struct X
	{
	    static void call(T0 const &p0, T1 const &p1)
	    {
		std::cout << " X be called " << std::endl;
	    }
	}
	template <typename T0> struct X<T0>
	{
	    static void call(T0 const &p0)
	    {
		std::cout << "Y be called!" << std::endl;
	    }
	}


	// Boost.Tuple
	template <
	    class T0 = null_type, class T1 = null_type, class T2 = null_type,
	    class T3 = null_type, class T4 = null_type, class T5 = null_type,
	    class T6 = null_type, class T7 = null_type, class T8 = null_type,
	    class T9= null_type>
	class tuple;
	// Tuple的一些用例
	tuple<int> a;
	tuple<double&,const double&, const double, double*, const double*> b;
	tuple<A, int(*)(char, int), B(A::*)(C&), C> c;
	tuple<std::string, std::pair<A, B> > d;
	tuple<A*, tuple<const A*, const B&, C> , bool, void *> e;

	// C++11 变参模板(Variadic Template) 通常只能放在最后面
	template <typename... Ts> class tuple;

	int main()
	{
	    //PrintID();
	    Foo();
	    return 0;
	}

