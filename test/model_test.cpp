#include "model.h"
#include <catch.h>
#include <ge211.h>

using namespace invaders;

position test_invader_pos{150, 60};
position test_invader_bullet{40, screen_dim.height - player_dim.height + 5};
position test_player_pos{35, screen_dim.height - player_dim.height};
position test_player_bullet_hit{165, 110};
position test_player_bullet_no_hit{165, 111};
position test_invader_bullet_no_hit{40, screen_dim.height - player_dim.height - invader_dim.height - 1};
position test_low_invader{40, screen_dim.height - player_dim.height - invader_dim.height};



TEST_CASE("check level dims")
{
    Model game;
    game.start_game();
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
    CHECK(game.check_collision_down(test_invader_bullet, test_player_pos, bullet_dim, player_dim));
    //an invader bullet does not hit an invader
    CHECK(!game.check_collision_down(test_invader_bullet_no_hit, test_player_pos, bullet_dim, player_dim));
    //a player bullet hits an invader on the edge
    CHECK(game.check_collision_up(test_invader_pos, test_player_bullet_hit, invader_dim, bullet_dim));
    //a player bullet does not hit an invader
    CHECK(!game.check_collision_up(test_invader_pos, test_player_bullet_no_hit, invader_dim, bullet_dim));
    //check in_screen
    CHECK(game.in_screen(test_invader_bullet, bullet_dim));
    CHECK(game.in_screen(position{game.get_player_pos(), player_y_pos}, player_dim));

    CHECK(game.check_collision_down(test_low_invader, test_player_pos, invader_dim, player_dim));


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

TEST_CASE("check time level 1")
{
    Model game;
    game.start_game();
    game.set_frame_count(999);
    size_t invader_bullet_count = game.get_invader_bullets().size();
    double y_pos_an_invader = game.get_invader_pos(ge211::Position{2,1}).y;
    game.invader_shoot_bullet();
    game.move_invaders();
    CHECK(game.get_invader_bullets().size() == invader_bullet_count);
    CHECK(game.get_invader_pos(ge211::Position{2,1}).y == y_pos_an_invader);
    game.inc_time(1);
    game.invader_shoot_bullet();
    game.move_invaders();
    CHECK(invader_bullet_count + 1 == game.get_invader_bullets().size());
    CHECK(game.get_invader_pos(ge211::Position{2,1}).y == y_pos_an_invader + invader_displacement);

}

TEST_CASE("check time level 4")
{
    Model game;
    game.go_to_next_level(); //level 2
    game.go_to_next_level(); //level 3
    game.go_to_next_level(); //level 4
    game.set_frame_count(999);
    double x_pos_an_invader = game.get_invader_pos(ge211::Position{2,1}).x;
    game.move_invaders();
    CHECK(game.get_invader_pos(ge211::Position{2,1}).x == x_pos_an_invader);
    game.inc_time(1);
    game.move_invaders();
    CHECK(game.get_invader_pos(ge211::Position{2,1}).x == x_pos_an_invader + invader_displacement);
    CHECK(game.get_invader_pos(ge211::Position{2,1}).x == x_pos_an_invader + invader_displacement);
    double y_pos_an_invader = game.get_invader_pos(ge211::Position{2,1}).y;
    //in order for a row to go down, it needs to be an the edge.
    // for level 4, the right most needs to move 150 more pixels

    int steps_to_right = 150 / (int)invader_displacement;
    for(int i = 0; i < steps_to_right; ++i) {
        game.move_invaders();
    }
    CHECK(game.get_invader_pos(ge211::Position{2,1}).y == y_pos_an_invader + invader_displacement);
}

TEST_CASE("check time level 7")
{
    Model game;
    game.go_to_next_level(); //level 2
    game.go_to_next_level(); //level 3
    game.go_to_next_level(); //level 4
    game.go_to_next_level(); //level 5
    game.go_to_next_level(); //level 6
    game.go_to_next_level(); //level 7
    game.set_frame_count(999);
    double x_pos_an_invader = game.get_invader_pos(ge211::Position{2,1}).x;
    game.move_invaders();
    CHECK(game.get_invader_pos(ge211::Position{2,1}).x == x_pos_an_invader);
    game.inc_time(1);
    game.move_invaders();
    CHECK(game.get_invader_pos(ge211::Position{2,1}).x == x_pos_an_invader + invader_displacement);
    double y_pos_an_invader = game.get_invader_pos(ge211::Position{2,1}).y;
    //in order for a row to go down, it needs to be an the edge.
    // for level 7, the right most needs to move 150 more pixels

    int steps_to_right = 150 / (int)invader_displacement;
    for(int i = 0; i < steps_to_right; ++i) {
        game.move_invaders();
    }
    CHECK(game.get_invader_pos(ge211::Position{2,1}).y == y_pos_an_invader + invader_displacement * 2);

}



