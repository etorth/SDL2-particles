#include "ParticleExample.h"

void ParticleExample::setStyle(ParticleStyle style)
{
    if (m_style == style)
    {
        return;
    }
    m_style = style;
    if (style == NONE)
    {
        stopSystem();
    }
    if (m_texture == nullptr)
    {
        setTexture(getDefaultTexture());
    }
    switch (style)
    {
    case ParticleExample::FIRE:
    {
        initWithTotalParticles(250);

        // duration
        m_duration = DURATION_INFINITY;

        // Gravity Mode
        this->m_emitterMode = Mode::GRAVITY;

        // Gravity Mode: gravity
        this->modeA.gravity = { 0, 0 };

        // Gravity Mode: radial acceleration
        this->modeA.radialAccel = 0;
        this->modeA.radialAccelVar = 0;

        // Gravity Mode: speed of particles
        this->modeA.speed = -60;
        this->modeA.speedVar = 20;

        // starting angle
        m_angle = 90;
        m_angleVar = 10;

        // life of particles
        m_life = 3;
        m_lifeVar = 0.25f;

        // size, in pixels
        m_startSize = 54.0f;
        m_startSizeVar = 10.0f;
        m_endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per frame
        m_emissionRate = m_totalParticles / m_life;

        // color of particles
        m_startColor.r = 0.76f;
        m_startColor.g = 0.25f;
        m_startColor.b = 0.12f;
        m_startColor.a = 1.0f;
        m_startColorVar.r = 0.0f;
        m_startColorVar.g = 0.0f;
        m_startColorVar.b = 0.0f;
        m_startColorVar.a = 0.0f;
        m_endColor.r = 0.0f;
        m_endColor.g = 0.0f;
        m_endColor.b = 0.0f;
        m_endColor.a = 0.0f;
        m_endColorVar.r = 0.0f;
        m_endColorVar.g = 0.0f;
        m_endColorVar.b = 0.0f;
        m_endColorVar.a = 0.0f;

        m_posVar = { 40.0f, 20.0f };
        break;
    }
    case ParticleExample::FIRE_WORK:
    {
        initWithTotalParticles(1500);

        // duration
        m_duration = DURATION_INFINITY;

        // Gravity Mode
        this->m_emitterMode = Mode::GRAVITY;

        // Gravity Mode: gravity
        this->modeA.gravity = { 0.0f, 90.0f };

        // Gravity Mode:  radial
        this->modeA.radialAccel = 0.0f;
        this->modeA.radialAccelVar = 0.0f;

        //  Gravity Mode: speed of particles
        this->modeA.speed = -180.0f;
        this->modeA.speedVar = 50.0f;

        // angle
        this->m_angle = 90.0f;
        this->m_angleVar = 20.0f;

        // life of particles
        this->m_life = 3.5f;
        this->m_lifeVar = 1.0f;

        // emits per frame
        this->m_emissionRate = m_totalParticles / m_life;

        // color of particles
        m_startColor.r = 0.5f;
        m_startColor.g = 0.5f;
        m_startColor.b = 0.5f;
        m_startColor.a = 1.0f;
        m_startColorVar.r = 0.5f;
        m_startColorVar.g = 0.5f;
        m_startColorVar.b = 0.5f;
        m_startColorVar.a = 0.1f;
        m_endColor.r = 0.1f;
        m_endColor.g = 0.1f;
        m_endColor.b = 0.1f;
        m_endColor.a = 0.2f;
        m_endColorVar.r = 0.1f;
        m_endColorVar.g = 0.1f;
        m_endColorVar.b = 0.1f;
        m_endColorVar.a = 0.2f;

        // size, in pixels
        m_startSize = 8.0f;
        m_startSizeVar = 2.0f;
        m_endSize = START_SIZE_EQUAL_TO_END_SIZE;

        m_posVar = { 0, 0 };
        break;
    }
    case ParticleExample::SUN:
    {
        initWithTotalParticles(350);

        // additive
        //this->setBlendAdditive(true);

        // duration
        m_duration = DURATION_INFINITY;

        // Gravity Mode
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vec2(0, 0));

        // Gravity mode: radial acceleration
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Gravity mode: speed of particles
        setSpeed(-20);
        setSpeedVar(5);

        // angle
        m_angle = 90;
        m_angleVar = 360;

        // life of particles
        m_life = 1;
        m_lifeVar = 0.5f;

        // size, in pixels
        m_startSize = 30.0f;
        m_startSizeVar = 10.0f;
        m_endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per seconds
        m_emissionRate = m_totalParticles / m_life;

        // color of particles
        m_startColor.r = 0.76f;
        m_startColor.g = 0.25f;
        m_startColor.b = 0.12f;
        m_startColor.a = 1.0f;
        m_startColorVar.r = 0.0f;
        m_startColorVar.g = 0.0f;
        m_startColorVar.b = 0.0f;
        m_startColorVar.a = 0.0f;
        m_endColor.r = 0.0f;
        m_endColor.g = 0.0f;
        m_endColor.b = 0.0f;
        m_endColor.a = 1.0f;
        m_endColorVar.r = 0.0f;
        m_endColorVar.g = 0.0f;
        m_endColorVar.b = 0.0f;
        m_endColorVar.a = 0.0f;

        m_posVar = { 0, 0 };
        break;
    }
    case ParticleExample::GALAXY:
    {
        initWithTotalParticles(200);
        // duration
        m_duration = DURATION_INFINITY;

        // Gravity Mode
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vec2(0, 0));

        // Gravity Mode: speed of particles
        setSpeed(-60);
        setSpeedVar(10);

        // Gravity Mode: radial
        setRadialAccel(-80);
        setRadialAccelVar(0);

        // Gravity Mode: tangential
        setTangentialAccel(80);
        setTangentialAccelVar(0);

        // angle
        m_angle = 90;
        m_angleVar = 360;

        // life of particles
        m_life = 4;
        m_lifeVar = 1;

        // size, in pixels
        m_startSize = 37.0f;
        m_startSizeVar = 10.0f;
        m_endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        m_emissionRate = m_totalParticles / m_life;

        // color of particles
        m_startColor.r = 0.12f;
        m_startColor.g = 0.25f;
        m_startColor.b = 0.76f;
        m_startColor.a = 1.0f;
        m_startColorVar.r = 0.0f;
        m_startColorVar.g = 0.0f;
        m_startColorVar.b = 0.0f;
        m_startColorVar.a = 0.0f;
        m_endColor.r = 0.0f;
        m_endColor.g = 0.0f;
        m_endColor.b = 0.0f;
        m_endColor.a = 1.0f;
        m_endColorVar.r = 0.0f;
        m_endColorVar.g = 0.0f;
        m_endColorVar.b = 0.0f;
        m_endColorVar.a = 0.0f;

        m_posVar = { 0, 0 };
        break;
    }
    case ParticleExample::FLOWER:
    {
        initWithTotalParticles(250);

        // duration
        m_duration = DURATION_INFINITY;

        // Gravity Mode
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vec2(0, 0));

        // Gravity Mode: speed of particles
        setSpeed(-80);
        setSpeedVar(10);

        // Gravity Mode: radial
        setRadialAccel(-60);
        setRadialAccelVar(0);

        // Gravity Mode: tangential
        setTangentialAccel(15);
        setTangentialAccelVar(0);

        // angle
        m_angle = 90;
        m_angleVar = 360;

        // life of particles
        m_life = 4;
        m_lifeVar = 1;

        // size, in pixels
        m_startSize = 30.0f;
        m_startSizeVar = 10.0f;
        m_endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        m_emissionRate = m_totalParticles / m_life;

        // color of particles
        m_startColor.r = 0.50f;
        m_startColor.g = 0.50f;
        m_startColor.b = 0.50f;
        m_startColor.a = 1.0f;
        m_startColorVar.r = 0.5f;
        m_startColorVar.g = 0.5f;
        m_startColorVar.b = 0.5f;
        m_startColorVar.a = 0.5f;
        m_endColor.r = 0.0f;
        m_endColor.g = 0.0f;
        m_endColor.b = 0.0f;
        m_endColor.a = 1.0f;
        m_endColorVar.r = 0.0f;
        m_endColorVar.g = 0.0f;
        m_endColorVar.b = 0.0f;
        m_endColorVar.a = 0.0f;

        m_posVar = { 0, 0 };
        break;
    }
    case ParticleExample::METEOR:
    {
        initWithTotalParticles(150);

        // duration
        m_duration = DURATION_INFINITY;

        // Gravity Mode
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vec2(-200, -200));

        // Gravity Mode: speed of particles
        setSpeed(-15);
        setSpeedVar(5);

        // Gravity Mode: radial
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Gravity Mode: tangential
        setTangentialAccel(0);
        setTangentialAccelVar(0);

        // angle
        m_angle = 90;
        m_angleVar = 360;

        // life of particles
        m_life = 2;
        m_lifeVar = 1;

        // size, in pixels
        m_startSize = 60.0f;
        m_startSizeVar = 10.0f;
        m_endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        m_emissionRate = m_totalParticles / m_life;

        // color of particles
        m_startColor.r = 0.2f;
        m_startColor.g = 0.4f;
        m_startColor.b = 0.7f;
        m_startColor.a = 1.0f;
        m_startColorVar.r = 0.0f;
        m_startColorVar.g = 0.0f;
        m_startColorVar.b = 0.2f;
        m_startColorVar.a = 0.1f;
        m_endColor.r = 0.0f;
        m_endColor.g = 0.0f;
        m_endColor.b = 0.0f;
        m_endColor.a = 1.0f;
        m_endColorVar.r = 0.0f;
        m_endColorVar.g = 0.0f;
        m_endColorVar.b = 0.0f;
        m_endColorVar.a = 0.0f;

        m_posVar = { 0, 0 };
        break;
    }
    case ParticleExample::SPIRAL:
    {
        initWithTotalParticles(500);

        // duration
        m_duration = DURATION_INFINITY;

        // Gravity Mode
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vec2(0, 0));

        // Gravity Mode: speed of particles
        setSpeed(-150);
        setSpeedVar(0);

        // Gravity Mode: radial
        setRadialAccel(-380);
        setRadialAccelVar(0);

        // Gravity Mode: tangential
        setTangentialAccel(45);
        setTangentialAccelVar(0);

        // angle
        m_angle = 90;
        m_angleVar = 0;

        // life of particles
        m_life = 12;
        m_lifeVar = 0;

        // size, in pixels
        m_startSize = 20.0f;
        m_startSizeVar = 0.0f;
        m_endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        m_emissionRate = m_totalParticles / m_life;

        // color of particles
        m_startColor.r = 0.5f;
        m_startColor.g = 0.5f;
        m_startColor.b = 0.5f;
        m_startColor.a = 1.0f;
        m_startColorVar.r = 0.5f;
        m_startColorVar.g = 0.5f;
        m_startColorVar.b = 0.5f;
        m_startColorVar.a = 0.0f;
        m_endColor.r = 0.5f;
        m_endColor.g = 0.5f;
        m_endColor.b = 0.5f;
        m_endColor.a = 1.0f;
        m_endColorVar.r = 0.5f;
        m_endColorVar.g = 0.5f;
        m_endColorVar.b = 0.5f;
        m_endColorVar.a = 0.0f;

        m_posVar = { 0, 0 };
        break;
    }
    case ParticleExample::EXPLOSION:
    {
        initWithTotalParticles(700);

        // duration
        m_duration = 0.1f;

        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vec2(0, 0));

        // Gravity Mode: speed of particles
        setSpeed(-70);
        setSpeedVar(40);

        // Gravity Mode: radial
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Gravity Mode: tangential
        setTangentialAccel(0);
        setTangentialAccelVar(0);

        // angle
        m_angle = 90;
        m_angleVar = 360;

        // life of particles
        m_life = 5.0f;
        m_lifeVar = 2;

        // size, in pixels
        m_startSize = 15.0f;
        m_startSizeVar = 10.0f;
        m_endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        m_emissionRate = m_totalParticles / m_duration;

        // color of particles
        m_startColor.r = 0.7f;
        m_startColor.g = 0.1f;
        m_startColor.b = 0.2f;
        m_startColor.a = 1.0f;
        m_startColorVar.r = 0.5f;
        m_startColorVar.g = 0.5f;
        m_startColorVar.b = 0.5f;
        m_startColorVar.a = 0.0f;
        m_endColor.r = 0.5f;
        m_endColor.g = 0.5f;
        m_endColor.b = 0.5f;
        m_endColor.a = 0.0f;
        m_endColorVar.r = 0.5f;
        m_endColorVar.g = 0.5f;
        m_endColorVar.b = 0.5f;
        m_endColorVar.a = 0.0f;

        m_posVar = { 0, 0 };
        break;
    }
    case ParticleExample::SMOKE:
    {
        initWithTotalParticles(200);

        // duration
        m_duration = DURATION_INFINITY;

        // Emitter mode: Gravity Mode
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vec2(0, 0));

        // Gravity Mode: radial acceleration
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Gravity Mode: speed of particles
        setSpeed(-25);
        setSpeedVar(10);

        // angle
        m_angle = 90;
        m_angleVar = 5;

        // life of particles
        m_life = 4;
        m_lifeVar = 1;

        // size, in pixels
        m_startSize = 60.0f;
        m_startSizeVar = 10.0f;
        m_endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per frame
        m_emissionRate = m_totalParticles / m_life;

        // color of particles
        m_startColor.r = 0.8f;
        m_startColor.g = 0.8f;
        m_startColor.b = 0.8f;
        m_startColor.a = 1.0f;
        m_startColorVar.r = 0.02f;
        m_startColorVar.g = 0.02f;
        m_startColorVar.b = 0.02f;
        m_startColorVar.a = 0.0f;
        m_endColor.r = 0.0f;
        m_endColor.g = 0.0f;
        m_endColor.b = 0.0f;
        m_endColor.a = 1.0f;
        m_endColorVar.r = 0.0f;
        m_endColorVar.g = 0.0f;
        m_endColorVar.b = 0.0f;
        m_endColorVar.a = 0.0f;

        m_posVar = { 20.0f, 0.0f };
        break;
    }
    case ParticleExample::SNOW:
    {
        initWithTotalParticles(700);

        // duration
        m_duration = DURATION_INFINITY;

        // set gravity mode.
        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vec2(0, 1));

        // Gravity Mode: speed of particles
        setSpeed(-5);
        setSpeedVar(1);

        // Gravity Mode: radial
        setRadialAccel(0);
        setRadialAccelVar(1);

        // Gravity mode: tangential
        setTangentialAccel(0);
        setTangentialAccelVar(1);

        // angle
        m_angle = -90;
        m_angleVar = 5;

        // life of particles
        m_life = 45;
        m_lifeVar = 15;

        // size, in pixels
        m_startSize = 10.0f;
        m_startSizeVar = 5.0f;
        m_endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        m_emissionRate = 10;

        // color of particles
        m_startColor.r = 1.0f;
        m_startColor.g = 1.0f;
        m_startColor.b = 1.0f;
        m_startColor.a = 1.0f;
        m_startColorVar.r = 0.0f;
        m_startColorVar.g = 0.0f;
        m_startColorVar.b = 0.0f;
        m_startColorVar.a = 0.0f;
        m_endColor.r = 1.0f;
        m_endColor.g = 1.0f;
        m_endColor.b = 1.0f;
        m_endColor.a = 0.0f;
        m_endColorVar.r = 0.0f;
        m_endColorVar.g = 0.0f;
        m_endColorVar.b = 0.0f;
        m_endColorVar.a = 0.0f;

        m_posVar = { 1.0f * m_x, 0.0f };
        break;
    }
    case ParticleExample::RAIN:
    {
        initWithTotalParticles(1000);

        // duration
        m_duration = DURATION_INFINITY;

        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vec2(10, 10));

        // Gravity Mode: radial
        setRadialAccel(0);
        setRadialAccelVar(1);

        // Gravity Mode: tangential
        setTangentialAccel(0);
        setTangentialAccelVar(1);

        // Gravity Mode: speed of particles
        setSpeed(-130);
        setSpeedVar(30);

        // angle
        m_angle = -90;
        m_angleVar = 5;

        // life of particles
        m_life = 4.5f;
        m_lifeVar = 0;

        // size, in pixels
        m_startSize = 4.0f;
        m_startSizeVar = 2.0f;
        m_endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        m_emissionRate = 20;

        // color of particles
        m_startColor.r = 0.7f;
        m_startColor.g = 0.8f;
        m_startColor.b = 1.0f;
        m_startColor.a = 1.0f;
        m_startColorVar.r = 0.0f;
        m_startColorVar.g = 0.0f;
        m_startColorVar.b = 0.0f;
        m_startColorVar.a = 0.0f;
        m_endColor.r = 0.7f;
        m_endColor.g = 0.8f;
        m_endColor.b = 1.0f;
        m_endColor.a = 0.5f;
        m_endColorVar.r = 0.0f;
        m_endColorVar.g = 0.0f;
        m_endColorVar.b = 0.0f;
        m_endColorVar.a = 0.0f;

        m_posVar = { 1.0f * m_x, 0.0f };
        break;
    }
    default:
        break;
    }
}
