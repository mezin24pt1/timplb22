#include "table_cipher.h"
#include <vector>
#include <stdexcept>
#include <string>


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


std::wstring cleanRussianText(const std::wstring& s) {
    std::wstring result;
    for (wchar_t c : s) {
        if (isRussianLetter(c)) {
            result += toUpperRussian(c);
        }
    }
    return result;
}

TableCipher::TableCipher(int key) {
    if (key <= 0) {
        throw cipher_error("Ключ должен быть положительным числом");
    }
    columns = key;
}

std::wstring TableCipher::encrypt(const std::wstring& plain_text) {
    if (plain_text.empty()) {
        throw cipher_error("Текст не может быть пустым");
    }

    std::wstring cleanText = cleanRussianText(plain_text);
    if (cleanText.empty()) {
        throw cipher_error("Недопустимые символы");
    }

    int text_length = static_cast<int>(cleanText.length());
    int rows = (text_length + columns - 1) / columns;
    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(columns, L' '));
    int index = 0;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (index < text_length) {
                table[row][col] = cleanText[index++];
            } else {
                table[row][col] = L'Я'; 
            }
        }
    }

    std::wstring result;
    for (int col = columns - 1; col >= 0; --col) {
        for (int row = 0; row < rows; ++row) {
            result += table[row][col];
        }
    }
    return result;
}

std::wstring TableCipher::decrypt(const std::wstring& cipher_text) {
    if (cipher_text.empty()) {
        throw cipher_error("Текст не может быть пустым");
    }

    int total_chars = static_cast<int>(cipher_text.length());
    if (total_chars % columns != 0) {
        throw cipher_error("Длина зашифрованного текста не кратна числу столбцов");
    }

    int rows = total_chars / columns;
    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(columns, L' '));
    int index = 0;

    for (int col = columns - 1; col >= 0; --col) {
        for (int row = 0; row < rows; ++row) {
            wchar_t c = cipher_text[index++];
            if (!((c >= L'А' && c <= L'Я') || c == L'Ё')) {
                throw cipher_error(std::string("Недопустимый символ в шифротексте: ") + static_cast<char>(c));
            }
            table[row][col] = c;
        }
    }

    std::wstring result;
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            result += table[row][col];
        }
    }

    while (!result.empty() && result.back() == L'Я') {
        result.pop_back();
    }
    return result;
}
