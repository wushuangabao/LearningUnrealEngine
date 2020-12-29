// 智能指针
#include "SmartPointer.h"
#include <iostream>

template <class T>
T& VSPointer<T>::operator* () const
{ return *m_pObject; 
}
template <class T>
T* VSPointer<T>::operator-> () const
{ return m_pObject;
}
template <class T>
VSPointer<T>::operator T* ()const
{ return m_pObject;
}

template <class T>
bool VSPointer<T>::operator== (T* pObject) const
{    return (m_pObject == pObject);
}
template <class T>bool VSPointer<T>::operator!= (T* pObject) const
{    return (m_pObject != pObject);
}
template <class T>
bool VSPointer<T>::operator== (const VSPointer& rPointer) const
{    return (m_pObject == rPointer.m_pObject);
}
template <class T>
bool VSPointer<T>::operator!= (const VSPointer& rPointer) const
{    return (m_pObject != rPointer.m_pObject);
}

template <class T>
VSPointer<T>::VSPointer(T* pObject)
{
    m_pObject = pObject;
    if (m_pObject) {
        m_pObject->IncreRef(); //表示指向这个对象，这个对象的Reference加1
    }
}

template <class T>
VSPointer<T>::VSPointer (const VSPointer& rPointer)
{
    m_pObject=rPointer.m_pObject;
    if (m_pObject){
        m_pObject->IncreRef(); //表示指向这个对象，这个对象的Reference加1
    }
}

template <class T>
VSPointer<T>::~VSPointer ()
{
    if (m_pObject){
        m_pObject->DecreRef(); //表示放弃指向这个对象，这个对象的Reference减1
        m_pObject=NULL;
    }
}

template <class T>
VSPointer<T>& VSPointer<T>::operator= (T * pObject)
{
    if (m_pObject != pObject){
        if (pObject){
            //表示指向这个对象，这个对象的Reference加1
            pObject->IncreRef();
        }
        if (m_pObject){
            //表示放弃指向这个对象，这个对象的Reference减1
            m_pObject->DecreRef();
        }
        m_pObject=pObject;
    }
    return *this;
}

template <class T>
VSPointer<T>& VSPointer<T>::operator= (const VSPointer& rPointer)
{
    if (m_pObject != rPointer.m_pObject){
        if (rPointer.m_pObject){
            //表示指向这个对象，这个对象的Reference加1
            rPointer.m_pObject->IncreRef();
        }
        if (m_pObject){
            //表示放弃指向这个对象，这个对象的Reference减1
            m_pObject->DecreRef();
        }
        m_pObject=rPointer.m_pObject;
    }
    return *this;
}


// 使用智能指针的例子：

class A :public VSReference
{
public:
    A() {}
    A(int i):a(i) {}
    int a = 0;
};

void test()
{
    VSPointer<A> p1 = NULL;
    A* p2 = new A(); //申请了一个对象m 它的reference为0
    p1 = p2;   // m的reference变为1
    std::cout << p1->a << std::endl;

    VSPointer<A> p3(new A(1)); //申请了一个对象n 它的reference为1
    p1 = p3;   // m的reference变为0 强制销毁，p2变成了野指针；n的reference变为2
    std::cout << p1->a << std::endl;

    // 函数结束时 p1 p3分别析构，n的reference变为0 强制销毁
}

// 1. 智能指针和普通指针不要混用，最好都用智能指针。否则会有如上例的野指针产生
// 2. 函数的形参不要用智能指针！
// 3. 用智能指针构成环形指向的指针是危险的

// 为了避免冗长的智能指针定义，会使用宏定义智能指针
#define DECLARE_Ptr(ClassName)\
class ClassName;\
    typedef VSPointer<ClassName> ##ClassName##Ptr;
// VSPointer<A>就可以用APtr来代替了