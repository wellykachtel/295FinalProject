#pragma once

#include "model.h"
#include "../.ipd/lib/ge211/src/ge211_sprites.h"
#include <ge211.h>

namespace invaders {

    ge211::Color const player_bullet_color = ge211::Color::white();
    ge211::Color const invader_bullet_color = ge211::Color::medium_red();
    ge211::Color const player_indicate_color = ge211::Color::medium_blue().lighten(0.3);
    ge211::Color const background = ge211::Color::black();

    class View
    {
    public:
        explicit View(Model const&);
        void draw(ge211::Sprite_set&) const;
        ge211::Dimensions screen_dimensions() const;
        // Text sprites
        ge211::Text_sprite score_sprite;
        ge211::Text_sprite level_sprite;
        ge211::Font sans30{"sans.ttf", 30};
        ge211::Font sans20{"sans.ttf", 20};
        ge211::Font roboto20{"Roboto-Black.ttf", 20};
        ge211::Font roboto30{"Roboto-Black.ttf", 30};
        ge211::Font roboto40{"Roboto-Black.ttf", 40};





    private:
        Model const& model_;

        // The different invaders. different levels will take longer to kill,
        // but show that by different images
        ge211::Image_sprite const invader_hard_3{"invader_hard3.png"};
        ge211::Image_sprite const invader_hard_2{"invader_hard2.png"};
        ge211::Image_sprite const invader_hard_1{"invader_hard1.png"};
        ge211::Image_sprite const invader_medium_2{"invader_medium2.png"};
        ge211::Image_sprite const invader_medium_1{"invader_medium1.png"};
        ge211::Image_sprite const invader_easy{"invader_easy.png"};
        ge211::Rectangle_sprite const invader_none{invader_dim.into<int>(), background};

        // different player sprites
        ge211::Image_sprite const player{"player.png"};
        ge211::Image_sprite const player_hit{"player_hit.png"};

        // pawns with a lighter opacity to show possible moves when hovering
        ge211::Rectangle_sprite const player_bullet{ge211::Dimensions{3,10}, player_bullet_color};
        ge211::Rectangle_sprite const invader_bullet{ge211::Dimensions{3,10}, invader_bullet_color};

        ge211::Rectangle_sprite const divider_line{ge211::Dimensions{(int)screen_dim.width, 5}};

        ge211::Image_sprite const life_heart{"heart.png"};

        ge211::Text_sprite const score{"Score", roboto20};
        ge211::Text_sprite const level{"Level", roboto20};
        ge211::Text_sprite const lives{"Lives", roboto20};
        ge211::Text_sprite const game{"GAME", roboto40};
        ge211::Text_sprite const over{"OVER", roboto40};
        ge211::Text_sprite const win{"YOU WIN!", roboto40};
        ge211::Text_sprite const welcome{"Welcome!", roboto40};
        ge211::Text_sprite const start{"Press space bar to play", roboto20};


    };
}