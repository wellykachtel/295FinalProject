#include "view.h"
#include <iostream>


namespace invaders {
    View::View(const Model& model)
        : model_(model)
    { }

    void View::draw(ge211::Sprite_set& sprites, ge211::Position mouse_pos) const {

        //given back as rows,cols
        ge211::Dimensions grid_dims = model_.get_invader_grid_size();
        for (int i = 0; i < grid_dims.width; ++i) {
            for (int j = 0; j < grid_dims.height; ++i) {
                Invader_difficulty diff = model_.get_invader_diff(ge211::Dimensions{i,j});
                int hits_left = model_.get_invader_hits(ge211::Dimensions{i,j});
                position inv_pos = model_.get_invader_pos(ge211::Dimensions{i,j});
                if (diff == Invader_difficulty::hard) {
                    if(hits_left == 3) {
                        sprites.add_sprite(invader_hard_3, board_to_screen(ge211::Position{i,j}), 0);
                    }
                }
            }
        }
    }


    ge211::Dimensions View::screen_dimensions() const {
        return {0,0};
    }

    ge211::Position View::board_to_screen(ge211::Position board_pos) const
    {
        int x = invader_dim.width * board_pos.x;
        int y = invader_dim.height * board_pos.y;
        return {x, y};
    }

    ge211::Position View::screen_to_board(ge211::Position screen_pos) const
    {
        int col_no = screen_pos.x / (invader_dim.width);
        int row_no = screen_pos.y / (invader_dim.height);
        return {col_no, row_no};
    }

}