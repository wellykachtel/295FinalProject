#include "model.h"
#include <iostream>
#include <ge211.h>

namespace invaders {

    Model::Model()
        :level_(1), score_(0), player_{0, 3}
    {
        init_invaders();
    }

    void Model::move_player(Direction dir) {

        if(dir == Direction::left
            && in_screen(position{player_.x_pos - 5, 0}, player_dim))
            player_.x_pos -= player_displacement;
        else if(dir == Direction::right
                && in_screen(position{player_.x_pos + 5, 0}, player_dim))
            player_.x_pos += player_displacement;
    }

    void Model::move_invaders() {

        time_;
        //need to handle row by row since some rows may be "turning around" before others
        // levels 1-3, they just move down the screen after a certain amount of time
        // levels 4-6, they move down and side to side after a certain amount of time
        // levels 7-9, they move down and side to side, but faster

        for (auto invader_row : invaders_) {
            //this will go row by row
            int x = 1;
            size_t row_size = invader_row.size();
            auto invader_check = invader_row.back();
            if(invader_row.at(0).direction == Direction::left) {
                x = -1;
                invader_check = invader_row.at(0);
            }

            for (auto invader : invader_row) {
                if(level_ > 3) {
                    if(!in_screen(position{invader_check.pos.x + x * invader_displacement,
                                           invader_check.pos.y}, invader_dim))
                    {
                        invader.direction = invader.direction == Direction::right ? Direction::left : Direction::right;
                        invader.pos.y += invader_displacement;
                    } else {
                        invader.pos.x += x * invader_displacement;
                    }
                } else {
                    invader.pos.y += invader_displacement;
                }

                if(invader_hit_player(invader)) {
                    //game ends
                }
            }
        }
    }

    int Model::get_lives() {
        return player_.lives;
    }

    double Model::get_player_pos() {
        return player_.x_pos;
    }


    int Model::get_score() {
        return score_;
    }

    void Model::player_shoot_bullet() {
        player_bullets_.emplace_back(player_.x_pos, screen_dim.height - player_dim.height);
    }

    void Model::invader_shoot_bullet() {
        time_;

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<> dist(0, invaders_.size() - 1);
        size_t index = dist(mt);
        std::uniform_int_distribution<> dist2(0, invaders_.at(index).size() - 1);
        size_t index2 = dist2(mt);
        invader_bullets_.push_back(invaders_.at(index).at(index2).pos);
    }

