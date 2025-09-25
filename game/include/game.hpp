#pragma once
#include "lulu.hpp"
#include <memory>

namespace game
{
  class GameScene;

  /**
   * @brief Classe principale del gioco che gestisce il game loop e le scene
   *
   * La classe Game è responsabile di:
   * - Inizializzare e chiudere la finestra del gioco
   * - Gestire il loop principale del gioco
   * - Permettere il cambio di scene durante l'esecuzione
   */
  class Game final
  {
  private:
    // Dimensioni fisse della finestra di gioco (larghezza x altezza)
    const lulu::Vec2<int> SCREEN_SIZE{800, 550};

    // Puntatore alla scena attualmente attiva
    // Usa unique_ptr per gestione automatica della memoria
    std::unique_ptr<GameScene> scene_;

  public:
    /**
     * @brief Costruttore: inizializza la finestra, audio e la scena iniziale (Menu)
     */
    Game();

    /**
     * @brief Distruttore: chiude finestra e sistema audio
     */
    ~Game();

    /**
     * @brief Avvia il game loop principale
     *
     * Continua fino a quando l'utente non chiude la finestra.
     * In ogni frame chiama tick() e render() della scena corrente.
     */
    void run() const;

    /**
     * @brief Cambia la scena attualmente attiva
     *
     * @param newScene Nuova scena da attivare (prende ownership)
     */
    void switchScene(std::unique_ptr<GameScene> &newScene);
  };
} // namespace game