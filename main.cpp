#include <iostream>
#include <windows.h>

using namespace std;

int main() {
    system("chcp 1251");
    int choice;
    cout << "Change the type of process creating:" << endl;
    cout << "1. WinExec" << endl;
    cout << "2. ShellExecute" << endl;
    cout << "3. CreateProcess" << endl;
    cin >> choice;

    // Создание процесса
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    switch (choice) {
        case 1:
            if (!WinExec("notepad.exe", SW_SHOW)) {
                cout << "Ошибка при создании процесса WinExec" << endl;
                return 1;
            }
            break;
        case 2:
            if (!ShellExecute(NULL, "open", "notepad.exe", NULL, NULL, SW_SHOW)) {
                cout << "Ошибка при создании процесса ShellExecute" << endl;
                return 1;
            }
            break;
        case 3:
            if (!CreateProcess(NULL, "notepad.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                cout << "Ошибка при создании процесса CreateProcess" << endl;
                return 1;
            }
            break;
        default:
            cout << "Неверный выбор" << endl;
            return 1;
    }

    // Получение дескриптора процесса
    DWORD processId;
    cout << "Введите идентификатор процесса:" << endl;
    cin >> processId;
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
    if (processHandle == NULL) {
        cout << "Ошибка при получении дескриптора процесса" << endl;
        return 1;
    }

    // Получение временных характеристик процесса
    FILETIME creationTime, exitTime, kernelTime, userTime;
    if (!GetProcessTimes(processHandle, &creationTime, &exitTime, &kernelTime, &userTime)) {
        cout << "Ошибка при получении временных характеристик процесса" << endl;
        CloseHandle(processHandle);
        return 1;
    }

    // Вывод временных характеристик процесса
    cout << "Время создания процесса:" << endl;
    cout << "  Секунды: " << creationTime.dwLowDateTime / 10000000 << endl;
    cout << "  Миллисекунды: " << creationTime.dwLowDateTime % 10000000 / 1000 << endl;

    cout << "Время завершения процесса:" << endl;
    cout << "  Секунды: " << exitTime.dwLowDateTime / 10000000 << endl;
    cout << "  Миллисекунды: " << exitTime.dwLowDateTime % 10000000 / 1000 << endl;

    cout << "Время работы процесса в ядре:" << endl;
    cout << "  Секунды: " << kernelTime.dwLowDateTime / 10000000 << endl;
    cout << "  Миллисекунды: " << kernelTime.dwLowDateTime % 10000000 / 1000 << endl;

    cout << "Время работы процесса в пользовательском режиме:" << endl;
    cout << "  Секунды: " << userTime.dwLowDateTime / 10000000 << endl;
    cout << "  Миллисекунды: " << userTime.dwLowDateTime % 10000000 / 1000 << endl;

    // Завершение процесса
    if (!TerminateProcess(processHandle, 0)) {
        cout << "Ошибка при завершении процесса" << endl;
        CloseHandle(processHandle);
        return 1;
    }

    // Закрытие дескриптора процесса
    CloseHandle(processHandle);

    return 0;
}
