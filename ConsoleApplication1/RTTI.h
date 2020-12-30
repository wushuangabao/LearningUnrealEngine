// Run-Time Type Identification
// 运行时类型识别

#pragma once

#define TCHAR char

class VSObject
{};

class VSString
{};

class VSMemObject
{};

typedef VSObject* (*CreateObjectFun)();

class VSRtti : public VSMemObject
{
public:
	VSRtti(const TCHAR* pcRttiName, VSRtti* pBase, CreateObjectFun COF);

	inline bool IsSameType(const VSRtti& Type) const {
		return (&Type == this);
	}
	inline bool IsDerived(const VSRtti& Type) const {
		const VSRtti* pTemp = this;
		while (!pTemp->IsSameType(Type)) {
			if (pTemp->m_pBase)
				pTemp = pTemp->m_pBase;
			else
				return 0;
		}
		return 1;
	}

	inline VSRtti* GetBase()const {
		return m_pBase;
	}
	inline const VSString& GetName()const {
		return m_cRttiName;
	}

private:
	VSString m_cRttiName;
	VSRtti* m_pBase;
};

