#pragma once
#include "types.hpp"
#include <memory>
#include <unordered_map>
#include <vector>

namespace lulu
{
  class Actor;

  /**
   * @brief L'arena di gioco che contiene e gestisce tutti gli attori
   *
   * L'Arena è responsabile di:
   * - Contenere tutti gli attori (giocatore, nemici, oggetti, porte, ecc.)
   * - Gestire gli input della tastiera (correnti e precedenti)
   * - Eseguire il movimento di tutti gli attori mobili
   * - Rilevare e gestire tutte le collisioni
   * - Permettere spawn e rimozione di attori durante il gioco
   *
   * Un'arena corrisponde tipicamente a una "stanza" nel gioco.
   */
  class Arena final
  {
  private:
    Vec2<float> pos_{}; // Posizione dell'arena nello spazio di gioco
    Vec2<float> size_{}; // Dimensioni dell'arena

    // Sistema di input: mantiene i tasti del frame precedente e corrente
    // per rilevare quando un tasto viene "appena premuto"
    std::vector<Key> prevInputs_, currInputs_;

    /**
     * Vector di unique_ptr per gestire automaticamente la memoria degli attori.
     *
     * Perché unique_ptr?
     * - Gli attori vengono creati durante il parsing JSON in scope locali
     * - Se fossero in stack, i puntatori diventerebbero invalidi fuori dallo scope
     * - L'allocazione manuale in heap richiederebbe gestione esplicita della memoria
     * - unique_ptr trasferisce ownership in sicurezza e libera automaticamente la memoria
     * - Garantisce che ogni attore abbia sempre un solo proprietario
     */
    std::vector<std::unique_ptr<Actor>> actors_;

    // Mappa delle collisioni: per ogni attore mobile, mantiene la lista
    // di tutti gli attori con cui è in collisione in questo frame
    std::unordered_map<const Actor*, std::vector<Collision>> collisions_;

    // === METODI INTERNI PER LE COLLISIONI ===

    /**
     * @brief Rileva tutte le collisioni per un singolo attore
     *
     * Controlla l'attore dato contro tutti gli altri attori nell'arena
     * e aggiorna la mappa delle collisioni.
     *
     * @param actor L'attore per cui rilevare le collisioni
     */
    void detectCollisionsFor(const Actor* actor);

    /**
     * @brief Gestisce tutte le collisioni rilevate per un attore
     *
     * Chiama handleCollision() per ogni collisione nell'elenco dell'attore.
     *
     * @param actor L'attore per cui gestire le collisioni
     */
    void handleCollisionsFor(Actor* actor) const;

  public:
    /**
     * @brief Costruttore per arena vuota con dimensioni specificate
     *
     * @param pos Posizione dell'arena
     * @param size Dimensioni dell'arena
     */
    Arena(Vec2<float> pos, Vec2<float> size);

    /**
     * @brief Costruttore che carica l'arena da file JSON
     *
     * Il file deve contenere:
     * - Sezione "arena" con pos, size, actors, doors
     * - Lista "actors" con oggetti statici (muri, decorazioni)
     * - Lista "doors" con porte che collegano diverse stanze
     *
     * @param configPath Percorso del file di configurazione JSON
     */
    explicit Arena(const std::string& configPath);

    // === GETTERS ===

    /** @brief Restituisce la posizione dell'arena */
    [[nodiscard]] const Vec2<float>& pos() const;

    /** @brief Restituisce le dimensioni dell'arena */
    [[nodiscard]] const Vec2<float>& size() const;

    /** @brief Restituisce gli input del frame precedente */
    [[nodiscard]] const std::vector<Key>& prevInputs() const;

    /** @brief Restituisce gli input del frame corrente */
    [[nodiscard]] const std::vector<Key>& currInputs() const;

    /** @brief Restituisce tutti gli attori nell'arena (lettura sola) */
    [[nodiscard]] const std::vector<std::unique_ptr<Actor>>& actors() const;

    /** @brief Restituisce la mappa delle collisioni correnti */
    [[nodiscard]] const std::unordered_map<const Actor*, std::vector<Collision>>& collisions() const;

    // === GESTIONE ATTORI ===

    /**
     * @brief Aggiunge un nuovo attore all'arena
     *
     * L'arena prende ownership dell'attore. Se l'attore è Movable,
     * viene anche aggiunto alla mappa delle collisioni.
     *
     * @param actor L'attore da aggiungere (unique_ptr trasferito)
     */
    void spawn(std::unique_ptr<Actor> actor);

    /**
     * @brief Rimuove un attore dall'arena e restituisce ownership
     *
     * Utile per trasferire attori tra arene (cambio stanza).
     * L'attore viene rimosso dalla lista e dalla mappa delle collisioni.
     *
     * @param actor Raw pointer all'attore da rimuovere
     * @return unique_ptr all'attore rimosso (nullptr se non trovato)
     */
    std::unique_ptr<Actor> kill(Actor* actor);

    // === GAME LOOP ===

    /**
     * @brief Aggiorna l'arena per un frame
     *
     * Sequenza di operazioni:
     * 1. Aggiorna gli input (sposta currInputs in prevInputs)
     * 2. Per ogni attore mobile:
     *    - Chiama move() per aggiornare posizione
     *    - Rileva tutte le collisioni
     *    - Gestisce tutte le collisioni
     *
     * @param keys Tasti premuti in questo frame
     */
    void tick(const std::vector<Key>& keys);

    // === METODI DI UTILITÀ ===

    /**
     * @brief Controlla se un tasto specifico è presente in una lista
     *
     * @param keys Lista di tasti da controllare
     * @param key Tasto da cercare
     * @return true se il tasto è presente
     */
    static bool hasKey(const std::vector<Key>& keys, Key key);

    /**
     * @brief Controlla se un tasto è stato appena premuto (non nel frame precedente)
     *
     * Utile per azioni che devono attivarsi solo alla pressione iniziale,
     * non mentre il tasto rimane premuto.
     *
     * @param key Tasto da controllare
     * @return true se premuto ora ma non nel frame precedente
     */
    [[nodiscard]] bool isKeyJustPressed(Key key) const;
  };
} // namespace lulu
