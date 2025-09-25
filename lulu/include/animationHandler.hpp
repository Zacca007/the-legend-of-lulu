#pragma once
#include "types.hpp"
#include <cstdint>
#include <fstream>
#include <map>
#include <vector>

namespace lulu
{

  /**
   * @brief Gestore delle animazioni sprite per attori mobili
   *
   * L'AnimationHandler si occupa di:
   * - Memorizzare sequenze di sprite per diverse combinazioni stato/direzione
   * - Tenere traccia dello stato corrente, direzione e frame dell'animazione
   * - Fornire la sprite corretta da renderizzare in ogni momento
   * - Avanzare automaticamente attraverso i frame delle animazioni
   *
   * Struttura delle animazioni:
   * animationSet_[STATO][DIREZIONE] = [sprite1, sprite2, sprite3, ...]
   *
   * Esempio: animationSet_[S_MOVING][D_UP] = ["link_walk_up_1.png", "link_walk_up_2.png"]
   */
  class AnimationHandler
  {
  private:
    // === STATO CORRENTE DELL'ANIMAZIONE ===

    Direction movementDirection_; // Direzione corrente (determina quale animazione usare)
    std::uint8_t frame_;          // Frame corrente nella sequenza (0, 1, 2, ...)
    State state_;                 // Stato corrente (determina quale set di animazioni usare)

    // === STORAGE DELLE ANIMAZIONI ===

    // Mappa tridimensionale: Stato -> Direzione -> Sequenza di sprite
    // Ogni combinazione stato/direzione ha la sua sequenza di immagini
    std::map<State, std::map<Direction, std::vector<std::string>>> animationSet_;

    // === HELPER INTERNI ===

    /**
     * @brief Legge un intero a 32 bit in formato big-endian da un file
     *
     * Usato per leggere header PNG e determinare dimensioni delle sprite.
     *
     * @param file Stream del file da cui leggere
     * @return Valore a 32 bit letto
     */
    static std::uint32_t readBigEndian(std::ifstream &file);

  public:
    // === FLAG DI CONTROLLO ===

    // Se false, l'AnimationHandler non cambierà mai sprite
    // Utile per attori che hanno solo una sprite statica
    bool enabled_;

    // === COSTRUTTORI ===

    /**
     * @brief Costruttore di default: animazioni disabilitate
     *
     * Crea un handler inattivo con stato S_STILL e direzione D_NONE.
     */
    AnimationHandler();

    /**
     * @brief Costruttore con configurazione iniziale
     *
     * @param enableAnimation Se abilitare il sistema di animazioni
     * @param initialDirection Direzione iniziale
     * @param initialState Stato iniziale
     */
    AnimationHandler(bool enableAnimation, Direction initialDirection, State initialState);

    /**
     * @brief Distruttore di default
     */
    ~AnimationHandler() = default;

    // === GETTERS ===

    /** @brief Restituisce la direzione corrente dell'animazione */
    [[nodiscard]] Direction currentDirection() const;

    /** @brief Restituisce il frame corrente nella sequenza (0-based) */
    [[nodiscard]] std::uint8_t currentFrame() const;

    /** @brief Restituisce lo stato corrente dell'animazione */
    [[nodiscard]] State currentState() const;

    // === CONTROLLO ANIMAZIONI ===

    /**
     * @brief Restituisce la sequenza di sprite per lo stato/direzione correnti
     *
     * @return Riferimento al vector di percorsi sprite
     */
    [[nodiscard]] const std::vector<std::string> &currentAnimation() const;

    /**
     * @brief Aggiunge una nuova sequenza di animazione
     *
     * @param state Stato per cui vale questa animazione (S_MOVING, S_ATTACK, ecc.)
     * @param direction Direzione per cui vale (D_UP, D_LEFT, ecc.)
     * @param animation Sequenza di percorsi delle sprite
     */
    void addAnimation(State state, Direction direction, const std::vector<std::string> &animation);

    /**
     * @brief Cambia stato e direzione, resettando il frame a 0
     *
     * Usato quando l'attore cambia comportamento (da fermo a movimento,
     * o cambia direzione di movimento).
     *
     * @param newState Nuovo stato da impostare
     * @param newDirection Nuova direzione da impostare
     */
    void set(State newState, Direction newDirection);

    /**
     * @brief Restituisce la sprite corrente e avanza al frame successivo
     *
     * Questo è il metodo principale chiamato ogni frame per ottenere
     * la sprite da renderizzare. Automaticamente:
     * 1. Prende la sprite del frame corrente
     * 2. Incrementa il contatore frame
     * 3. Se raggiunge la fine, ricomincia da 0 (loop)
     *
     * @return Percorso della sprite da renderizzare (vuoto se nessuna animazione)
     */
    std::string nextSprite();

    // === METODI DI UTILITÀ ===

    /**
     * @brief Determina le dimensioni di un file PNG
     *
     * Legge l'header del file PNG per estrarre larghezza e altezza.
     * Utile per determinare automaticamente le dimensioni degli attori
     * basandosi sulle loro sprite.
     *
     * @param filepath Percorso del file PNG da analizzare
     * @return Dimensioni del PNG, oppure nullopt se errore
     */
    static std::optional<Vec2<float>> getSpriteDimension(const std::string &filepath);
  };

} // namespace lulu