#pragma once
#include "gameScene.hpp"

namespace game
{
  /**
   * @brief Scena principale di gioco dove avviene l'azione
   *
   * La scena Gameplay gestisce:
   * - L'arena di gioco con tutti gli attori (giocatore, nemici, oggetti)
   * - Le collision detection e handling
   * - Il cambio di stanza tramite porte
   * - La cache delle texture per performance ottimali
   * - Il rendering di tutti gli elementi di gioco
   */
  class Gameplay final : public GameScene
  {
    // Struttura per contenere le informazioni di una porta
    struct DoorInfo
    {
      std::string destination;
      lulu::Vec2<float> spawn;
      bool changeMusic;
    };

    // Arena di gioco che contiene tutti gli attori (giocatore, nemici, oggetti, porte)
    lulu::Arena arena_;

    // Cache delle texture per evitare di ricaricare le stesse immagini
    // Chiave: percorso del file, Valore: texture caricata
    std::unordered_map<std::string, Texture2D> textureCache_;

    /**
     * @brief Ottiene una texture dalla cache o la carica se non presente
     *
     * Questo sistema di cache migliora le performance evitando di ricaricare
     * le stesse texture multiple volte.
     *
     * @param path Percorso del file texture
     * @return Texture2D caricata
     */
    Texture2D getTexture(const std::string& path);

    /**
     * @brief Cerca il giocatore (Link) tra gli attori dell'arena
     * @return Puntatore a Link, o nullptr se non trovato
     */
    lulu::Link* findLink() const;

    /**
     * @brief Controlla se Link sta collidendo con una porta
     * @param link Puntatore al giocatore
     * @return Informazioni della porta se c'è collisione, nullopt altrimenti
     */
    std::optional<DoorInfo> checkDoorCollision(const lulu::Link* link) const;

    /**
     * @brief Controlla se Link sta collidendo con un NPC
     * @param link Puntatore al giocatore
     * @return Informazioni della porta se c'è collisione, nullopt altrimenti
     */
    std::optional<const lulu::NPC*> checkNpcCollision(const lulu::Link* link) const;

    /**
     * @brief Gestisce il cambio di stanza
     * @param link Puntatore al giocatore
     * @param doorInfo Informazioni della porta (destinazione, spawn, musica)
     */
    void changeRoom(lulu::Link* link, const DoorInfo& doorInfo);

  public:
    /**
     * @brief Costruttore che inizializza l'arena e spawna il giocatore
     *
     * @param game Puntatore al Game principale
     * @param configPath Percorso del file JSON della stanza iniziale (default: hall.json)
     */
    explicit Gameplay(Game* game, const std::string& configPath = "assets/dungeon/configs/hall.json");

    /**
     * @brief Distruttore: libera tutte le texture dalla cache
     */
    ~Gameplay() override;

    /**
     * @brief Aggiorna la logica di gioco per un frame
     *
     * In ordine:
     * 1. Aggiorna la musica
     * 2. Fa un tick dell'arena (movimento, collisioni)
     * 3. Controlla se il giocatore ha toccato una porta
     * 4. Se sì, cambia stanza e sposta il giocatore
     */
    void tick() override;

    /**
     * @brief Renderizza tutto sullo schermo
     *
     * In ordine:
     * 1. Pulisce lo schermo
     * 2. Disegna lo sfondo
     * 3. Disegna i bordi dell'arena (debug)
     * 4. Disegna i bordi di tutti gli attori (debug)
     * 5. Disegna le sprite di tutti gli attori
     */
    void render() override;
  };
} // namespace game
