#include "./include/Buffer.h"

#include <iostream>

void Buffer::append(const char *_str) {
    buf.append(_str);
}

ssize_t Buffer::size() {
    return buf.size();
}

const char* Buffer::c_str() {
    return buf.c_str();
}

void Buffer::clear() {
    buf.clear();
}

void Buffer::getLine() {
    buf.clear();
    std::getline(std::cin, buf);
}

void Buffer::setBuf(const char* _buf) {
    buf.clear();
    buf.append(_buf);
}
