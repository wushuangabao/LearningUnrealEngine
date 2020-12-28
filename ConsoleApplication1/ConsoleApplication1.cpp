// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

class A
{
public:
    int Add(int a, int b)
    {
        return a + b;
    }
};

int Add(int a, int b)
{
    return a + b;
}

typedef int (*F)(int, int);
typedef int (A::* CF)(int, int);


void test1()
{
    // 用普通函数指针代替Add函数
    F f = &Add;
    std::cout << f(1, 1) << std::endl;

    // 用类成员函数指针代替A类的Add函数
    A a;
    A* b = &a;
    CF cf = &(A::Add);
    std::cout << (a.*cf)(1, 2) << std::endl;
    std::cout << (b->*cf)(2, 2) << std::endl;
}
// 函数指针、类成员函数指针之间不能互相转换
// 用特殊手法把它们转换为同一种指针类型


////////////////////////////////////////////////////
/// 代理类（粗糙版）
////////////////////////////////////////////////////

class Delegate_1
{
public:
    typedef int(*F)(int, int);
    typedef int(A::* CF)(int, int);
    // 上述函数指针的定义方式导致了对类A存在依赖，而且还存在对函数参数和返回值类型的依赖

    // 把A的类成员函数封装到一个静态函数里
    template<CF cf>
    static int MethodStub(void* p, int a1, int a2)
    {
        A* Ap = (A*)p;
        return (Ap->*cf)(a1, a2);
    }

    // 把普通函数封装到一个静态函数里
    template<F f>
    static int FunStub(void* p, int a1, int a2)  //多写一个参数 p，是为了和 MethodStub 的参数保持一致
    {
        return (f)(a1, a2);
    }
};

// Delegate_1 的用法如下
void test2()
{
    A a;
    int result = Delegate_1::MethodStub<&(A::Add)>(&a, 0, 1);
    std::cout << result << std::endl;

    result = Delegate_1::FunStub<&Add>(NULL, 0, 1); //<>内用 &Add 和用 Add，输出结果是一样的
    std::cout << result << std::endl;

    // 现在可以用同一函数指针了
    typedef int(*FNew)(void* p, int, int);

    FNew fnew = &Delegate_1::MethodStub<&(A::Add)>;
    result = (fnew)(&a, 1, 2);
    std::cout << result << std::endl;

    fnew = &Delegate_1::FunStub<&Add>;
    result = (fnew)(NULL, 1, 2);
    std::cout << result << std::endl;
}


////////////////////////////////////////////////////
/// 代理类（第2版）
/// 利用模板参数让代理不再依赖任何具体的类
/// 也不再依赖具体的参数和返回值类型
////////////////////////////////////////////////////

template<typename R, typename A1, typename A2, class C>
class Delegate_2
{
public:
    Delegate_2()
    {
        m_p = NULL;
        m_F = NULL;
    }

private:
    typedef R(*F)(A1, A2);
    typedef R(C::* CF)(A1, A2);
    typedef R(*FNew)(void* p, A1, A2);

    void* m_p;
    FNew m_F;

    template<CF cf>
    static int MethodStub(void* p, A1 a1, A2 a2)
    {
        C* Ap = (C*)p;
        return (Ap->*cf)(a1, a2);
    }

    template<F f>
    static int FunStub(void* p, A1 a1, A2 a2)
    {
        return (f)(a1, a2);
    }
    
    static Delegate_2 Create(void* p, FNew f)
    {
        Delegate_2 Temp;
        Temp.m_F = f;
        Temp.m_p = p;
        return Temp;
    }

public:
    template<CF cf>
    static Delegate_2 FromMethod(C* p)
    {
        return Create(p, &MethodStub<cf>);
    }

    template<F f>
    static Delegate_2 FromFun()
    {
        return Create(NULL, &FunStub<f>);
    }

    int Execute(int a1, int a2)
    {
        return (*m_F)(m_p, a1, a2);
    }
};

void test3()
{
    A a;
    Delegate_2<int, int, int, A> k = Delegate_2<int, int, int, A>::FromMethod<&A::Add>(&a);
    int r = k.Execute(1, 1);
    std::cout << r << std::endl;
    
    // 普通函数还要加上一个类参数，有点别扭：
    Delegate_2<int, int, int, A> s = Delegate_2<int, int, int, A>::FromFun<&Add>();
    r = s.Execute(1, 2);
    std::cout << r << std::endl;
}


////////////////////////////////////////////////////
/// 代理类（最终版）
/// 移动一下类模板参数 C 的位置
////////////////////////////////////////////////////

template<typename R, typename A1, typename A2>
//template<typename R, typename A1, typename A2, class C>
class Delegate
{
public:
    Delegate()
    {
        m_p = NULL;
        m_F = NULL;
    }

private:
    //typedef R(*F)(A1, A2);
    //typedef R(C::* CF)(A1, A2);
    typedef R(*FNew)(void* p, A1, A2);

    void* m_p;
    FNew m_F;

    //template<CF cf>
    template<class C, R(C::* CF)(A1, A2)>
    static int MethodStub(void* p, A1 a1, A2 a2)
    {
        C* Ap = (C*)p;
        //return (Ap->*cf)(a1, a2);
        return (Ap->*CF)(a1, a2);
    }

    //template<F f>
    template<R(*F)(A1, A2)>
    static int FunStub(void* p, A1 a1, A2 a2)
    {
        //return (f)(a1, a2);
        return (F)(a1, a2);
    }

    static Delegate Create(void* p, FNew f)
    {
        Delegate Temp;
        Temp.m_F = f;
        Temp.m_p = p;
        return Temp;
    }

public:
    //template<CF cf>
    template<class C, R(C::* CF)(A1, A2)>
    static Delegate FromMethod(C* p)
    {
        //return Create(p, &MethodStub<cf>);
        return Create(p, &MethodStub<C, CF>);
    }

    //template<F f>
    template<R(*F)(A1, A2)>
    static Delegate FromFun()
    {
        //return Create(NULL, &FunStub<f>);
        return Create(NULL, &FunStub<F>);
    }

    int Execute(int a1, int a2)
    {
        return (*m_F)(m_p, a1, a2);
    }
};

void test4()
{
    A a;
    Delegate<int, int, int> k = Delegate<int, int, int>::FromMethod<A, &A::Add>(&a);
    int r = k.Execute(1, 2);
    std::cout << r << std::endl;

    Delegate<int, int, int> s = Delegate<int, int, int>::FromFun<&Add>();
    r = s.Execute(1, 2);
    std::cout << r << std::endl;
}


// 前面介绍的代码只是实现代理的第一步，把函数封装了起来
// 要实现多播，还需要再封装一下，就是把前面实现的代理类用数组保存起来，以实现代理的集合


int main()
{
    return 0;
}