#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <stdexcept>

struct SWdbcHeader
{
	uint32_t magic;
	uint32_t recordCount;
	uint32_t fieldCount;
	uint32_t recordSize;
	uint32_t stringSize;
};

class DBCFile
{
public:
	explicit DBCFile(const std::string& filename);

	// Open database. It must be openened before it can be used.
	void Open();

	class Iterator;
	class Record
	{
	public:
		const float& GetFloat(size_t field) const
		{
			assert(field < file->header.fieldCount);
			return *reinterpret_cast<float*>(offset + field * sizeof(float));
		}
		const uint32_t& GetUInt(size_t field) const
		{
			assert(field < file->header.fieldCount);
			return *reinterpret_cast<uint32_t*>(offset + field * sizeof(uint32_t));
		}
		const int32_t& GetInt(size_t field) const
		{
			assert(field < file->header.fieldCount);
			return *reinterpret_cast<int32_t*>(offset + field * sizeof(int32_t));
		}
		std::string GetString(size_t field) const
		{
			assert(field < file->header.fieldCount);
			size_t stringOffset = GetUInt(field);
			assert(stringOffset < file->header.stringSize);
			return file->stringTable.data() + stringOffset;
		}
		std::string GetLocalizedString(size_t field, int32_t locale = -1) const
		{
			int loc = locale;
			if (locale == -1)
			{
				assert(field < file->header.fieldCount - 8);
				for (loc = 0; loc < 9; loc++)
				{
					size_t stringOffset = GetUInt(field + loc);
					if (stringOffset != 0)
						break;
				}
			}

			assert(field + loc < file->header.fieldCount);
			size_t stringOffset = GetUInt(field + loc);
			assert(stringOffset < file->header.stringSize);
			return file->stringTable.data() + stringOffset;
		}

		Record& operator*()  { return *this; }
		Record& operator->() { return *this; }
		Record& operator++() {
			offset += file->header.recordSize;
			return *this;
		}
		bool operator==(Record& b) { return offset == b.offset; }
		bool operator!=(Record& b) { return offset != b.offset; }
	private:
		Record(const DBCFile* pfile, uint8_t *poffset) : file(pfile), offset(poffset) {}
		const DBCFile* file;
		uint8_t *offset;

		friend class DBCFile;
	};

	inline Record GetRecord(size_t id) { return records[id]; }

	inline Record& begin() { return *records.begin(); }
	inline Record& end()   { return *records.end(); }

	inline size_t GetRecordCount() const { return header.recordCount; }
	inline size_t GetFieldCount() const { return header.fieldCount; }
	inline Record* GetByID(int32_t id, size_t field = 0)
	{
		for (Record& i : *this)
		{
			if (i.GetUInt(field) == id)
				return &i;
		}
		return nullptr;
	}

private:
	std::string filename;
	SWdbcHeader header;
	std::vector<uint8_t> data;
	std::vector<char> stringTable;
	std::vector<Record> records;
};
