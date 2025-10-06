#include <iostream>
#include <locale>
#include <codecvt>
#include "table_cipher.h"
using namespace std;

std::wstring stringToWstring(const std::string& str);
std::string wstringToString(const std::wstring& wstr);

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    string key_input;
    cout << "=== Шифр табличной перестановки ===" << endl;
    cout << "Шифр готов. Введите количество столбцов: ";
    getline(cin, key_input);
    try {
        int key = stoi(key_input);
        if (key <= 0) {
            throw cipher_error("Ключ должен быть положительным числом");
        }
        cout << "Ключ загружен" << endl;
        TableCipher cipher(key);

        unsigned op;
        do {
            cout << "Шифр готов. Выберите операцию (0-выход, 1-шифрование, 2-расшифрование): ";
            cin >> op;
            cin.ignore();

            if (op > 2) {
                cout << "Неверная операция" << endl;
            } else if (op > 0) {
                string text_input;
                cout << "Введите текст: ";
                getline(cin, text_input);
                wstring text = stringToWstring(text_input);

                try {
                    if (op == 1) {
                        wstring encrypted = cipher.encrypt(text);
                        cout << "Зашифрованный текст: " << wstringToString(encrypted) << endl;
                    } else {
                        wstring decrypted = cipher.decrypt(text);
                        cout << "Расшифрованный текст: " << wstringToString(decrypted) << endl;
                    }
                } catch (const cipher_error& e) {
                    cout << "Ошибка: " << e.what() << endl;
                }
            }
        } while (op != 0);

    } catch (const cipher_error& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    } catch (const std::exception& e) {
        cerr << "Неизвестная ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
std::wstring stringToWstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

std::string wstringToString(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}