    void Model::init_invaders() {
        double x = invader_dim.width;
        double y = invader_dim.height;
        std::vector<invader_> hard_row;
        std::vector<invader_> medium_row;
        std::vector<invader_> easy_row;

        switch(level_) {
            case 1:
            case 4:
            case 7: {
                for (size_t i = 0; i < 6; ++i) {
                    hard_row.emplace_back(Invader_difficulty::hard, position{x * i, 0});
                    medium_row.emplace_back(Invader_difficulty::medium, position{x * i, y * 1});
                    easy_row.emplace_back(Invader_difficulty::easy, position{x * i, y * 2});
                }
                invaders_.push_back(hard_row);
                invaders_.push_back(medium_row);
                invaders_.push_back(easy_row);
                break;
            }
            case 2:
            case 5:
            case 8: {

                for (size_t i = 0; i < 6; ++i) {
                    if (i % 2) {
                        hard_row.emplace_back(Invader_difficulty::hard, position{x * i, 0});
                        easy_row.emplace_back(Invader_difficulty::easy, position{x * i, y * 2});
                    } else {
                        hard_row.emplace_back(Invader_difficulty::none, position{x * i, y * 0});
                        easy_row.emplace_back(Invader_difficulty::none, position{x * i, y * 2});
                    }
                    medium_row.emplace_back(Invader_difficulty::medium, position{x * i, y * 1});
                }
                invaders_.push_back(hard_row);
                invaders_.push_back(medium_row);
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
                            helper_cols.emplace_back(Invader_difficulty::hard, position{x * j, y * i});
                        else if(i == 1 && j < 8 && j > 1)
                            helper_cols.emplace_back(Invader_difficulty::medium, position{x * j, y * i});
                        else if(i == 2 && j > 3)
                            helper_cols.emplace_back(Invader_difficulty::easy, position{x * j, y * i});
                        else helper_cols.emplace_back(Invader_difficulty::none, position{x * j, y * i});
                    }
                    invaders_.push_back(helper_cols);
                    helper_cols.clear();
                }
                break;
            }
            default:
                invaders_.emplace_back(6, invader_(Invader_difficulty::none, position{0,0}));
        }

    }

    void Model::move_bullets() {
        for(size_t i = 0; i < player_bullets_.size(); ++i) {
            auto bullet = player_bullets_.at(i);
            bullet.y -= player_bullet_displacement;
            if(player_bullet_hit_invader(bullet)) {
                score_ += 10;
                player_bullets_.erase(player_bullets_.begin() + i);
            } else if(!in_screen(bullet,bullet_dim)) {
                player_bullets_.erase(player_bullets_.begin() + i);
            }
        }

        for(size_t i = 0; i < invader_bullets_.size(); ++i) {
            auto bullet = invader_bullets_.at(i);
            bullet.y += invader_bullet_displacement;
            if(invader_bullet_hit_player(bullet) ) {
                --player_.lives;
                invader_bullets_.erase(invader_bullets_.begin() + i);
            } else if(!in_screen(bullet, bullet_dim)) {
                invader_bullets_.erase(invader_bullets_.begin() + i);
            }
        }

    }

    bool Model::player_bullet_hit_invader(position player_bullet) const {

        bool result = false;

        for (size_t i = 0; i < invaders_.size(); ++i) {
            for (size_t j = 0; j < invaders_.at(i).size(); ++j) {
                auto invader = invaders_.at(i).at(j);
                if (in_boundary(player_bullet, invader.pos, bullet_dim, invader_dim)) {

                    --invader.hits_left;
                    if(!invader.hits_left) {
                        invader.active = false;
                    }

                    result = true;
                }
            }
        }

        return result;
    }

    bool Model::invader_bullet_hit_player(position invader_bullet) const {

        return in_boundary(invader_bullet, position{player_.x_pos, 0}, bullet_dim, player_dim);
    }


    bool Model::in_boundary(position const &  pos_moving_thing, position const &  pos_stationary_thing,
            dimension const & dim_moving_thing, dimension const & dim_stationary_thing) const {


        double right_boundary = pos_stationary_thing.x + dim_stationary_thing.width;
        double left_boundary = pos_stationary_thing.x;
        double top_boundary = pos_stationary_thing.y;
        double bottom_boundary = pos_stationary_thing.y + dim_stationary_thing.height;

        return pos_moving_thing.x + dim_moving_thing.height > top_boundary && pos_moving_thing.x < bottom_boundary
               && pos_moving_thing.y > left_boundary && pos_moving_thing.y + dim_moving_thing.width < right_boundary;
    }

    bool Model::in_screen(position const & a_thing_pos,
                          dimension const & a_thing_dim) {
        return in_boundary(a_thing_pos, position{0,0}, a_thing_dim, screen_dim);
    }

    bool Model::invader_hit_player(invader_ const & an_invader) const {
        if (an_invader.difficulty != Invader_difficulty::none)
            return in_boundary(an_invader.pos, position{player_.x_pos, screen_dim.height}, invader_dim, player_dim);

        return false;

    }

    bool Model::check_player_win() {
        for(auto invader_row : invaders_) {
            for (auto invader : invader_row) {
                if (invader.active) {
                    return false;
                }
            }
        }

        return true;
    }

    ge211::Dimensions Model::get_invader_grid_size() const {
        return ge211::Dimensions{(int)invaders_.size(), (int)invaders_.at(0).size()};
    }

    position Model::get_invader_pos(ge211::Dimensions dim) const {
        assert(invaders_.size() < dim.width && invaders_.at(0).size() < dim.height);
        return invaders_.at(dim.width).at(dim.height).pos;
    }

    Invader_difficulty Model::get_invader_diff(ge211::Dimensions dim) const {
        assert(invaders_.size() < dim.width && invaders_.at(0).size() < dim.height);
        return invaders_.at(dim.width).at(dim.height).difficulty;
    }

    int Model::get_invader_hits(ge211::Dimensions dim) const {
        assert(invaders_.size() < dim.width && invaders_.at(0).size() < dim.height);
        return invaders_.at(dim.width).at(dim.height).hits_left;
    }

}