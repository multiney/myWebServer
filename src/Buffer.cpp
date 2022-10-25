#include "Buffer.h"

#include <iostream>

Buffer::Buffer() {

}

Buffer::~Buffer() {

}

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
