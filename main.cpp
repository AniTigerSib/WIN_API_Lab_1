#include <iostream>
#include <windows.h>

using namespace std;

int main() {
    system("chcp 1251");
    int choice;
    cout << "Change the type of process creating:" << endl;
    cout << "1. WinExec" << endl;
    cout << "2. ShellExecute" << endl;
    cin >> choice;

    // Создание процесса
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    switch (choice) {
        case 1:
            if (!WinExec(R"(C:\Windows\System32\notepad.exe)", SW_SHOW)) {
                cout << "Error creating process WinExec" << endl;
                return 1;
            }
            break;
        case 2:
            if (!ShellExecute(NULL, "open", R"(C:\Windows\System32\notepad.exe)", NULL, NULL, SW_SHOW)) {
                cout << "Error creating process ShellExecute" << endl;
                return 1;
            }
            break;
        default:
            cout << "Incorrect choice" << endl;
            return 1;
    }

    // Получение дескриптора процесса
    DWORD processId;
    cout << "Enter the process ID:" << endl;
    cin >> processId;
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (processHandle == NULL) {
        cout << "Error getting process handle" << endl;
        return 1;
    }
    cout << "Process ID (taken from handle): " << GetProcessId(processHandle) << endl << endl;

    // Завершение процесса
    if (!TerminateProcess(processHandle, 0)) {
        cout << "Error closing process" << endl;
        CloseHandle(processHandle);
        return 1;
    }
    cout << "Process closed" << endl << endl;

    // Получение временных характеристик процесса
    FILETIME creationTime, exitTime, kernelTime, userTime;
    if (!GetProcessTimes(processHandle, &creationTime, &exitTime, &kernelTime, &userTime)) {
        cout << "Error getting process times" << endl;
        CloseHandle(processHandle);
        return 1;
    }

    // Преобразуем FILETIME в системное время
    SYSTEMTIME creationSystemTime, exitSystemTime;
    FileTimeToSystemTime(&creationTime, &creationSystemTime);
    FileTimeToSystemTime(&exitTime, &exitSystemTime);

    // Вывод временных характеристик процесса
    cout << "Process creating time:" << endl;
    cout << creationSystemTime.wYear << "-" << creationSystemTime.wMonth << "-" << creationSystemTime.wDay << " " << creationSystemTime.wHour << ":" << creationSystemTime.wMinute << ":" << creationSystemTime.wSecond << endl;

    cout << "Process closing time:" << endl;
    cout << exitSystemTime.wYear << "-" << exitSystemTime.wMonth << "-" << exitSystemTime.wDay << " " << exitSystemTime.wHour << ":" << exitSystemTime.wMinute << ":" << exitSystemTime.wSecond << endl;

    cout << "Process operating time in kernel mode:" << endl;
    cout << "  Seconds: " << kernelTime.dwLowDateTime / 10000000 << endl;
    cout << "  Milliseconds: " << kernelTime.dwLowDateTime % 10000000 / 1000 << endl;

    cout << "Process operating time in user mode:" << endl;
    cout << "  Seconds: " << userTime.dwLowDateTime / 10000000 << endl;
    cout << "  Milliseconds: " << userTime.dwLowDateTime % 10000000 / 1000 << endl << endl;

    // Закрытие дескриптора процесса
    CloseHandle(processHandle);
    cout << "Handle closed" << endl;

    return 0;
}
