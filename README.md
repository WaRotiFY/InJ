InJ - DLL Injection Library
Описание

InJ - это библиотека для внедрения DLL в процессы Windows. Проект предоставляет простой и эффективный способ инжектирования собственных DLL в целевые процессы с возможностью вызова экспортируемых функций.
Особенности

    Поддержка различных методов инжектирования

    Возможность вызова функций из инжектированной DLL

    Простой API для использования

    Примеры использования

Структура проекта

    inj.cpp/inj.h - Основная библиотека для инжектирования

    myDllLibrary.cpp - Пример DLL для инжектирования

    example.cpp - Пример использования библиотеки

Использование
Подключение библиотеки
cpp
Copy

#include "inj.h"

Основные функции
cpp
Copy

// Инжектирование DLL в процесс по PID
bool InjectDLL(DWORD pid, const char* dllPath);

// Вызов функции из инжектированной DLL
bool CallInjectedFunction(DWORD pid, const char* functionName);

Пример использования
cpp
Copy

#include "inj.h"

int main() {
    DWORD pid = 1234; // Замените на PID целевого процесса
    const char* dllPath = "myDllLibrary.dll";
    
    if (InjectDLL(pid, dllPath)) {
        std::cout << "DLL успешно инжектирована!" << std::endl;
        
        // Вызов функции из инжектированной DLL
        CallInjectedFunction(pid, "MyExportedFunction");
    } else {
        std::cerr << "Ошибка инжектирования DLL!" << std::endl;
    }
    
    return 0;
}

Сборка

    Скомпилируйте DLL библиотеку (myDllLibrary.cpp)

    Скомпилируйте основную программу, линкуясь с inj.cpp

Требования

    Windows OS

    Компилятор, поддерживающий C++ (MSVC, MinGW и т.д.)
