#pragma once
#include "gameScene.hpp"

namespace game
{
  /**
   * @brief Scena del menu principale del gioco
   *
   * Il menu presenta:
   * - Un'immagine di sfondo
   * - Un testo lampeggiante "Press SPACE to start"
   * - Musica di sottofondo
   * - Transizione al gameplay quando si preme INVIO
   */
  class Menu final : public GameScene
  {
  private:
    // Valore di trasparenza per l'effetto lampeggiante del testo (0-255)
    unsigned char transparency;

    // Flag che indica se la trasparenza sta aumentando (true) o diminuendo (false)
    bool growing;

  public:
    /**
     * @brief Costruttore con parametri di default per il menu
     *
     * @param game Puntatore al Game principale
     * @param background Percorso immagine di sfondo (default: menu.png)
     * @param music Percorso musica di sottofondo (default: menu.mp3)
     * @param inputs Lista tasti da controllare (default: solo INVIO)
     */
    explicit Menu(Game *game, const std::string &background = "assets/menu/menu.png",
                  const std::string &music = "assets/menu/menu.mp3",
                  const std::vector<lulu::Key> &inputs = std::vector{lulu::K_ENTER});

    /**
     * @brief Aggiorna la logica del menu
     *
     * - Aggiorna la musica di sottofondo
     * - Gestisce l'effetto lampeggiante del testo
     * - Controlla se è stato premuto INVIO per iniziare il gioco
     * - Se sì, cambia scena verso il gameplay
     */
    void tick() override;

    /**
     * @brief Renderizza il menu sullo schermo
     *
     * - Disegna lo sfondo
     * - Disegna il testo "Press SPACE to start" con trasparenza variabile
     */
    void render() override;
  };
} // namespace game