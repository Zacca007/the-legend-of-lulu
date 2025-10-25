#pragma once
#include "fighter.hpp"

namespace lulu
{
  /**
   * @brief Il protagonista del gioco - personaggio controllato dal giocatore
   *
   * Link eredita da Fighter e implementa:
   * - Controllo da tastiera (WASD/frecce per movimento, SPAZIO per attacco)
   * - Sistema di animazioni complesso con sprite multiple
   * - Logica di movimento con supporto per 8 direzioni
   * - Sistema di attacco con cambio dimensioni temporaneo
   * - Gestione delle collisioni specifiche del giocatore
   */
  class Link final : public Fighter
  {
    // === SISTEMA DI ATTACCO ===
    bool isHurt_{false};
    bool isAttacking_{false};           // Flag che indica se Link sta attaccando
    std::uint8_t attackFrame_{0};       // Frame corrente dell'animazione di attacco (0-4)
    std::uint8_t hurtFrame_{0};

    static constexpr std::uint8_t ATTACK_DURATION = 5;  // Durata totale attacco in frame
    static constexpr std::uint8_t DAMAGE_FRAME = 2;     // Frame in cui viene inflitto il danno

    static constexpr std::uint8_t INVINCIBILITY_DURATION = 10;

    // === IMPLEMENTAZIONI DEI METODI VIRTUALI DI MOVABLE ===

    /**
     * @brief Determina lo stato di animazione corrente
     *
     * Logica:
     * - Se già in attacco/ferito, mantiene lo stato
     * - Se premuto SPAZIO, passa ad attacco
     * - Se si sta muovendo, passa a movimento
     * - Altrimenti, rimane fermo (idle)
     *
     * @return Stato da impostare per le animazioni
     */
    [[nodiscard]] State updatedState() const override;

    /**
     * @brief Determina la direzione di movimento dai tasti premuti
     *
     * Supporta 8 direzioni:
     * - 4 cardinali: su, giù, sinistra, destra
     * - 4 diagonali: su-sinistra, su-destra, giù-sinistra, giù-destra
     *
     * Gestisce conflitti (es: sinistra+destra = nessun movimento).
     *
     * @return Direzione di movimento desiderata
     */
    [[nodiscard]] Direction updatedDirection() const override;

    /**
     * @brief Calcola il vettore di spostamento per una direzione
     *
     * Converte la direzione astratta in pixel di movimento,
     * normalizzando il movimento diagonale per mantenere
     * velocità costante in tutte le direzioni.
     *
     * @param dir Direzione di movimento
     * @return Vettore di spostamento in pixel
     */
    [[nodiscard]] Vec2<float> calculateMovement(Direction dir) const override;

    // === SISTEMA DI ATTACCO ===

    /**
     * @brief Prepara l'attacco di Link
     *
     * - Inizializza i flag di attacco
     * - Imposta l'animazione corretta
     * - Salva le dimensioni originali
     */
    void setupAttack();

    /**
     * @brief Esegue l'attacco di Link
     *
     * - Aggiorna il frame dell'animazione
     * - Aggiusta le dimensioni in base alla sprite
     * - Infligge danno nel frame corretto
     */
    void performAttack();

    /**
     * @brief Conclude l'attacco di Link
     *
     * - Resetta i flag di attacco
     * - Ripristina le dimensioni originali
     * - Torna allo stato normale
     */
    void endAttack();

    /**
     * @brief Gestisce le collisioni specifiche di Link
     *
     * Durante l'attacco, Link può attraversare gli ostacoli statici
     * ma continua a collidere con entità mobili.
     */
    void handleCollision(Collision collision) override;

    /**
     * @brief Aggiusta la posizione in base al cambio di dimensioni
     *
     * Quando la sprite cambia dimensione (durante attacco),
     * aggiusta la posizione per mantenere l'allineamento corretto
     * in base alla direzione corrente.
     *
     * @param sizeDifference Differenza tra nuova e vecchia dimensione
     */
    void adjustPositionForSize(const Vec2<float>& sizeDifference);

  public:
    /**
     * @brief Costruttore: carica configurazione da JSON
     *
     * Il file link.json contiene:
     * - Statistiche base (HP, danno, velocità, dimensioni)
     * - Set completo di animazioni per tutti gli stati e direzioni:
     *   * Movimento: 8 direzioni con sequenze di sprite
     *   * Idle: 8 direzioni (spesso uguali al movimento)
     *   * Attacco: 4 direzioni cardinali con sequenze
     *
     * @param pos Posizione iniziale di spawn
     * @param configPath Percorso del file JSON (default: assets/link/link.json)
     */
    explicit Link(Vec2<float> pos, const std::string &configPath = "assets/characters/link/link.json");

    /**
     * @brief Implementa il movimento di Link per un frame
     *
     * Sequenza di operazioni:
     * 1. Determina stato e direzione correnti dai tasti premuti
     * 2. Se fermo: imposta animazione idle
     * 3. Se in movimento: calcola spostamento e aggiorna posizione
     * 4. Se in attacco: gestisce l'animazione e il danno
     * 5. Gestisce il cambio di animazioni quando necessario
     * 6. Avanza l'animazione ogni 4 frame per velocità appropriata
     *
     * Override del metodo virtuale puro di Movable.
     */
    void move() override;
  };
} // namespace lulu