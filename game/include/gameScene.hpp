#pragma once
#include <raylib.h>
#include "lulu.hpp"
#include <string>
#include <vector>

namespace game
{
class Game;

/**
 * @brief Classe base astratta per tutte le scene del gioco
 * 
 * Una scena rappresenta uno "schermo" del gioco (menu, gameplay, ecc.).
 * Ogni scena gestisce:
 * - Un'immagine di sfondo
 * - Una musica di sottofondo
 * - Gli input da controllare
 * - La logica di aggiornamento (tick)
 * - Il rendering grafico
 */
class GameScene
{
  protected:
    // Texture dello sfondo della scena
    Texture2D background_{};
    
    // Musica di sottofondo della scena
    Music music_{};
    
    // Lista dei tasti che questa scena deve controllare
    std::vector<lulu::Key> inputs_;
    
    // Riferimento al Game principale (per cambio scene)
    Game *game_;

    /**
     * @brief Restituisce solo i tasti attualmente premuti dalla lista inputs_
     * @return Vector con i tasti premuti in questo frame
     */
    [[nodiscard]] std::vector<lulu::Key> activeInputs() const;

    /**
     * @brief Carica lo sfondo da file JSON
     * @param configPath Percorso del file di configurazione JSON
     */
    void setBackground(const std::string &configPath);
    
    /**
     * @brief Carica la musica da file JSON
     * @param configPath Percorso del file di configurazione JSON
     */
    void setMusic(const std::string &configPath);
    
    /**
     * @brief Carica la lista degli input da file JSON
     * @param configPath Percorso del file di configurazione JSON
     */
    void setInputs(const std::string &configPath);

  public:
    /**
     * @brief Costruttore con parametri espliciti
     * 
     * @param game Puntatore al Game principale
     * @param background Percorso dell'immagine di sfondo
     * @param music Percorso del file musicale
     * @param inputs Lista dei tasti da controllare
     */
    GameScene(Game *game, const std::string &background, const std::string &music,
              const std::vector<lulu::Key> &inputs);
    
    /**
     * @brief Costruttore che carica tutto da file JSON
     * 
     * @param game Puntatore al Game principale
     * @param configPath Percorso del file di configurazione JSON
     */
    GameScene(Game *game, const std::string &configPath);

    /**
     * @brief Distruttore virtuale: libera texture e musica
     */
    virtual ~GameScene();

    /**
     * @brief Aggiorna la logica della scena (da implementare nelle sottoclassi)
     * 
     * Chiamata una volta per frame dal game loop.
     * Qui va inserita tutta la logica di gioco specifica della scena.
     */
    virtual void tick() = 0;
    
    /**
     * @brief Renderizza la scena sullo schermo (da implementare nelle sottoclassi)
     * 
     * Chiamata una volta per frame dal game loop, dopo tick().
     * Qui va inserito tutto il codice di rendering specifico della scena.
     */
    virtual void render() = 0;
};
} // namespace game