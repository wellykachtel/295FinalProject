#pragma once

#include <vector>
#include <ge211.h>
#include <memory>

namespace invaders {

    using position = ge211::Basic_position<double>;
    using dimension = ge211::Basic_dimensions<double>;

    enum class Invader_difficulty {
        easy, medium, hard, none
    };

    enum class Direction {
        right, left
    };

    double const player_displacement = 5;
    double const invader_displacement = 5;
    double const player_bullet_displacement = 5;
    double const invader_bullet_displacement = 5;
    dimension const invader_dim{530, 530};
    dimension const bullet_dim{3, 10};
    dimension const player_dim{414, 256};
    dimension const screen_dim{invader_dim.width * 14, invader_dim.height * 10};


    class Model {
    public:

        Model();

        //left or right, by a discrete distance
        void move_player(Direction);

        // actual motion depends on level
        // levels 1-3, they just move down the screen
        // levels 4-6, they move down and side to side
        // levels 7-9, they move down and side to side, but faster
        void move_invaders();

        void move_bullets();

        // lives will be displayed on the screen
        // updated by `invader_bullet_hit_player`
        // and `invader_hit_player`
        int get_lives();

        double get_player_pos();

        // score will be displayed on the screen
        // updated by `player_bullet_hit_invader`
        int get_score();

        // adds a bullet to the player_bullets_ vector
        // at the position where the player is
        void player_shoot_bullet();

        // adds a bullet to the invader_bullets_ vector
        // at the position where the shooting invader is
        void invader_shoot_bullet();


        //checks if something is within the screen
        bool in_boundary(position const & pos_moving_thing, position const & pos_stationary_thing,
                        dimension const & dim_moving_thing, dimension const &  dim_stationary_thing) const;

        bool in_screen(position const & a_thing_pos, dimension const & a_thing_dim);

        //checks if 0 lives
        bool check_player_lose() const;

        //check if 0 invaders, advance to next level
        bool check_player_win();

        ge211::Dimensions get_invader_grid_size() const;

        position get_invader_pos(ge211::Dimensions) const;

        Invader_difficulty get_invader_diff(ge211::Dimensions) const;

        int get_invader_hits(ge211::Dimensions) const;



    private:

        int level_;
        int score_;
        double time_;

        struct player {
            double x_pos;
            int lives;
        };

        player player_;

        struct invader_ {
            invader_(Invader_difficulty difficulty, position pos)
                    : difficulty(difficulty),pos(pos),direction(Direction::right) {
                if (difficulty == Invader_difficulty::hard) {
                    hits_left = 3;
                    active = true;
                } else if (difficulty == Invader_difficulty::medium) {
                    hits_left = 2;
                    active = true;
                } else if (difficulty == Invader_difficulty::easy) {
                    hits_left = 1;
                    active = true;
                } else {
                    hits_left = 0;
                    active = false;
                }
            }
            Direction direction;
            ge211::Basic_position<double> pos;
            bool active;
            int hits_left;
            Invader_difficulty difficulty;

        };

        // goes through all player_bullet_ positions and
        // checks if any have intersected with an invader, updates score
        bool player_bullet_hit_invader(position player_bullet) const;

        // goes through all invader_bullet_ positions and checks if any have
        // intersected with the player, updates lives
        bool invader_bullet_hit_player(position invader_bullet) const;

        // goes through all invaders_ positions and checks if any have
        // intersected with the player, updates lives
        bool invader_hit_player(invader_ const &) const;

        void init_invaders();

        std::vector<std::vector<invader_>> invaders_;
        std::vector<position> player_bullets_;
        std::vector<position> invader_bullets_;

        //three formations: block formation, up-down formation, step formation
        //                    x x x x x x    x   x   x   x       x x x x x
        //                    x x x x x x    x x x x x x x              x x x x x
        //
    };





}