#pragma once

// Windows ve Linux/macOS terminalleri arasındaki farkları gizleyen küçük katman.
namespace terminal {

enum class Key { Up, Down, Left, Right, Undo, Restart, Quit, Other };

// Renkleri açar, tuşları Enter beklemeden okunur hale getirir, imleci gizler.
void init();

// Terminali eski haline döndürür.
void restore();

// Bir tuşa basılmasını bekler ve oyun tuşuna çevirir.
Key readKey();

// Ekranı temizleyip imleci sol üste götürür.
void clear();

}  // namespace terminal
