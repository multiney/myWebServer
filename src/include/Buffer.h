#pragma once

#include <string>

class Buffer
{
private:
    std::string buf_;
public:
    Buffer() = default;
    ~Buffer() = default;

    void Append(const char* _str);
    size_t Size();
    const char* CStr();
    void Clear();
    void GetLine();
    void SetBuf(const char*);
};
