#include "view.h"
#include <iostream>

namespace invaders {
    View::View(const Model& model)
        : model_(model)
    { }

    void View::draw(ge211::Sprite_set& sprites) const {

    }


    ge211::Dimensions View::screen_dimensions() const {
        return {0,0};
    }

}