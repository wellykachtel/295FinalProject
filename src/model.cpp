#include "model.h"
#include <iostream>
#include <ge211.h>

namespace invaders {

    Model::Model()
        :level_(1), score_(0), player_{0, 3}
    {
        init_invaders();
    }

    void Model::move_player() {

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
                invaders_.emplace_back(6, invader_(Invader_difficulty::hard));
                invaders_.emplace_back(6, invader_(Invader_difficulty::medium));
                invaders_.emplace_back(6, invader_(Invader_difficulty::easy));
                break;
            }
            case 2:
            case 5:
            case 8: {
                std::vector<invader_> hard_row;
                std::vector<invader_> easy_row;

                for (size_t i = 0; i < 6; ++i) {
                    if (i % 2) {
                        hard_row.emplace_back(Invader_difficulty::hard);
                        easy_row.emplace_back(Invader_difficulty::easy);
                    } else {
                        hard_row.emplace_back(Invader_difficulty::none);
                        easy_row.emplace_back(Invader_difficulty::none);
                    }
                }
                invaders_.push_back(hard_row);
                invaders_.emplace_back(6, invader_(Invader_difficulty::medium));
                invaders_.push_back(easy_row);
                break;
            }
            case 3:
            case 6:
            case 9: {
                std::vector<invader_> helper_cols;
                for (size_t i = 0; i < 3; ++i) {
                    for (size_t j = 0; j < 10; ++j) {
                        if(i == 0 && j < 6)
                            helper_cols.emplace_back(Invader_difficulty::hard);
                        else if(i == 1 && j < 8 && j > 1)
                            helper_cols.emplace_back(Invader_difficulty::medium);
                        else if(i == 2 && j > 3)
                            helper_cols.emplace_back(Invader_difficulty::easy);
                        else helper_cols.emplace_back(Invader_difficulty::none);
                    }
                    invaders_.push_back(helper_cols);
                    helper_cols.clear();
                }
                break;
            }
            default:
                invaders_.emplace_back(invader_(Invader_difficulty::none));
        }

    }



}