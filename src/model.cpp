#include "model.h"
#include <iostream>
#include <ge211.h>

namespace invaders {

    Model::Model()
        :level_(1), score_(0), player_{0, 3}
    {
        init_invaders();
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


    int Model::get_score() {
        return score_;
    }

    void Model::player_shoot_bullet() {

    }

    void Model::invader_shoot_bullet() {

    }

    void Model::init_invaders() {
        switch(level_) {
            case 1:
            case 4:
            case 7: {
                std::vector<invader_> cols;
                cols.emplace_back(true, 3, Invader_difficulty::hard);
                cols.emplace_back(true, 2, Invader_difficulty::medium);
                cols.emplace_back(true, 1, Invader_difficulty::easy);
                for (size_t i = 0; i < 6; ++i)
                    invaders_.push_back(cols);
                break;
            }
            case 2:
            case 5:
            case 8: {
                std::vector<invader_> cols_type1;
                std::vector<invader_> cols_type2;
                cols_type1.emplace_back(true, 3, Invader_difficulty::hard);
                cols_type1.emplace_back(true, 2, Invader_difficulty::medium);
                cols_type1.emplace_back(true, 1, Invader_difficulty::easy);
                cols_type2.emplace_back(nullptr);
                cols_type2.emplace_back(true, 2, Invader_difficulty::medium);
                cols_type2.emplace_back(nullptr);

                for (size_t i = 0; i < 6; ++i) {
                    if (i % 2) {
                        invaders_.push_back(cols_type2);
                    } else {
                        invaders_.push_back(cols_type1);
                    }
                }
                break;
            }
            case 3:
            case 6:
            case 9: {
                for (size_t i = 0; i < 10; ++i) {
                    for (size_t j = 0; j < 3; ++j) {
                        if(j == 0 && i < 6)
                            invaders_.at(j).emplace_back(true, 3, Invader_difficulty::hard);
                        else if(j == 1 && i < 8 && i > 1)
                            invaders_.at(j).emplace_back(true, 2, Invader_difficulty::medium);
                        else if(j == 2 && i > 3)
                            invaders_.at(j).emplace_back(true, 1, Invader_difficulty::easy);
                        else invaders_.at(i).emplace_back(nullptr);
                    }
                }
                break;
            }
            default:
                invaders_.emplace_back(nullptr);
        }

    }



}