// Run-Time Type Identification
// ����ʱ����ʶ��

#include <iostream>
#include "RTTI.h"

// ������� RTTI ���÷�

class A {
public:
	virtual VSRtti& GetType()const { return ms_Type; }
	static VSRtti ms_Type;
	bool IsSameType(const A* pObject)const;
	bool IsDerived(const A* pObject)const;
	bool IsSameType(const VSRtti& Type)const;
	bool IsDerived(const VSRtti& Type)const;
};
// ������A�� RTTI����̬ȫ�ֱ�����
TCHAR strA[2] = "A";
VSRtti A::ms_Type(strA, nullptr, nullptr);

bool A::IsSameType(const A* pObject)const { return pObject && GetType().IsSameType(pObject->GetType()); }
bool A::IsDerived(const A* pObject)const { return pObject && GetType().IsDerived(pObject->GetType()); }

bool A::IsSameType(const VSRtti& Type)const { return GetType().IsSameType(Type); }
bool A::IsDerived(const VSRtti& Type)const { return GetType().IsDerived(Type); }



class B : public A {
public:
	virtual VSRtti& GetType()const { return ms_Type; }
	static VSRtti ms_Type;
};
// ������B��RTTI
const TCHAR* strB = {0};
VSRtti B::ms_Type(strB, A::ms_Type, nullptr);



class C : public A {
public:
	virtual VSRtti& GetType()const { return ms_Type; }
	static VSRtti ms_Type;
};
// ������C��RTTI
TCHAR strC[2] = "C";
VSRtti C::ms_Type(strC, A::ms_Type, nullptr);


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// ��ʵ����һ�ֱ�RTTI�����õķ������Ǿ����麯��
// ������Ŀ������õ����ַ���
// ���ַ�����Լ򵥣��ж����͵�ʱ���ٶ�Ҳ�ܿ�
class A {
public:
	enum ObjectType { OT_A, OT_B, OT_C, OT_MAX };
	virtual ObjectType GetObjectType() { return OT_A; }
};

class B : public A {
public:
	virtual ObjectType GetObjectType() { return OT_B; }
};

class C : public A {
public:
	virtual ObjectType GetObjectType() { return OT_C; }
};

void test() {
	if (p2->GetObjectType() == A::OT_B) {
		//ִ��ĳЩ����
	}
	if (p3->GetObjectType() == A::OT_C) {
		//ִ��ĳЩ����
	}
}