#pragma once
#include "fighter.hpp"

namespace lulu {

    class Zol final : public Fighter {
        // === SISTEMA DI MOVIMENTO ===
        std::uint8_t movementCounter_{0};
        std::uint8_t directionCounter_{0};
        std::uint8_t animationCounter_{0};
        Direction currentDirection_{D_DOWN};

        [[nodiscard]] State updatedState() const override;
        [[nodiscard]] Direction updatedDirection() const override;
        [[nodiscard]] Vec2<float> calculateMovement(Direction dir) const override;

    public:
        explicit Zol(Vec2<float> pos, const std::string &config = "assets/characters/zol/zol.json");

        void move() override;
    };

} // lulu