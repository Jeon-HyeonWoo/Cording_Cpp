#define _CRTDBG_MAP_ALLOC
#include "cstdlib"
#include "crtdbg.h"

#ifdef _DEBUG
	#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define DBG_NEW new
#endif

#include "iostream"
#include "type_traits"
#include "string"
#include "utility"
#include "typeinfo"
#include "memory"
#include "Windows.h"
#include "stdlib.h"

using namespace std;

class DebugOutput
{
public:
	
	template<typename T>
	static void IsValid(unique_ptr<T>& _Uptr, int line)
	{
		if (_Uptr == nullptr)
		{
			OutputDebugStringA(("Nullptr " + to_string(line) + "\n").c_str());
		}
	}
};

class JClass
{
public:

	JClass(int _num) : num(_num) {};

	template<typename T>
	JClass(T& Other) : num(0) {};

	template<typename T>
	JClass(T&& Other) noexcept {};

public:

	int GetNum() const { return num; }
	void SetNum(int _num) { num = _num; }
	
private:

	int num;
	
};

int main(void)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	DebugOutput DebugCheck;

	unique_ptr<JClass> uptr = make_unique<JClass>(3);


	//1. ����
	unique_ptr<JClass>& uptr3 = uptr;
	uptr3->SetNum(5);
	cout << uptr3->GetNum() << endl;
	
	
	//2. Raw Pointer�� ����
	JClass* ptr = uptr.get();
	ptr->SetNum(7);
	cout << ptr->GetNum() << endl;

	//3. ������ ����
	unique_ptr<JClass> ReUPtr = move(uptr);
	DebugCheck.IsValid(uptr, __LINE__);
	
	//3. ������ ���� -> Raw Pointer�� ���
	JClass* RePtr = ReUPtr.release();
	DebugCheck.IsValid(ReUPtr, __LINE__);
	
	//4. ����Ʈ �����ͷ� ������
	unique_ptr<JClass> RRptr(RePtr);
	DebugCheck.IsValid(RRptr, __LINE__);

	//5. ������ ����
	RRptr.reset();
	DebugCheck.IsValid(RRptr, __LINE__);
}

/*
	Unique_Ptr's fucntion

	Reset() : �����Ͱ� ����Ű�� ���� ��ü�� ����(Delete)�ϰ� Nullptr�� �ʱ�ȭ
	Release() : �����Ͱ� ����Ű�� ���� ��ü�� �������� �����ϰ� nullptr�� �ʱ�ȭ, ��ü�� ����ֵ� Raw ponter�� ��ƾ� ��

	Release() T* �� ��ȯ�ϰ� �����ʹ� ���� ����, ��ü�� ���� ����
	Reset() : ���� ���� ��ü�� delete�ϰ� nullptr�� �ʱ�ȭ
	
*/