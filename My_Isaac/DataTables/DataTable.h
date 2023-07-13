#pragma once

class DataTable
{
public:
	enum class Ids
	{
		None = -1,
		String,
		Zombie,
	};

	static const int TotalIds = 1;

	DataTable(const DataTable& copy) = delete;
	bool operator==(const DataTable& other) const = delete;

protected:
	Ids tableId;

public:
	DataTable(DataTable::Ids id) : tableId(id) {}
	virtual ~DataTable() {}

	virtual bool Load() = 0;
	virtual void Release() = 0;
};