#include "model.h"
#include <iostream>
#include <ge211.h>

namespace invaders {

    Model::Model()
        :level_(1), score_(0),
        player_{0, 3}, time_(0),
        frame_count_(0), game_started(false),
        player_hit(-1)
    {

    }

    void Model::move_player(Direction dir) {

        if(dir == Direction::left) {
            player_.x_pos -= player_displacement;
            if (!in_screen(position{player_.x_pos, player_y_pos}, player_dim))
                player_.x_pos = 0;
        } else if(dir == Direction::right) {
            player_.x_pos += player_displacement;
            if(!in_screen(position{player_.x_pos, player_y_pos}, player_dim))
                player_.x_pos = screen_dim.width - player_dim.width;
        }

    }

    void Model::move_invaders() {

        //need to handle row by row since some rows may be "turning around" before others
        // levels 1-3, they just move down the screen after a certain amount of time
        // levels 4-6, they move down and side to side after a certain amount of time
        // levels 7-9, they move down and side to side, but faster
        if(level_ < 4) {
            if (frame_count_ % 1000 != 0) return;
        } else if (level_ < 7) {
            if (frame_count_ % 100 != 0) return;
        } else {
            if (frame_count_ % 50 != 0) return;
        }

        for (auto &invader_row : invaders_) {
            //this will go row by row
            int x = 1;
            size_t row_size = invader_row.size();
            //find the right and left most invaders

            if(row_has_active_invaders(invader_row)) {
                invader_ invader_check = invader_row.back();
                if (invader_row.at(0).direction == Direction::left) {
                    for (size_t i = 0; i < row_size; ++i) {
                        if (invader_row.at(i).active) {
                            invader_check = invader_row.at(i);
                            break;
                        }
                    }
                    x = -1;
                } else {
                    for (size_t i = row_size - 1; i >= 0; --i) {
                        if (invader_row.at(i).active) {
                            invader_check = invader_row.at(i);
                            break;
                        }
                    }
                }

                for (auto &invader : invader_row) {
                    if (level_ > 3) {
                        double test_x = invader_check.pos.x;
                        if (!in_screen(position{test_x + x * invader_displacement,
                                                invader_check.pos.y}, invader_dim)) {
                            invader.direction =
                                    invader.direction == Direction::right ? Direction::left : Direction::right;
                            if (level_ > 6)
                                invader.pos.y += (invader_displacement * 2);
                            else
                                invader.pos.y += invader_displacement;
                        } else
                            invader.pos.x += x * invader_displacement;
                    } else
                        invader.pos.y += invader_displacement;

                    if (invader.active && (invader_hit_player(invader) || (!in_screen(invader.pos, invader_dim)))) {
                        player_.lives = 0;
                        game_started = false;
                    }
                }
            }
        }
    }

    int Model::get_lives() const{
        return player_.lives;
    }

    double Model::get_player_pos() const {
        return player_.x_pos;
    }


    int Model::get_score() {
        return score_;
    }

    void Model::player_shoot_bullet() {
        player_bullets_.emplace_back(player_.x_pos + (player_dim.width / 2) - (bullet_dim.width / 2),
                                     player_y_pos - bullet_dim.height);
    }

    void Model::invader_shoot_bullet() {

        if (frame_count_ % 1000 != 0) return;

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<> dist(0, invaders_.size() - 1);
        size_t index = dist(mt);
        std::uniform_int_distribution<> dist2(0, invaders_.at(index).size() - 1);
        size_t index2 = dist2(mt);
        if(invaders_.at(index).at(index2).active) {
            invader_bullets_.push_back(position{invaders_.at(index).at(index2).pos.x + (invader_dim.width / 2),
                                                invaders_.at(index).at(index2).pos.y + invader_dim.height});
        }
    }

