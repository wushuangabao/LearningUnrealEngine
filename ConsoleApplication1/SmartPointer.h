#pragma once

/////////////////////////////////////////////////
/// ���࣬���д����̳е����඼����֧������ָ��
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
        //VSLockedIncrement((long*)&m_iReference); //����һ���̰߳�ȫ�ĺ��������Ա�֤�ڶ��̳߳����±����ļ�1��ԭ�Ӳ�����
        m_iReference++; //��������Ƕ��߳����⣬��ôֱ�������д���Ϳ����ˡ�
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
/// ����ָ����
////////////////////////////////////////////
template <class T>
class VSPointer
{
public:
    VSPointer(T* pObject = 0);
    VSPointer(const VSPointer& rPointer);
    ~VSPointer();

    T& operator* () const;  // * ȡָ��ָ��Ķ���
    T* operator-> () const; // ->������ʵָ��
    operator T* () const;   // ��ʽת������������Ĳ�������ָͨ�룬��ô���ݵ�ְ��ָ�����ʽ��ת��Ϊ��ָͨ��

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