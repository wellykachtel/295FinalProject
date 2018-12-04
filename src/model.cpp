#include "model.h"
#include <iostream>
#include <ge211.h>

namespace invaders {
    void Model::Model()
        :level_(1), score_(0)
    {
        player_{0, 3, Player_direction::neither};
        init_invaders(1);
    }

    void Model::init_invaders(int level) {
        //case statement

    }

    void Model::move_player(Player_direction) {

    }

    void Model::move_invaders() {

    }

    int Model::get_lives() {
        return player_.lives;
    }

    int Model::get_player_pos() {
        return player_.x_pos;
    }

    Player_direction Model::get_player_direction() {
        return player_.dir;
    }

    int Model::get_score() {
        return score_;
    }

    void Model::player_shoot_bullet() {

    }

    void Model::invader_shoot_bullet() {

    }

    void Model::player_bullet_hit_invader() {

    }

    void Model::invader_bullet_hit_player() {

    }

    bool Model::invader_hit_player() {

    }

    bool Model::in_boundary(ge211::Position) {

    }

    bool Model::check_player_lose() {

    }

    bool Model::check_player_win() {

    }


}