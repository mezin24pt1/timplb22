#include "modAlphaCipher.h"
#include <algorithm>
#include <cctype>


bool isRussianLetter(wchar_t c) {
    return (c >= L'А' && c <= L'Я') || c == L'Ё' ||
           (c >= L'а' && c <= L'я') || c == L'ё';
}


wchar_t toUpperRussian(wchar_t c) {
    if (c >= L'а' && c <= L'я') {
        return c - (L'а' - L'А');
    }
    if (c == L'ё') return L'Ё';
    return c;
}

std::wstring modAlphaCipher::getValidKey(const std::wstring& s) {
    if (s.empty()) {
        throw cipher_error("Empty key");
    }

    std::wstring tmp;
    for (wchar_t c : s) {
        if (!isRussianLetter(c)) {
            throw cipher_error(std::string("Invalid key character: ") + static_cast<char>(c));
        }
        tmp += toUpperRussian(c);
    }
    return tmp;
}

std::wstring modAlphaCipher::getValidOpenText(const std::wstring& s) {
    std::wstring tmp;
    for (wchar_t c : s) {
        if (isRussianLetter(c)) {
            tmp += toUpperRussian(c);
        }
    }
    if (tmp.empty()) {
        throw cipher_error("Empty open text");
    }
    return tmp;
}


std::wstring modAlphaCipher::getValidCipherText(const std::wstring& s) {
    if (s.empty()) {
        throw cipher_error("Empty cipher text");
    }
    for (wchar_t c : s) {
        if (!((c >= L'А' && c <= L'Я') || c == L'Ё')) {
            throw cipher_error(std::string("Invalid cipher text character: ") + static_cast<char>(c));
        }
    }
    return s;
}


modAlphaCipher::modAlphaCipher(const std::wstring& skey) {
    for (size_t i = 0; i < numAlpha.size(); ++i) {
        alphaNum[numAlpha[i]] = static_cast<int>(i);
    }
    key = convert(getValidKey(skey));
}


std::vector<int> modAlphaCipher::convert(const std::wstring& s) {
    std::vector<int> result;
    for (auto c : s) {
        result.push_back(alphaNum.at(c)); // .at() для безопасности
    }
    return result;
}


std::wstring modAlphaCipher::convert(const std::vector<int>& v) {
    std::wstring result;
    for (auto i : v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}


std::wstring modAlphaCipher::encrypt(const std::wstring& open_text) {
    std::vector<int> work = convert(getValidOpenText(open_text));
    size_t alpha_size = numAlpha.size();
    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = (work[i] + key[i % key.size()]) % static_cast<int>(alpha_size);
    }
    return convert(work);
}


std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text) {
    std::vector<int> work = convert(getValidCipherText(cipher_text));
    size_t alpha_size = numAlpha.size();
    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = (work[i] + static_cast<int>(alpha_size) - key[i % key.size()]) % static_cast<int>(alpha_size);
    }
    return convert(work);
}
