// ����ָ��
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
        m_pObject->IncreRef(); //��ʾָ�����������������Reference��1
    }
}

template <class T>
VSPointer<T>::VSPointer (const VSPointer& rPointer)
{
    m_pObject=rPointer.m_pObject;
    if (m_pObject){
        m_pObject->IncreRef(); //��ʾָ�����������������Reference��1
    }
}

template <class T>
VSPointer<T>::~VSPointer ()
{
    if (m_pObject){
        m_pObject->DecreRef(); //��ʾ����ָ�����������������Reference��1
        m_pObject=NULL;
    }
}

template <class T>
VSPointer<T>& VSPointer<T>::operator= (T * pObject)
{
    if (m_pObject != pObject){
        if (pObject){
            //��ʾָ�����������������Reference��1
            pObject->IncreRef();
        }
        if (m_pObject){
            //��ʾ����ָ�����������������Reference��1
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
            //��ʾָ�����������������Reference��1
            rPointer.m_pObject->IncreRef();
        }
        if (m_pObject){
            //��ʾ����ָ�����������������Reference��1
            m_pObject->DecreRef();
        }
        m_pObject=rPointer.m_pObject;
    }
    return *this;
}


// ʹ������ָ������ӣ�

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
    A* p2 = new A(); //������һ������m ����referenceΪ0
    p1 = p2;   // m��reference��Ϊ1
    std::cout << p1->a << std::endl;

    VSPointer<A> p3(new A(1)); //������һ������n ����referenceΪ1
    p1 = p3;   // m��reference��Ϊ0 ǿ�����٣�p2�����Ұָ�룻n��reference��Ϊ2
    std::cout << p1->a << std::endl;

    // ��������ʱ p1 p3�ֱ�������n��reference��Ϊ0 ǿ������
}

// 1. ����ָ�����ָͨ�벻Ҫ���ã���ö�������ָ�롣���������������Ұָ�����
// 2. �������ββ�Ҫ������ָ�룡
// 3. ������ָ�빹�ɻ���ָ���ָ����Σ�յ�

// Ϊ�˱����߳�������ָ�붨�壬��ʹ�ú궨������ָ��
#define DECLARE_Ptr(ClassName)\
class ClassName;\
    typedef VSPointer<ClassName> ##ClassName##Ptr;
// VSPointer<A>�Ϳ�����APtr��������