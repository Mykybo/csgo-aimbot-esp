#pragma once
#include <string>
#include <unordered_map>


struct RecvTable;
class NetVar
{
public:
	struct netvar_info_t {
		std::string name;
		unsigned long offset;
	};

	typedef std::unordered_map<std::string, netvar_info_t> netvar_pair_t;
	
	struct netvar_table_props_t {
		std::string baseclass;
		netvar_pair_t pair;
	};

	typedef std::unordered_map<std::string, netvar_table_props_t*> netvar_table_t;

	void Init();
	void ExpandNetVar();
	void DumpTable(RecvTable* table);

	netvar_pair_t* GetNetVar(std::string tableName);
	unsigned long GetNetVar(std::string tableName, std::string key);

	netvar_table_t m_netVarTable;
};

extern NetVar g_NetVars;