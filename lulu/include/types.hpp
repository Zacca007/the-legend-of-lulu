#pragma once
#include <optional>
#include <cmath>
#include <type_traits>

namespace lulu
{
class Actor;

/**
 * @brief Template di vettore 2D generico
 * 
 * Supporta qualsiasi tipo numerico (int, float, double, ecc.)
 * Fornisce operatori matematici per calcoli vettoriali.
 * Usato per posizioni, dimensioni, velocità, ecc.
 * 
 * @tparam T Tipo numerico (int, float, double, ecc.)
 */
template <typename T>
struct Vec2
{
    // Assicura che T sia un tipo numerico
    static_assert(std::is_arithmetic_v<T>, "Vec2<T> requires a numeric type");

    T x, y;  // Componenti x e y del vettore

    // === OPERATORI TRA VETTORI ===
    
    /** @brief Addizione vettoriale: (x1,y1) + (x2,y2) = (x1+x2, y1+y2) */
    Vec2 operator+(const Vec2 &other) const { return {x + other.x, y + other.y}; }
    
    /** @brief Sottrazione vettoriale: (x1,y1) - (x2,y2) = (x1-x2, y1-y2) */
    Vec2 operator-(const Vec2 &other) const { return {x - other.x, y - other.y}; }
    
    /** @brief Moltiplicazione componente per componente */
    Vec2 operator*(const Vec2 &other) const { return {x * other.x, y * other.y}; }
    
    /** @brief Divisione componente per componente */
    Vec2 operator/(const Vec2 &other) const { return {x / other.x, y / other.y}; }

    // === OPERATORI CON SCALARE ===
    
    /** @brief Addizione con scalare: (x,y) + s = (x+s, y+s) */
    Vec2 operator+(T other) const { return {x + other, y + other}; }
    
    /** @brief Sottrazione con scalare: (x,y) - s = (x-s, y-s) */
    Vec2 operator-(T other) const { return {x - other, y - other}; }
    
    /** @brief Moltiplicazione con scalare: (x,y) * s = (x*s, y*s) */
    Vec2 operator*(T other) const { return {x * other, y * other}; }
    
    /** @brief Divisione con scalare: (x,y) / s = (x/s, y/s) */
    Vec2 operator/(T other) const { return {x / other, y / other}; }

    // === OPERATORI COMPOSTI (MODIFICANO IL VETTORE CORRENTE) ===
    
    Vec2 &operator+=(const Vec2 &other) { *this = *this + other; return *this; }
    Vec2 &operator-=(const Vec2 &other) { *this = *this - other; return *this; }
    Vec2 &operator*=(const Vec2 &other) { *this = *this * other; return *this; }
    Vec2 &operator/=(const Vec2 &other) { *this = *this / other; return *this; }

    Vec2 &operator+=(T other) { *this = *this + other; return *this; }
    Vec2 &operator-=(T other) { *this = *this - other; return *this; }
    Vec2 &operator*=(T other) { *this = *this * other; return *this; }
    Vec2 &operator/=(T other) { *this = *this / other; return *this; }

    // === METODI DI UTILITÀ ===
    
    /**
     * @brief Converte il vettore in un altro tipo numerico
     * @tparam C Tipo di destinazione
     * @return Nuovo Vec2 con tipo C
     * 
     * Esempio: Vec2<int>{10, 20}.convert<float>() → Vec2<float>{10.0f, 20.0f}
     */
    template <typename C> Vec2<C> convert() const { return Vec2<C>{static_cast<C>(x), static_cast<C>(y)}; }

    /**
     * @brief Calcola le componenti diagonali se il vettore è quadrato
     * 
     * Utile per normalizzare il movimento diagonale nei giochi.
     * Se x == y, ritorna un vettore con componenti ridotte di sqrt(2)
     * per mantenere la stessa velocità in tutte le direzioni.
     * 
     * @return Optional con Vec2 diagonale normalizzato, oppure nullopt se x != y
     */
    [[nodiscard]] std::optional<Vec2> diagonal() const
    {
        if (x == y)
        {
            auto l = static_cast<T>(x / std::sqrt(2.0));
            return Vec2{l, l};
        }
        return std::nullopt;
    }
};

// === ENUMERAZIONI PER INPUT E STATI ===

/**
 * @brief Enum per i tasti gestiti dal gioco
 * 
 * I valori corrispondono ai codici tasto di Raylib.
 * Supporta sia WASD che frecce direzionali.
 */
enum Key
{
    K_SPACE = 32,   // Barra spaziatrice
    K_A = 65,       // Tasto A (sinistra)
    K_D = 68,       // Tasto D (destra)
    K_S = 83,       // Tasto S (giù)
    K_W = 87,       // Tasto W (su)
    K_ENTER = 257,  // Invio
    K_RIGHT = 262,  // Freccia destra
    K_LEFT = 263,   // Freccia sinistra
    K_DOWN = 264,   // Freccia giù
    K_UP = 265      // Freccia su
};

/**
 * @brief Stati di animazione per gli attori
 * 
 * Determinano quale set di animazioni deve essere riprodotto.
 */
enum State
{
    S_STILL,    // Fermo (idle)
    S_MOVING,   // In movimento
    S_ATTACK,   // Attaccando
    S_HURT      // Ferito
};

/**
 * @brief Direzioni di movimento e collisioni
 * 
 * Supporta 8 direzioni (cardinali + diagonali) più NONE.
 * Usato sia per movimento che per determinare da che lato avviene una collisione.
 */
enum Direction
{
    D_NONE,      // Nessun movimento
    D_UP,        // Su
    D_DOWN,      // Giù
    D_LEFT,      // Sinistra
    D_RIGHT,     // Destra
    D_UPLEFT,    // Diagonale su-sinistra
    D_UPRIGHT,   // Diagonale su-destra
    D_DOWNLEFT,  // Diagonale giù-sinistra
    D_DOWNRIGHT  // Diagonale giù-destra
};

/**
 * @brief Struttura che rappresenta una collisione
 * 
 * Contiene informazioni su chi ha colpito chi e da che direzione.
 * Usata dall'Arena per gestire le collisioni tra attori.
 */
struct Collision
{
    Actor *target;                    // L'attore con cui si è colliduto
    Direction collisionDirection;     // Da che direzione è avvenuta la collisione
};

} // namespace lulu