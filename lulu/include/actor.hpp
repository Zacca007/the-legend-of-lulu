#pragma once
#include "types.hpp"
#include <string>

namespace lulu
{
  class Arena;

  /**
   * @brief Classe base per tutti gli oggetti presenti nell'arena di gioco
   *
   * Un Actor rappresenta qualsiasi entità che ha:
   * - Una posizione e dimensione nello spazio 2D
   * - Una sprite grafica (opzionale)
   * - La capacità di rilevare collisioni con altri attori
   * - La capacità di reagire alle collisioni
   *
   * Esempi di Actor: giocatore, nemici, muri, porte, oggetti, ecc.
   */
  class Actor
  {
  protected:
    Vec2<float> pos_; // Posizione nell'arena (angolo in alto a sinistra)
    Vec2<float> size_{}; // Dimensioni del rettangolo di collisione
    std::string sprite_; // Percorso del file immagine da renderizzare
    Arena* arena_; // Puntatore all'arena che contiene questo attore

  public:
    /**
     * @brief Costruttore per attori semplici con parametri espliciti
     *
     * Usato per oggetti di scena, porte e altri attori con configurazione diretta.
     *
     * @param pos Posizione iniziale
     * @param size Dimensioni del rettangolo di collisione
     * @param sprite Percorso dell'immagine (opzionale)
     */
    Actor(Vec2<float> pos, Vec2<float> size, const std::string& sprite = "");

    /**
     * @brief Costruttore per attori complessi che caricano da JSON
     *
     * Usato per giocatore, nemici e personaggi con configurazione complessa.
     * Il file JSON deve contenere una sezione "actor" con sprite, size, ecc.
     *
     * @param pos Posizione iniziale (sovrascrive quella nel JSON)
     * @param configPath Percorso del file di configurazione JSON
     */
    explicit Actor(Vec2<float> pos, const std::string& configPath);

    /**
     * @brief Distruttore virtuale per ereditarietà corretta
     */
    virtual ~Actor() = default;

    // === GETTERS ===

    /** @brief Restituisce la posizione corrente */
    [[nodiscard]] const Vec2<float>& pos() const;

    /** @brief Restituisce le dimensioni del rettangolo di collisione */
    [[nodiscard]] const Vec2<float>& size() const;

    /** @brief Restituisce il percorso della sprite */
    [[nodiscard]] const std::string& sprite() const;

    /** @brief Restituisce l'arena che contiene questo attore */
    [[nodiscard]] Arena* arena() const;

    // === SETTERS ===

    /**
     * @brief Imposta l'arena di appartenenza
     * @param arena Puntatore alla nuova arena
     */
    void setArena(Arena* arena);

    /**
     * @brief Cambia la posizione dell'attore
     * @param pos Nuova posizione
     */
    void setPos(Vec2<float> pos);

    // === SISTEMA DI COLLISIONI ===

    /**
     * @brief Controlla se questo attore è in collisione con un altro
     *
     * Usa AABB (Axis-Aligned Bounding Box) collision detection.
     * Calcola anche da che direzione avviene la collisione basandosi
     * sulla penetrazione minima.
     *
     * @param other L'altro attore da controllare
     * @return Direzione della collisione (D_NONE se non c'è collisione)
     */
    Direction checkCollision(const Actor* other) const;

    /**
     * @brief Gestisce una collisione rilevata
     *
     * Implementazione di base: sposta l'attore per evitare la sovrapposizione.
     * Le sottoclassi possono sovrascrivere questo metodo per comportamenti specifici.
     *
     * @param collision Informazioni sulla collisione (chi e da dove)
     */
    virtual void handleCollision(Collision collision);
  };
} // namespace lulu
