#pragma once

#include "model.h"
#include "view.h"
#include <ge211.h>

namespace invaders {
class Controller : public ge211::Abstract_game
{
public:
    explicit Controller(int c = 3, int r = 3);

protected:
    ge211::Dimensions initial_window_dimensions() const override;
    std::string initial_window_title() const override;
    void on_mouse_down(ge211::Mouse_button, ge211::Position) override;
    void draw(ge211::Sprite_set&) override;
    void on_key(ge211::Key key) override;
    void pass_time(double seconds);

private:
    Model model_;
    View view_;

    int mouse_column_;
    int mouse_row_;
};
}