#pragma once
#include "actor.hpp"

namespace lulu
{
    /**
     * @brief Classe base per personaggi non giocanti (NPC)
     *
     * Character estende Actor aggiungendo:
     * - Un nome identificativo
     * - Base per dialoghi e interazioni (da espandere)
     *
     * Può essere usata per:
     * - Mercanti che vendono oggetti
     * - PNG che forniscono informazioni o quest
     * - Personaggi decorativi che danno vita al mondo
     * - Boss o nemici speciali con nome
     */
    class Character : public Actor
    {
        // Nome del personaggio (per dialoghi, UI, debug)
        std::string name_;

    public:
        /**
         * @brief Costruttore con parametri espliciti
         *
         * @param pos Posizione del personaggio
         * @param size Dimensioni del rettangolo di collisione
         * @param name Nome identificativo del personaggio
         */
        Character(Vec2<float> pos, Vec2<float> size, const std::string &name)
            : Actor(pos, size), name_(name) {}

        /**
         * @brief Costruttore che carica da file JSON
         *
         * Il file deve contenere una sezione "actor" con sprite, size, ecc.
         * Il nome può essere definito nel JSON o passato come parametro.
         *
         * @param pos Posizione del personaggio
         * @param configPath Percorso del file di configurazione JSON
         */
        explicit Character(Vec2<float> pos, const std::string &configPath)
            : Actor(pos, configPath) {}

        // === GETTERS ===

        /**
         * @brief Restituisce il nome del personaggio
         * @return Nome del personaggio
         */
        [[nodiscard]] const std::string &name() const { return name_; }

        // TODO: Aggiungere metodi per:
        // - Sistema di dialoghi
        // - Interazioni specifiche
        // - Stati del personaggio (friendly, hostile, neutral)
        // - Inventory per mercanti
    };

} // namespace lulu