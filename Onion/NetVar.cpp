#include "NetVar.h"
#include "interfaces/InterfaceClient.h"
#include "Log.h"
#include "Defines.h"

NetVar g_NetVars;
void NetVar::DumpTable(RecvTable* table)
{
	std::string tableName = std::string(table->m_pNetTableName);
	if (m_netVarTable.find(tableName) == m_netVarTable.end()){
		netvar_table_props_t *ptr = new netvar_table_props_t();
		m_netVarTable[tableName] = ptr;
	}

	for (int i = 0; i < table->m_nProps; i++) {
		RecvProp *prop = &table->m_pProps[i];
		if (!prop)
			continue;

		if (!_stricmp(prop->m_pVarName, "baseclass")) {
			m_netVarTable[tableName]->baseclass = std::string(prop->m_pDataTable->m_pNetTableName);
			continue;
		}

		netvar_info_t info;
		info.name = prop->m_pVarName;
		info.offset = prop->m_Offset;

		m_netVarTable[tableName]->pair[prop->m_pVarName] = info;
		
		if (info.name.find("clrRender") != std::string::npos || info.name.find("HasDefuser") != std::string::npos) {
			Log::Write(DEBUG, "%s | %s | 0x%X", tableName.c_str(), info.name.c_str(), info.offset);
		}

		if (prop->m_pDataTable) {
			DumpTable(prop->m_pDataTable);
		}
	}
}

void NetVar::Init()
{
	ExpandNetVar();
	Log::Write(DEBUG, "Found %i NetVar tables", m_netVarTable.size());
}

void NetVar::ExpandNetVar()
{
	ClientClass* it;
	for (it = g_pClient->GetAllClasses(); it; it = it->m_pNext) {
		DumpTable(it->m_pRecvTable);
	}
}


NetVar::netvar_pair_t* NetVar::GetNetVar(std::string tableName)
{
	if (m_netVarTable.find(tableName) != m_netVarTable.end())
		return &m_netVarTable[tableName]->pair;
	return 0;
}

unsigned long NetVar::GetNetVar(std::string tableName, std::string key)
{
	netvar_pair_t* table = GetNetVar(tableName);
	return table && table->find(key) != table->end() ? table->at(key).offset : 0;
}