#ifndef __TYPE_MANAGER_HH__
#define __TYPE_MANAGER_HH__

#include <vector>
#include <string>

namespace cint
{

struct fieldInfo
{
    unsigned long offset;
    unsigned long len;
    int typeNum;
    std::string name;
};

class typeInfo
{
    int typeNum;                      // number of the type
    unsigned long typeSize;           // size of the type
    std::string typeName;             // name of the type
    std::vector<fieldInfo> fields;    // fields contained in the type
public:
    inline explicit typeInfo(int _typeNum, unsigned long _size,
                             const std::string &_typeName,
                             const std::vector<fieldInfo> &_fields) noexcept;
    inline explicit typeInfo(int _typeNum, unsigned long _size,
                             std::string &&_typeName,
                             std::vector<fieldInfo> &&_fields) noexcept;
    inline bool isBasic() noexcept;
    fieldInfo getFieldByName(const std::string queryName);
};

}

#endif
