#pragma once
#include "ParticleSystem.h"
#include "SDL2/SDL_image.h"

class ParticleExample : public ParticleSystem
{
public:
    ParticleExample() {}
    virtual ~ParticleExample() {}

    enum ParticleStyle
    {
        NONE  = 0,
        BEGIN = 1,
        FIRE  = BEGIN,
        FIRE_WORK,
        SUN,
        GALAXY,
        FLOWER,
        METEOR,
        SPIRAL,
        EXPLOSION,
        SMOKE,
        SNOW,
        RAIN,
        END,
    };

    ParticleStyle style_ = NONE;
    void setStyle(ParticleStyle style);
    SDL_Texture* getDefaultTexture()
    {
        static SDL_Texture* t = IMG_LoadTexture(m_renderer, "fire.png");
        //printf(SDL_GetError());
        return t;
    }

    static const char *styleName(ParticleStyle s)
    {
        switch(s){
            case FIRE       : return "FIRE";
            case FIRE_WORK  : return "FIRE_WORK";
            case SUN        : return "SUN";
            case GALAXY     : return "GALAXY";
            case FLOWER     : return "FLOWER";
            case METEOR     : return "METEOR";
            case SPIRAL     : return "SPIRAL";
            case EXPLOSION  : return "EXPLOSION";
            case SMOKE      : return "SMOKE";
            case SNOW       : return "SNOW";
            case RAIN       : return "RAIN";
            default         : return "???";
        }
    }
};
