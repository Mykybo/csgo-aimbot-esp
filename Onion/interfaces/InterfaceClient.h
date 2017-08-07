#pragma once
#include "BaseInterface.h"
struct RecvTable;

struct RecvProp
{
	const char* m_pVarName;
	unsigned char __pad0[12 + 1 + 20]; // 3 int + 1 bool + 5 ptr
	RecvTable* m_pDataTable;
	int	m_Offset;
	int m_ElementStride;
	int	m_nElements;
	const char* m_pParentArrayPropName;
};

struct RecvTable
{
	RecvProp* m_pProps;
	int	m_nProps;
	void* unused;
	const char* m_pNetTableName;
	unsigned char __pad0[2];
};
struct ClientClass
{
private:
	unsigned char __pad0[8];
public:
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	int m_ClassID;
};
class CClient extends BaseInterface
{
public:
	ClientClass* GetAllClasses() { VRESULT(ClientClass*, 8, Args())(); }
};

extern CClient *g_pClient;