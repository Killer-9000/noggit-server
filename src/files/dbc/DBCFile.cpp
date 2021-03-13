#include "DBCFile.h"
#include "log.h"

#include <string>

DBCFile::DBCFile(const std::string& _filename)
{
	filename = _filename;
}

void DBCFile::Open()
{
    FILE* f = fopen(filename.c_str(), "rb");
    if (!f)
    {
        LOG_FATAL("Error opening dbc (%s).", filename);
        return;
    }

    if (fread(&header, sizeof(SWdbcHeader), 1, f) != 1)
    {
        LOG_FATAL("Error reading dbc (%s) header.", filename);
        fclose(f);
        return;
    }

    data.resize(header.recordSize * header.recordCount);
    stringTable.resize(header.stringSize);

    if (fread(data.data(), header.recordSize, header.recordCount, f) 
      != header.recordCount)
    {
      LOG_FATAL("Error reading dbc (%s) data.", filename);
      fclose(f);
      return;
    }
    if (fread(stringTable.data(), sizeof(char), header.stringSize, f) 
      != header.stringSize)
    {
      LOG_FATAL("Error reading dbc (%s) strings.", filename);
      fclose(f);
      return;
    }

	records.reserve(header.recordCount);
	for (int i = 0; i < header.recordCount; i++)
		records.emplace_back(Record(this, data.data() + (i * header.recordSize)));

    fclose(f);
}
