#include "controller.h"
#include <iostream>

using namespace invaders;
using namespace ge211;


namespace invaders {

    Controller::Controller(int c, int r)
        : model_()
        , view_(model_)
        , mouse_pos_{-1, -1}
    {  }

    void Controller::draw(Sprite_set& set) {
        view_.draw(set, mouse_pos_);
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
        } //else if (key == Key::code(" ")) {
            //shoot bullet
        //}
    }

    void Controller::on_frame(double seconds) {

    }
}