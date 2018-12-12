#include "view.h"
#include <iostream>


namespace invaders {
    View::View(const Model& model)
        : model_(model)
    { }

    void View::draw(ge211::Sprite_set& sprites) const {

        if(! model_.is_game_started()) {
            if(model_.is_game_lose()) {
                sprites.add_sprite(game, ge211::Position{((int)screen_dim.width / 2) - (game.dimensions().width / 2),
                                                         ((int)screen_dim.height / 2 - (game.dimensions().height))});
                sprites.add_sprite(over, ge211::Position{((int)screen_dim.width / 2) - (over.dimensions().width / 2),
                                                         (int)screen_dim.height / 2});
            } else if (model_.is_game_win()) {
                sprites.add_sprite(win, ge211::Position{((int)screen_dim.width / 2) - (win.dimensions().width / 2),
                                                            (int)screen_dim.height / 2});
            } else {
                sprites.add_sprite(welcome, ge211::Position{((int)screen_dim.width / 2) - (welcome.dimensions().width / 2),
                                                            (int)screen_dim.height / 2});
            }

            sprites.add_sprite(start, ge211::Position{((int)screen_dim.width / 2) - (start.dimensions().width / 2),
                                                      (int)screen_dim.height - 100});
        }


        sprites.add_sprite(divider_line, ge211::Position{0, (int)screen_dim.height});
        sprites.add_sprite(lives, ge211::Position{10, (int) screen_dim.height + 20});
        sprites.add_sprite(level, ge211::Position{((int) screen_dim.width / 2) - (level.dimensions().width / 2), (int) screen_dim.height + 20});
        sprites.add_sprite(score, ge211::Position{(int) screen_dim.width - score.dimensions().width - 10, (int) screen_dim.height + 20});
        for(int i = 0; i < model_.get_lives(); ++i) {
            sprites.add_sprite(life_heart, ge211::Position{10 + (i * (life_heart.dimensions().width + 7)), (int) screen_dim.height + 60});
        }

        //given back as rows,cols
        ge211::Dimensions grid_dims = model_.get_invader_grid_size();
        for (int i = 0; i < grid_dims.width; ++i) {
            for (int j = 0; j < grid_dims.height; ++j) {
                Invader_difficulty diff = model_.get_invader_diff(ge211::Position{j,i});
                int hits_left = model_.get_invader_hits(ge211::Position{j,i});
                ge211::Position inv_pos = model_.get_invader_pos(ge211::Position{j,i});
                bool inv_active = model_.get_invader_active(ge211::Position{j,i});

                if (diff == Invader_difficulty::hard && inv_active) {
                    if(hits_left == 3) {
                        sprites.add_sprite(invader_hard_3, inv_pos, 0);
                    } else if(hits_left == 2) {
                        sprites.add_sprite(invader_hard_2, inv_pos, 0);
                    } else {
                        sprites.add_sprite(invader_hard_1, inv_pos, 0);
                    }
                } else if (diff == Invader_difficulty::medium && inv_active) {
                    if(hits_left == 2) {
                        sprites.add_sprite(invader_medium_2, inv_pos, 0);
                    } else {
                        sprites.add_sprite(invader_medium_1, inv_pos, 0);
                    }
                } else if (diff == Invader_difficulty::easy && inv_active) {
                    sprites.add_sprite(invader_easy, inv_pos, 0);
                }
            }
        }

        for(auto bullet : model_.get_player_bullets()) {
            sprites.add_sprite(player_bullet, bullet.into<int>(), 1);
        }

        for(auto bullet : model_.get_invader_bullets()) {
            sprites.add_sprite(invader_bullet, bullet.into<int>(), 1);
        }

        if (model_.show_player_hit()) {
            sprites.add_sprite(player_hit,
                               ge211::Position{(int)model_.get_player_pos(),
                                               screen_dim.into<int>().height - player_dim.into<int>().height}, 1);
        } else {
            sprites.add_sprite(player,
                    ge211::Position{(int)model_.get_player_pos(),
                                    screen_dim.into<int>().height - player_dim.into<int>().height}, 1);
            }


    }


    ge211::Dimensions View::screen_dimensions() const {
        return ge211::Dimensions{(int)screen_dim.width, (int)screen_dim.height + bottom_margin};
    }


}