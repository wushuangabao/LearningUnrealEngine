// Run-Time Type Identification
// 运行时类型识别

#include <iostream>
#include "RTTI.h"

// 下面给出 RTTI 的用法

class A {
public:
	virtual VSRtti& GetType()const { return ms_Type; }
	static VSRtti ms_Type;
	bool IsSameType(const A* pObject)const;
	bool IsDerived(const A* pObject)const;
	bool IsSameType(const VSRtti& Type)const;
	bool IsDerived(const VSRtti& Type)const;
};
// 定义类A的 RTTI（静态全局变量）
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
// 定义类B的RTTI
const TCHAR* strB = {0};
VSRtti B::ms_Type(strB, A::ms_Type, nullptr);



class C : public A {
public:
	virtual VSRtti& GetType()const { return ms_Type; }
	static VSRtti ms_Type;
};
// 定义类C的RTTI
TCHAR strC[2] = "C";
VSRtti C::ms_Type(strC, A::ms_Type, nullptr);


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//
// 其实还有一种比RTTI更好用的方法，那就是虚函数
// 天龙项目里就有用到这种方法
// 这种方法相对简单，判断类型的时候速度也很快
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
		//执行某些操作
	}
	if (p3->GetObjectType() == A::OT_C) {
		//执行某些操作
	}
}