#include "view.h"
#include <iostream>

namespace hexapawn {
    View::View(const Model& model)
        : model_(model)
    { }

    void View::draw(ge211::Sprite_set& sprites, int selected_col, int selected_row, int mouse_col, int mouse_row) const {

        if(model_.get_winner() == Player::white) {
            sprites.add_sprite(winner_1_title_, {(screen_dimensions().width/2) - (winner_1_title_.dimensions().width/2), 5});
        } else if (model_.get_winner() == Player::black) {
            sprites.add_sprite(winner_2_title_, {(screen_dimensions().width/2) - (winner_2_title_.dimensions().width/2), 5});
        } else {
            sprites.add_sprite(title_, {(screen_dimensions().width / 2) - (title_.dimensions().width / 2), 5});
        }

        if(model_.get_turn() == Player::white) {
            sprites.add_sprite(player_1_turn, {2, (margin - player_1_.dimensions().height)});
        } else {
            sprites.add_sprite(player_2_turn, {2, (screen_dimensions().height - margin)});
        }
        sprites.add_sprite(player_1_, {5, (margin - player_1_.dimensions().height)}, 1);
        sprites.add_sprite(player_2_, {5, (screen_dimensions().height - margin)}, 1);

        for(int row = 0; row < model_.r(); ++row) {
            for(int col = 0; col < model_.c(); ++col) {
                bool col_is_even = col%2 == 0;
                bool row_is_even = row%2 == 0;
                if(col_is_even && row_is_even) {
                    sprites.add_sprite(white_square_, board_to_screen({col, row}), 0);
                    //std::cout << "white square at" << square_dim.width*col << " x " <<square_dim.height*row <<std::endl;
                } else if(!col_is_even && !row_is_even) {
                    sprites.add_sprite(white_square_, board_to_screen({col, row}), 0);
                    //std::cout << "white square at" << square_dim.width*col << " x " <<square_dim.height*row <<std::endl;
                } else {
                    sprites.add_sprite(black_square_, board_to_screen({col, row}), 0);
                    //std::cout << "black square at" << square_dim.width*col << " x " <<square_dim.height*row <<std::endl;
                }

                Player player = model_.get_space(col, row);

                if (selected_row == row && selected_col == col) {
                    if(player == Player::white) {
                        sprites.add_sprite(select_white_token_, board_to_screen({col, row}), 1);
                    } else if (player == Player::black) {
                        sprites.add_sprite(select_black_token_, board_to_screen({col, row}), 1);
                    }
                } else if(player == Player::white) {
                    sprites.add_sprite(white_token_, board_to_screen({col, row}), 1);
                } else if(player == Player::black) {
                    sprites.add_sprite(black_token_, board_to_screen({col, row}), 1);
                }
            }
        }

       if (model_.is_good_spot(selected_col, selected_row, mouse_col, mouse_row)) {
            Player player = model_.get_turn();
            if(player == Player::white) {
                sprites.add_sprite(possible_white_token_, board_to_screen({mouse_col, mouse_row}), 2);
            } else if(player == Player::black) {
                sprites.add_sprite(possible_black_token_, board_to_screen({mouse_col, mouse_row}), 2);
            }
        }
    }


    ge211::Position View::board_to_screen(ge211::Position board_pos) const
    {
        int x = square_dim.width * board_pos.x;
        int y = margin + square_dim.height * (model_.r() - board_pos.y - 1);
        return {x, y};
    }

    ge211::Position View::screen_to_board(ge211::Position screen_pos) const
    {
        int col_no = screen_pos.x / (square_dim.width);
        int row_no = model_.r() - (screen_pos.y - margin) / (square_dim.height) - 1;
        return {col_no, row_no};
    }

    ge211::Dimensions View::screen_dimensions() const {
        return { (square_dim.width * model_.c()),
                margin * 2 + (square_dim.height * model_.r())};
    }

}