#pragma once
#include "actor.hpp"

namespace lulu
{
  /**
   * @brief Porta che collega diverse stanze del gioco
   *
   * Le porte sono attori invisibili (senza sprite) che rilevano
   * quando il giocatore le attraversa e attivano il cambio di stanza.
   *
   * Ogni porta contiene:
   * - Posizione e dimensioni dell'area di attivazione
   * - Stanza di destinazione (file JSON)
   * - Posizione di spawn nella stanza di destinazione
   * - Flag per decidere se cambiare anche la musica
   */
  class Door final : public Actor
  {
    // === DATI DELLA DESTINAZIONE ===

    Vec2<float> spawn_;       // Dove far apparire il giocatore nella stanza di destinazione
    std::string destination_; // Percorso del file JSON della stanza di destinazione
    bool changeMusic_;        // Se cambiare la musica insieme alla stanza

  public:
    /**
     * @brief Costruttore per porta con tutti i parametri
     *
     * @param pos Posizione dell'area di attivazione della porta
     * @param size Dimensioni dell'area di attivazione
     * @param spawn Posizione dove far apparire il giocatore nella destinazione
     * @param destination Percorso del file JSON della stanza di destinazione
     * @param changeMusic Se cambiare anche la musica (true/false)
     */
    Door(Vec2<float> pos, Vec2<float> size, Vec2<float> spawn,
         const std::string &destination, bool changeMusic)
        : Actor(pos, size), // Chiama costruttore Actor (senza sprite)
          spawn_(spawn),
          destination_(destination),
          changeMusic_(changeMusic)
    {
    }

    // === GETTERS ===

    /**
     * @brief Restituisce la posizione di spawn nella stanza di destinazione
     *
     * Usata dal sistema di gioco per posizionare il giocatore
     * quando attraversa la porta.
     *
     * @return Coordinate di spawn
     */
    [[nodiscard]] Vec2<float> spawn() const { return spawn_; }

    /**
     * @brief Restituisce il percorso del file JSON della destinazione
     *
     * Usato per caricare la nuova arena quando si attraversa la porta.
     *
     * @return Percorso del file di configurazione della stanza
     */
    [[nodiscard]] std::string destination() const { return destination_; }

    /**
     * @brief Indica se deve cambiare la musica insieme alla stanza
     *
     * Alcune porte collegano stanze con la stessa atmosfera (stessa musica),
     * altre portano in zone completamente diverse (musica diversa).
     *
     * @return true se deve cambiare musica, false se mantenerla
     */
    [[nodiscard]] bool changeMusic() const { return changeMusic_; }
  };
} // namespace lulu