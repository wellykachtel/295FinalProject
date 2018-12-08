#pragma once

#include <vector>
#include <ge211.h>
#include <memory>

namespace invaders {

    enum class Player_direction {
        left, right, neither
    };

    enum class Invader_difficulty {
        easy, medium, hard
    };


    class Model {
    public:

        explicit Model();

        //left or right, by a discrete distance
        void move_player(Player_direction);

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

        int get_player_pos();


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
        bool in_boundary(ge211::Basic_position<double>) const;

        //checks if 0 lives
        bool check_player_lose() const;

        //check if 0 invaders, advance to next level
        bool check_player_win() const;


    private:

        int level_;
        int score_;
        double time_;

        struct player {
            int x_pos;
            int lives;
        };

        player player_;

        struct invader_ {
            invader_(bool active, int hits_left, Invader_difficulty difficulty)
                : active(active),hits_left(hits_left),difficulty(difficulty) {}
            //ge211::Basic_position<double> pos;
            bool active;
            int hits_left;
            Invader_difficulty difficulty;

        };

        using invader_link = std::shared_ptr<invader_>;


        // goes through all player_bullet_ positions and
        // checks if any have intersected with an invader, updates score
        void player_bullet_hit_invader() const;

        // goes through all invader_bullet_ positions and checks if any have
        // intersected with the player, updates lives
        void invader_bullet_hit_player() const;

        // goes through all invaders_ positions and checks if any have
        // intersected with the player, updates lives
        bool invader_hit_player() const;

        void init_invaders();

        std::vector<std::vector<invader_>> invaders_;
        std::vector<ge211::Basic_position<double>> player_bullets_;
        std::vector<ge211::Basic_position<double>> invader_bullets_;

        //three formations: block formation, up-down formation, step formation
        //                    x x x x x x    x   x   x   x       x x x x x
        //                    x x x x x x    x x x x x x x              x x x x x
        //
    };





}