#pragma once

/////////////////////////////////////////////////
/// 基类，所有从它继承的子类都可以支持智能指针
/////////////////////////////////////////////////
class VSReference
{
public:
    inline int GetRef()const
    {
        return m_iReference;
    }

    inline void IncreRef()
    {
        //VSLockedIncrement((long*)&m_iReference); //这是一个线程安全的函数，可以保证在多线程场景下变量的加1是原子操作。
        m_iReference++; //如果不考虑多线程问题，那么直接用这行代码就可以了。
    }

    inline void DecreRef()
    {
        m_iReference--;
        //VSLockedDecrement((long *)&m_iReference);
        if (m_iReference == 0)
        {
            //VS_DELETE this;
            delete this;
        }
    }

private:
    int m_iReference = 0;
};

////////////////////////////////////////////
/// 智能指针类
////////////////////////////////////////////
template <class T>
class VSPointer
{
public:
    VSPointer(T* pObject = 0);
    VSPointer(const VSPointer& rPointer);
    ~VSPointer();

    T& operator* () const;  // * 取指针指向的对象
    T* operator-> () const; // ->访问真实指针
    operator T* () const;   // 隐式转换。如果函数的参数是普通指针，那么传递的职能指针会隐式地转换为普通指针

    VSPointer& operator= (T* pObject);
    VSPointer& operator= (const VSPointer& rReference);

    bool operator== (T* pObject) const;
    bool operator!= (T* pObject) const;
    bool operator== (const VSPointer& rReference) const;
    bool operator!= (const VSPointer& rReference) const;

    //friend class VSStream;

private:
    T* m_pObject;
    inline void SetObject(T* pObject) { m_pObject = pObject; }
    inline T* GetObject()const { return m_pObject; }
};