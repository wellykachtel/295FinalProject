#include "model.h"
#include <catch.h>
#include <ge211.h>

using namespace hexapawn;

TEST_CASE("Make default")
{
    Model hexapawn;
    CHECK(hexapawn.c() == 3);
    CHECK(hexapawn.r() == 3);

}

TEST_CASE("Board size too big or too small")
{
    Model hexapawn(2,10);
    CHECK(hexapawn.c() == 3);
    CHECK(hexapawn.r() == 8);
}

TEST_CASE("Another Board Size")
{
    Model hexapawn(5,4);
    CHECK(hexapawn.c() == 5);
    CHECK(hexapawn.r() == 4);
}

TEST_CASE("Gameplay of a 3x3 Board")
{
    Model hexapawn;
    CHECK(hexapawn.get_turn() == Player::white);
    CHECK(hexapawn.get_winner() == Player::neither);
    CHECK(hexapawn.get_space(0,1) == Player::neither);
    CHECK(hexapawn.get_space(0,2) == Player::white);

    hexapawn.move_pawn(0,0,0,1);
    CHECK(hexapawn.get_space(0,0) == Player::black);
    CHECK(hexapawn.get_space(0,1) == Player::neither);

    hexapawn.move_pawn(0, 2, 0, 1);
    CHECK(hexapawn.get_turn() == Player::black);
    CHECK(hexapawn.get_space(0,1) == Player::white);
    CHECK(hexapawn.get_space(0,2) == Player::neither);

    CHECK(hexapawn.get_space(1,0) == Player::black);
    hexapawn.move_pawn(1, 0, 0, 1);
    CHECK(hexapawn.get_space(1,0) == Player::neither);
    CHECK(hexapawn.get_space(0,1) == Player::black);

    CHECK(hexapawn.get_space(2,2) == Player::white);
    hexapawn.move_pawn(2, 2, 2, 1);
    CHECK(hexapawn.get_space(2,2) == Player::neither);
    CHECK(hexapawn.get_space(2,1) == Player::white);

    hexapawn.move_pawn(0,1,1,2);
    CHECK(hexapawn.get_space(1,2) == Player::black);
    CHECK(hexapawn.get_space(0,1) == Player::neither);
    CHECK(hexapawn.get_winner() == Player::black);

}
TEST_CASE("Gameplay of a 3x3 Board with blocking")
{
    Model hexapawn;
    CHECK(hexapawn.get_turn() == Player::white);
    CHECK(hexapawn.get_winner() == Player::neither);
    CHECK(hexapawn.get_space(0,1) == Player::neither);
    CHECK(hexapawn.get_space(0,2) == Player::white);
    hexapawn.move_pawn(0, 2, 0, 1);
    CHECK(hexapawn.get_turn() == Player::black);
    CHECK(hexapawn.get_space(0,1) == Player::white);
    CHECK(hexapawn.get_space(0,2) == Player::neither);

    CHECK(hexapawn.get_space(1,0) == Player::black);
    hexapawn.move_pawn(1, 0, 1, 1);
    CHECK(hexapawn.get_space(1,0) == Player::neither);
    CHECK(hexapawn.get_space(1,1) == Player::black);

    CHECK(hexapawn.get_space(2,2) == Player::white);
    hexapawn.move_pawn(2, 2, 2, 1);
    CHECK(hexapawn.get_space(2,2) == Player::neither);
    CHECK(hexapawn.get_space(2,1) == Player::white);
    CHECK(hexapawn.get_winner() == Player::white);

}

TEST_CASE("is_good_spot tests")
{
    Model hexapawn(4,3);
    CHECK(!hexapawn.is_good_spot(0,0,0,1));
    CHECK(hexapawn.is_good_spot(0,2,0,1));
    CHECK(hexapawn.is_good_spot(1,2,1,1));
    CHECK(hexapawn.is_good_spot(2,2,2,1));
    CHECK(hexapawn.is_good_spot(3,2,3,1));
    hexapawn.move_pawn(3,2,3,1);
    CHECK(hexapawn.get_turn() == Player::black);
    CHECK(hexapawn.is_good_spot(2,0,3,1));
    CHECK(hexapawn.is_good_spot(2,0,2,1));
    CHECK(!hexapawn.is_good_spot(2,0,1,1));
}