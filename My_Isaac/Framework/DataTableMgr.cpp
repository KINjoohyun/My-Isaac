#include "stdafx.h"
#include "DataTableMgr.h"
#include "StringTable.h"

void DataTableMgr::LoadAll()
{
	tables.insert({ DataTable::Ids::String, new StringTable() });

	for (auto table : tables)
	{
		if (!table.second->Load())
		{
			std::cerr << "ERROR: DataTableMgr::LoadAll()" << std::endl;
			exit(-1);
		}
	}
}
void DataTableMgr::ReleaseAll()
{
	for (auto table : tables)
	{
		//it.second->Release();
		delete table.second;
	}
	tables.clear();
}