    void Model::init_invaders() {
        double x = invader_dim.width;
        double y = invader_dim.height;
        std::vector<invader_> hard_row;
        std::vector<invader_> medium_row;
        std::vector<invader_> easy_row;
        double starting_x = (screen_dim.width - (invader_dim.width * 6)) / 2;

        switch(level_) {
            case 1:
            case 4:
            case 7: {
                for (size_t i = 0; i < 6; ++i) {
                    hard_row.emplace_back(Invader_difficulty::hard, position{x * i + starting_x, 0});
                    medium_row.emplace_back(Invader_difficulty::medium, position{x * i + starting_x, y * 1});
                    easy_row.emplace_back(Invader_difficulty::easy, position{x * i + starting_x, y * 2});
                }
                invaders_.push_back(hard_row);
                invaders_.push_back(medium_row);
                invaders_.push_back(easy_row);
                break;
            }
            case 2:
            case 5:
            case 8: {

                std::vector<invader_> medium_row2;

                for (size_t i = 0; i < 6; ++i) {
                    if (i % 2) {
                        hard_row.emplace_back(Invader_difficulty::hard, position{x * i + starting_x, 0});
                        easy_row.emplace_back(Invader_difficulty::easy, position{x * i + starting_x, y * 2});
                    } else {
                        hard_row.emplace_back(Invader_difficulty::none, position{x * i + starting_x, y * 0});
                        easy_row.emplace_back(Invader_difficulty::none, position{x * i + starting_x, y * 2});
                    }
                    medium_row.emplace_back(Invader_difficulty::medium, position{x * i + starting_x, y * 1});
                    medium_row2.emplace_back(Invader_difficulty::medium, position{x * i + starting_x, y * 3});
                }
                invaders_.push_back(hard_row);
                invaders_.push_back(medium_row);
                invaders_.push_back(easy_row);
                invaders_.push_back(medium_row2);
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

        auto player_bullet_displacement = player_bullet_speed * time_;
        auto invader_bullet_displacement = invader_bullet_speed1 * time_;
        if(level_ > 6)
            invader_bullet_displacement = invader_bullet_speed3 * time_;
        else if (level_ > 3)
            invader_bullet_displacement = invader_bullet_speed2 * time_;

        for(size_t i = 0; i < player_bullets_.size(); ++i) {
            auto &bullet = player_bullets_.at(i);
            bullet.y -= player_bullet_displacement;
            if(player_bullet_hit_invader(bullet)) {
                score_ += 10;
                if ((score_ % 1000 == 0) && player_.lives < 3)
                    player_.lives ++;

                player_bullets_.erase(player_bullets_.begin() + i);
                if(check_player_win()) {
                    go_to_next_level();
                    break;
                }
            } else if(!in_screen(bullet,bullet_dim))
                player_bullets_.erase(player_bullets_.begin() + i);
        }

        for(size_t i = 0; i < invader_bullets_.size(); ++i) {
            auto &bullet = invader_bullets_.at(i);
            bullet.y += invader_bullet_displacement;
            if(invader_bullet_hit_player(bullet) ) {
                --player_.lives;
                player_hit = time_;
                invader_bullets_.erase(invader_bullets_.begin() + i);
                if(is_game_lose()) {
                    game_started = false;
                    break;
                }
            } else if(!in_screen(bullet, bullet_dim))
                invader_bullets_.erase(invader_bullets_.begin() + i);
        }

    }

    bool Model::player_bullet_hit_invader(position player_bullet) {

        bool result = false;

        for (size_t i = 0; i < invaders_.size(); ++i) {
            for (size_t j = 0; j < invaders_.at(i).size(); ++j) {
                auto &invader = invaders_.at(i).at(j);
                if (invader.active && check_collision_up(invader.pos, player_bullet,invader_dim, bullet_dim)) {
                    --invader.hits_left;
                    if(!invader.hits_left)
                        invader.active = false;
                    result = true;
                }
            }
        }

        return result;
    }

    bool Model::invader_bullet_hit_player(position invader_bullet) const {
        return check_collision_down(invader_bullet, position{player_.x_pos, player_y_pos}, bullet_dim, player_dim);
    }


    bool Model::check_collision_down(position const &pos_top_thing, position const &pos_bottom_thing,
                                dimension const &dim_top_thing, dimension const &dim_bottom_thing) const {
        double top_border = pos_bottom_thing.y;
        double right_border = pos_bottom_thing.x;
        double left_border = pos_bottom_thing.x + dim_bottom_thing.width;

        return pos_top_thing.y + dim_top_thing.height >= top_border
               && pos_top_thing.x > right_border
               && pos_top_thing.x < left_border;
    }

    bool Model::check_collision_up(position const &pos_top_thing, position const &pos_bottom_thing,
                                     dimension const &dim_top_thing, dimension const &dim_bottom_thing) const {
        double bottom_border = pos_top_thing.y + dim_top_thing.height;
        double right_border = pos_top_thing.x;
        double left_border = pos_top_thing.x + dim_top_thing.width;

        return pos_bottom_thing.y <= bottom_border
               && pos_bottom_thing.x > right_border
               && pos_bottom_thing.x + dim_bottom_thing.width < left_border;
    }

    bool Model::in_screen(position const & a_thing_pos,
                          dimension const & a_thing_dim) {
        double right_boundary = screen_dim.width;
        double left_boundary = 0;
        double top_boundary = 0;
        double bottom_boundary = screen_dim.height;
        return a_thing_pos.y >= top_boundary
                && a_thing_pos.y + a_thing_dim.height <= bottom_boundary
                && a_thing_pos.x >= left_boundary
                && a_thing_pos.x + a_thing_dim.width <= right_boundary;
    }

    bool Model::invader_hit_player(invader_ const & an_invader) const {
        if (an_invader.active)
            return check_collision_down(an_invader.pos, position{player_.x_pos, player_y_pos}, invader_dim, player_dim);

        return false;

    }

    bool Model::check_player_win() {
        for(auto invader_row : invaders_)
            if(row_has_active_invaders(invader_row))
                return false;

        return true;
    }

    ge211::Dimensions Model::get_invader_grid_size() const {
        return ge211::Dimensions{(int)invaders_.at(0).size(), (int)invaders_.size()};
    }

    ge211::Position Model::get_invader_pos(ge211::Position pos) const {
        assert(pos.x < invaders_.size() && pos.y < invaders_.at(0).size());
        return invaders_.at(pos.x).at(pos.y).pos.into<int>();
    }

    Invader_difficulty Model::get_invader_diff(ge211::Position pos) const {
        assert(pos.x < invaders_.size() && pos.y < invaders_.at(0).size());
        return invaders_.at(pos.x).at(pos.y).difficulty;
    }

    int Model::get_invader_hits(ge211::Position pos) const {
        assert(pos.x < invaders_.size() && pos.y < invaders_.at(0).size());
        return invaders_.at(pos.x).at(pos.y).hits_left;
    }

    bool Model::get_invader_active(ge211::Position pos) const {
        assert(pos.x < invaders_.size() && pos.y < invaders_.at(0).size());
        return invaders_.at(pos.x).at(pos.y).active;
    }

    void Model::inc_time(double seconds) {
        time_ += seconds;
        ++frame_count_;
    }

    std::vector<position> Model::get_player_bullets() const {
        return player_bullets_;
    }

    std::vector<position> Model::get_invader_bullets() const {
        return invader_bullets_;
    }

    int Model::get_level() const {
        return level_;
    }

    bool Model::is_game_lose() const {
        return get_lives() <= 0;
    }

    void Model::go_to_next_level() {
        if(is_game_lose()) return;
        if(level_ == 9) {
            game_win = true;
            game_started = false;
        }
        ++level_;
        player_bullets_.clear();
        invader_bullets_.clear();
        invaders_.clear();
        init_invaders();
    }

    bool Model::is_game_started() const {
        return game_started;
    }

    void Model::start_game() {
        level_ = 1;
        score_ = 0;
        player_bullets_.clear();
        invader_bullets_.clear();
        invaders_.clear();
        init_invaders();
        player_.lives = 3;
        player_.x_pos = 0;
        game_started = true;
    }

    bool Model::row_has_active_invaders(std::vector<invader_> invader_row) const {
        for(auto invader : invader_row)
            if(invader.active) return true;

        return false;
    }

    bool Model::show_player_hit() const{
        return time_ - player_hit <= 0.5;
    }

    bool Model::is_game_win() const {
        return game_win;
    }

    double Model::get_time() {
        return time_;
    }


}