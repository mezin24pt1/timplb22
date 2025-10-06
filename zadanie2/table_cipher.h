#pragma once
#include <string>
#include <stdexcept>

class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error(const std::string& what_arg) : std::invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg) : std::invalid_argument(what_arg) {}
};

class TableCipher
{
private:
    int columns;
public:
    TableCipher(int key);
    std::wstring encrypt(const std::wstring& plain_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};
