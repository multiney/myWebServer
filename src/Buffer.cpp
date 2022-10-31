#include "./include/Buffer.h"

#include <iostream>

void Buffer::Append(const char *_str) {
    buf_.append(_str);
}

size_t Buffer::Size() {
    return buf_.size();
}

const char* Buffer::CStr() {
    return buf_.c_str();
}

void Buffer::Clear() {
    buf_.clear();
}

void Buffer::GetLine() {
    buf_.clear();
    std::getline(std::cin, buf_);
}

void Buffer::SetBuf(const char* _buf) {
    buf_.clear();
    buf_.append(_buf);
}
