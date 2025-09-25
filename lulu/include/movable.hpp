#pragma once
#include "animationHandler.hpp"
#include "types.hpp"

namespace lulu
{

  /**
   * @brief Classe base per tutti gli oggetti che possono muoversi
   *
   * Movable aggiunge alle funzionalità di Actor:
   * - Velocità di movimento
   * - Sistema di animazioni per rappresentare movimento, stati, direzioni
   * - Logica astratta per il movimento (da implementare nelle sottoclassi)
   *
   * È una classe mixin che viene ereditata insieme ad Actor per creare
   * attori che possono muoversi (es: Fighter eredita da Actor + Movable).
   */
  class Movable
  {
  protected:
    // Velocità di movimento in pixel per frame (x = orizzontale, y = verticale)
    Vec2<float> speed_{};

    // Gestore delle animazioni per sprite dinamiche
    // Si occupa di cambiare le sprite in base a stato, direzione e frame
    AnimationHandler movement_;

    /**
     * @brief Costruttore per oggetti mobili con velocità specifica
     *
     * @param speed Velocità di movimento
     * @param enableAnimation Se abilitare il sistema di animazioni
     */
    explicit Movable(Vec2<float> speed, bool enableAnimation = false);

    /**
     * @brief Costruttore che carica velocità e animazioni da file JSON
     *
     * Il file deve contenere una sezione "movable" con:
     * - "speed": {x, y} velocità di movimento
     * - "enableAnimation": booleano per abilitare animazioni
     *
     * @param configPath Percorso del file di configurazione JSON
     */
    explicit Movable(const std::string &configPath);

    // === METODI VIRTUALI PURI (DA IMPLEMENTARE NELLE SOTTOCLASSI) ===

    /**
     * @brief Determina lo stato corrente dell'attore
     *
     * Analizza le condizioni correnti (input, collisioni, stato interno)
     * per decidere quale stato di animazione usare (fermo, movimento, attacco, ecc.)
     *
     * @return Nuovo stato da impostare
     */
    [[nodiscard]] virtual State updatedState() const = 0;

    /**
     * @brief Determina la direzione corrente del movimento
     *
     * Analizza gli input della tastiera per decidere in che direzione
     * l'attore dovrebbe muoversi o guardare.
     *
     * @return Nuova direzione da impostare
     */
    [[nodiscard]] virtual Direction updatedDirection() const = 0;

    /**
     * @brief Calcola il vettore di movimento per una direzione
     *
     * Converte una direzione astratta (D_UP, D_LEFT, ecc.) in un vettore
     * di movimento concreto in pixel, considerando la velocità dell'attore.
     *
     * @param dir Direzione di movimento desiderata
     * @return Vettore di spostamento in pixel
     */
    [[nodiscard]] virtual Vec2<float> calculateMovement(Direction dir) const = 0;

  public:
    /**
     * @brief Distruttore virtuale per ereditarietà corretta
     */
    virtual ~Movable() = default;

    // === GETTERS ===

    /** @brief Restituisce la velocità di movimento corrente */
    [[nodiscard]] const Vec2<float> &speed() const;

    // === METODO PRINCIPALE ===

    /**
     * @brief Esegue il movimento per un frame (da implementare nelle sottoclassi)
     *
     * Questo metodo deve:
     * 1. Determinare stato e direzione correnti
     * 2. Calcolare il movimento necessario
     * 3. Aggiornare la posizione dell'attore
     * 4. Gestire le animazioni (cambio sprite)
     *
     * Viene chiamato dall'Arena durante il tick() per tutti gli attori mobili.
     */
    virtual void move() = 0;
  };

} // namespace lulu