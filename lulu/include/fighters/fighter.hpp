#pragma once
#include "actor.hpp"
#include "movable.hpp"
#include "types.hpp"

namespace lulu
{
  /**
   * @brief Classe base per tutti gli attori che possono combattere
   *
   * Fighter combina le funzionalità di Actor (posizione, collisioni)
   * e Movable (movimento, animazioni) aggiungendo:
   * - Sistema di punti vita (HP)
   * - Sistema di attacco e danno
   * - Meccaniche di combattimento
   *
   * È la classe base per giocatori, nemici e boss.
   */
  class Fighter : public Actor, public Movable
  {
  protected:
    // === STATISTICHE DI COMBATTIMENTO ===

    float hp_;     // Punti vita correnti
    float damage_; // Danno inflitto per attacco

    // === SISTEMA DI ATTACCO (DA IMPLEMENTARE NELLE SOTTOCLASSI) ===

    /**
     * @brief Prepara l'attacco (animazione iniziale, cambio hitbox)
     *
     * Chiamato all'inizio della sequenza di attacco.
     * Qui si può:
     * - Cambiare le dimensioni dell'attore per l'arma
     * - Impostare animazioni di preparazione
     * - Preparare effetti sonori
     */
    virtual void setupAttack() = 0;

    /**
     * @brief Esegue l'attacco vero e proprio (infligge danni)
     *
     * Chiamato nel frame centrale della sequenza di attacco.
     * Qui avviene il calcolo e l'applicazione dei danni.
     */
    virtual void performAttack() = 0;

    /**
     * @brief Conclude l'attacco (ripristina stato normale)
     *
     * Chiamato alla fine della sequenza di attacco.
     * Qui si può:
     * - Ripristinare le dimensioni originali
     * - Tornare allo stato normale
     * - Pulire effetti temporanei
     */
    virtual void endAttack() = 0;

    void recoil(Direction collisionDirection);

    // === COSTRUTTORI PROTETTI (SOLO PER SOTTOCLASSI) ===

    /**
     * @brief Costruttore con parametri espliciti
     *
     * @param position Posizione iniziale
     * @param size Dimensioni del rettangolo di collisione
     * @param speed Velocità di movimento
     * @param hp Punti vita massimi e iniziali
     * @param damage Danno per attacco
     * @param sprite Percorso sprite iniziale (opzionale)
     */
    Fighter(Vec2<float> position, Vec2<float> size, Vec2<float> speed, float hp, float damage,
            const std::string &sprite = "");

    /**
     * @brief Costruttore che carica da file JSON
     *
     * Il file deve contenere sezioni per "actor", "movable" e "fighter"
     * con tutti i parametri necessari.
     *
     * @param pos Posizione iniziale
     * @param configPath Percorso del file di configurazione JSON
     */
    explicit Fighter(Vec2<float> pos, const std::string &configPath);

  public:
    // === GETTERS ===

    /**
     * @brief Controlla se il fighter è ancora vivo
     * @return true se HP > 0
     */
    [[nodiscard]] bool isAlive() const;

    /** @brief Restituisce i punti vita correnti */
    [[nodiscard]] float hp() const;

    /** @brief Restituisce il danno per attacco */
    [[nodiscard]] float damage() const;

    // === SISTEMA DI COMBATTIMENTO ===

    /**
     * @brief Subisce danno e riduce gli HP
     *
     * Non controlla se il fighter muore - questo va gestito
     * dalla logica di gioco esterna.
     *
     * @param damage Quantità di danno da subire
     */
    void takeDamage(float damage);

    /**
     * @brief Attacca un altro fighter
     *
     * Infligge il proprio danno al fighter bersaglio.
     * Versione semplificata - le sottoclassi possono implementare
     * logiche più complesse.
     *
     * @param fighter Bersaglio dell'attacco
     */
    void attack(Fighter* fighter) const;
  };
} // namespace lulu