#include "controller.h"
#include <iostream>

using namespace invaders;
using namespace ge211;


namespace invaders {

    Controller::Controller(int c, int r)
        : model_(c, r)
        , view_(model_)
        , mouse_column_(-1)
        , mouse_row_(-1)
        , selected_column_(-1)
        , selected_row_(-1)
    {  }

    void Controller::draw(Sprite_set& set) {
        view_.draw(set, selected_column_, selected_row_, mouse_column_, mouse_row_);
    }

    Dimensions Controller::initial_window_dimensions() const {
        return view_.screen_dimensions();
    }

    std::string Controller::initial_window_title() const {
        return "Space Invaders";
    }

    void Controller::on_mouse_down(Mouse_button btn, Position screen_posn) {


        //add last line
    }


    void Controller::on_key(Key key)
    {
        if (key == Key::code('q')) {
            quit();
        } else if (key == Key::code('f')) {
            get_window().set_fullscreen(!get_window().get_fullscreen());
        } else if (key == Key::left()) {
            //move player left
        } else if (key == Key::right()) {
            //move player right
        } else if (key == Key::code(" ")) {
            //shoot bullet
        }
    }
}