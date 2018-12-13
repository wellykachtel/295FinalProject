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

    double const player_displacement = 7;
    double const invader_displacement = 5;
    dimension const invader_dim{50, 50};
    dimension const bullet_dim{3, 10};
    dimension const player_dim{60, 37};
    dimension const screen_dim{invader_dim.width * 10, invader_dim.height * 10};
    int const bottom_margin{100};
    double const player_y_pos = screen_dim.height - player_dim.height;

    double const player_bullet_speed{.05}; // px/s
    double const invader_bullet_speed1{.01};
    double const invader_bullet_speed2{.03};
    double const invader_bullet_speed3{.05};

    class Model {
    public:

        explicit Model();

        //left or right, by a discrete distance
        void move_player(Direction);

        // actual motion depends on level
        // levels 1-3, they just move down the screen
        // levels 4-6, they move down and side to side
        // levels 7-9, they move down and side to side, but faster
        void move_invaders();

        void move_bullets();

        // adds a bullet to the player_bullets_ vector
        // at the position where the player is
        void player_shoot_bullet();

        // adds a bullet to the invader_bullets_ vector
        // at the position where the shooting invader is
        void invader_shoot_bullet();


        bool check_collision_up(position const & pos_top_thing, position const & pos_bottom_thing,
                            dimension const & dim_top_thing, dimension const & dim_bottom_thing) const;

        bool check_collision_down(position const & pos_top_thing, position const & pos_bottom_thing,
                             dimension const & dim_top_thing, dimension const & dim_bottom_thing) const;

        bool in_screen(position const & a_thing_pos, dimension const & a_thing_dim);


        //check if 0 invaders, advance to next level
        bool check_player_win();

        ge211::Dimensions get_invader_grid_size() const;

        ge211::Position get_invader_pos(ge211::Position) const;

        Invader_difficulty get_invader_diff(ge211::Position) const;

        int get_invader_hits(ge211::Position) const;

        bool get_invader_active(ge211::Position) const;

        void inc_time(double seconds);

        std::vector<position> get_player_bullets() const;

        std::vector<position> get_invader_bullets() const;

        // lives will be displayed on the screen
        // updated by `invader_bullet_hit_player`
        // and `invader_hit_player`
        int get_lives() const;

        int get_level() const;

        double get_player_pos() const;

        // score will be displayed on the screen
        // updated by `player_bullet_hit_invader`
        int get_score();

        bool is_game_lose() const;
        bool is_game_win() const;
        void go_to_next_level();
        bool is_game_started() const;
        void start_game();
        bool show_player_hit() const;
        double get_time();
        void set_frame_count(int);


    private:

        int level_;
        int score_;
        double time_;
        int frame_count_;
        bool game_started;
        double player_hit;
        bool game_win;

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
        bool player_bullet_hit_invader(position player_bullet);

        // goes through all invader_bullet_ positions and checks if any have
        // intersected with the player, updates lives
        bool invader_bullet_hit_player(position invader_bullet) const;

        // goes through all invaders_ positions and checks if any have
        // intersected with the player, updates lives
        bool invader_hit_player(invader_ const &) const;

        void init_invaders();

        bool row_has_active_invaders(std::vector<invader_>) const;

        // bullets are continuous motion, so they need to be in doubles
        std::vector<std::vector<invader_>> invaders_;
        std::vector<position> player_bullets_;
        std::vector<position> invader_bullets_;

        //three formations: block formation, up-down formation, step formation
        //                    x x x x x x      x   x   x         x x x x x
        //                    x x x x x x    x x x x x x               x x x x x
        //                                     x   x   x
    };





}