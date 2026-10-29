#include "Terminal.hpp"

#include <cstdio>
#include <iostream>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#include <csignal>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#endif

namespace terminal {

#ifndef _WIN32
static termios originalSettings;
static bool rawModeEnabled = false;

// Ctrl+C ile çıkılırsa terminal yankısız kalmasın.
static void onInterrupt(int) {
    restore();
    std::_Exit(130);
}
#endif

void init() {
#ifdef _WIN32
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(out, &mode))
        SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    SetConsoleOutputCP(CP_UTF8);  // Türkçe karakterler ve çerçeve çizgileri için
#else
    if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &originalSettings) == 0) {
        termios raw = originalSettings;
        raw.c_lflag &= ~(ICANON | ECHO);  // Enter beklemeden oku, tuşu ekrana yazma
        raw.c_cc[VMIN] = 1;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        rawModeEnabled = true;
        std::signal(SIGINT, onInterrupt);
    }
#endif
    std::cout << "\033[?25l" << std::flush;  // imleci gizle
}

void restore() {
    std::cout << "\033[0m\033[?25h" << std::flush;  // renkleri sıfırla, imleci göster
#ifndef _WIN32
    if (rawModeEnabled) {
        tcsetattr(STDIN_FILENO, TCSANOW, &originalSettings);
        rawModeEnabled = false;
    }
#endif
}

static int readChar() {
#ifdef _WIN32
    return _getch();
#else
    return std::getchar();
#endif
}

Key readKey() {
    int c = readChar();

#ifdef _WIN32
    // Windows'ta ok tuşları iki bayt gelir: 0 veya 224, ardından tuş kodu
    if (c == 0 || c == 224) {
        switch (_getch()) {
            case 72: return Key::Up;
            case 80: return Key::Down;
            case 75: return Key::Left;
            case 77: return Key::Right;
        }
        return Key::Other;
    }
#else
    // Linux/macOS'ta ok tuşları kaçış dizisi olarak gelir: ESC [ A
    if (c == 27) {
        if (readChar() == '[') {
            switch (readChar()) {
                case 'A': return Key::Up;
                case 'B': return Key::Down;
                case 'C': return Key::Right;
                case 'D': return Key::Left;
            }
        }
        return Key::Other;
    }
#endif

    switch (c) {
        case 'w': case 'W': return Key::Up;
        case 's': case 'S': return Key::Down;
        case 'a': case 'A': return Key::Left;
        case 'd': case 'D': return Key::Right;
        case 'u': case 'U': return Key::Undo;
        case 'r': case 'R': return Key::Restart;
        case 'q': case 'Q': return Key::Quit;
        case EOF:           return Key::Quit;
    }
    return Key::Other;
}

void clear() {
    std::cout << "\033[2J\033[H";
}

}  // namespace terminal
