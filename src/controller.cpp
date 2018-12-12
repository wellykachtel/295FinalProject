#include "controller.h"
#include <iostream>

using namespace invaders;
using namespace ge211;


namespace invaders {

    Controller::Controller()
        : model_()
        , view_(model_)
    {  }

    void Controller::draw(Sprite_set& set) {

        if (!model_.is_game_lose() || !model_.is_game_started()) {

            view_.score_sprite.reconfigure(Text_sprite::Builder(view_.roboto20)
                                                   << model_.get_score());
            set.add_sprite(view_.score_sprite, {(int) screen_dim.width - view_.score_sprite.dimensions().width - 10,
                                                (int) screen_dim.height + 50});

            view_.level_sprite.reconfigure(Text_sprite::Builder(view_.roboto20)
                                                   << model_.get_level());
            set.add_sprite(view_.level_sprite,
                           {((int) screen_dim.width / 2) - (view_.level_sprite.dimensions().width / 2),
                            (int) screen_dim.height + 50});
        }

        view_.draw(set);
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
            model_.move_player(Direction::left);
        } else if (key == Key::right()) {
            model_.move_player(Direction::right);
        } else if (key == Key::code(' ')) {
            //shoot bullet
            if (!model_.is_game_started()) {
                model_.start_game();
            } else {
                model_.player_shoot_bullet();
            }
        }
    }

    void Controller::on_frame(double seconds) {
        if(!model_.is_game_started()) return;

        model_.inc_time(seconds);
        model_.move_invaders();
        model_.move_bullets();
        model_.invader_shoot_bullet();

    }
}