#include "model.h"
#include <catch.h>
#include <ge211.h>

using namespace invaders;

position test_invader_pos{150, 60};
position test_invader_bullet{40, screen_dim.height - player_dim.height + 5};
position test_player_pos{35, screen_dim.height - player_dim.height};
position test_player_bullet_hit{165, 60};
position test_player_bullet_no_hit{165, 59};
position test_invader_bullet_no_hit{40, screen_dim.height - player_dim.height - 1};



TEST_CASE("check level dims")
{
    Model game;
    ge211::Dimensions grid = game.get_invader_grid_size();
    CHECK(grid.height == 3);
    CHECK(grid.width == 6);
    CHECK(game.get_level() == 1);
    game.go_to_next_level();
    grid = game.get_invader_grid_size();
    CHECK(grid.height == 4);
    CHECK(grid.width == 6);
    CHECK(game.get_level() == 2);
    game.go_to_next_level();
    grid = game.get_invader_grid_size();
    CHECK(grid.height == 3);
    CHECK(grid.width == 10);
    CHECK(game.get_level() == 3);
}

TEST_CASE("collision tests")
{
    Model game;
    //an invader bullet hits the player
    CHECK(game.in_boundary(test_invader_bullet, test_player_pos, bullet_dim, player_dim));
    //an invader bullet does not hit an invader
    CHECK(!game.in_boundary(test_invader_bullet_no_hit, test_player_pos, bullet_dim, player_dim));
    //a player bullet hits an invader on the edge
    CHECK(game.in_boundary(test_player_bullet_hit, test_invader_pos, bullet_dim, invader_dim));
    //a player bullet does not hit an invader
    CHECK(!game.in_boundary(test_player_bullet_no_hit, test_invader_pos, bullet_dim, invader_dim));
    //check in_screen
    CHECK(game.in_screen(test_invader_bullet, bullet_dim));
    CHECK(game.in_screen(position{game.get_player_pos(), player_y_pos}, player_dim));


}

TEST_CASE("player in screen tests")
{
    Model game;
    //check when a player tries to move off screen, it doesn't
    double start_pos = game.get_player_pos();
    game.move_player(Direction::left);
    CHECK(game.in_screen(position{game.get_player_pos(), player_y_pos}, player_dim));
    CHECK(start_pos == game.get_player_pos());

    int right_clicks = (int)screen_dim.width / (int)player_displacement;
    for(int i = 0; i < right_clicks; ++i) {
        game.move_player(Direction::right);
    }

    game.move_player(Direction::right);
    CHECK(game.in_screen(position{game.get_player_pos(), player_y_pos}, player_dim));
}

