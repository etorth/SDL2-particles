#include "ParticleSystem.h"
#include <algorithm>
#include <assert.h>
#include <string>

inline float Deg2Rad(float a)
{
    return a * 0.01745329252f;
}

inline float Rad2Deg(float a)
{
    return a * 57.29577951f;
}

inline float clampf(float value, float min_inclusive, float max_inclusive)
{
    if (min_inclusive > max_inclusive)
    {
        std::swap(min_inclusive, max_inclusive);
    }
    return value < min_inclusive ? min_inclusive : value < max_inclusive ? value : max_inclusive;
}

inline void normalize_point(float x, float y, Pointf* out)
{
    float n = x * x + y * y;
    // Already normalized.
    if (n == 1.0f)
    {
        return;
    }

    n = sqrt(n);
    // Too close to zero.
    if (n < 1e-5)
    {
        return;
    }

    n = 1.0f / n;
    out->x = x * n;
    out->y = y * n;
}

/**
A more effect random number getter function, get from ejoy2d.
*/
inline static float RANDOM_M11(unsigned int* seed)
{
    *seed = *seed * 134775813 + 1;
    union
    {
        uint32_t d;
        float f;
    } u;
    u.d = (((uint32_t)(*seed) & 0x7fff) << 8) | 0x40000000;
    return u.f - 3.0f;
}

ParticleSystem::ParticleSystem()
{
}

// implementation ParticleSystem

bool ParticleSystem::initWithTotalParticles(int numberOfParticles)
{
    m_totalParticles = numberOfParticles;
    m_isActive = true;
    m_emitterMode = Mode::GRAVITY;
    m_isAutoRemoveOnFinish = false;
    m_transformSystemDirty = false;

    resetTotalParticles(numberOfParticles);

    return true;
}

void ParticleSystem::resetTotalParticles(int numberOfParticles)
{
    if (m_particleData.size() < numberOfParticles)
    {
        m_particleData.resize(numberOfParticles);
    }
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::addParticles(int count)
{
    if (m_paused)
    {
        return;
    }
    uint32_t RANDSEED = rand();

    int start = m_particleCount;
    m_particleCount += count;

    //life
    for (int i = start; i < m_particleCount; ++i)
    {
        float theLife = m_life + m_lifeVar * RANDOM_M11(&RANDSEED);
        m_particleData[i].timeToLive = (std::max)(0.0f, theLife);
    }

    //position
    for (int i = start; i < m_particleCount; ++i)
    {
        m_particleData[i].posx = m_sourcePosition.x + m_posVar.x * RANDOM_M11(&RANDSEED);
    }

    for (int i = start; i < m_particleCount; ++i)
    {
        m_particleData[i].posy = m_sourcePosition.y + m_posVar.y * RANDOM_M11(&RANDSEED);
    }

    //color
#define SET_COLOR(c, b, v)                                                 \
    for (int i = start; i < m_particleCount; ++i)                           \
    {                                                                      \
        m_particleData[i].c = clampf(b + v * RANDOM_M11(&RANDSEED), 0, 1); \
    }

    SET_COLOR(colorR, m_startColor.r, m_startColorVar.r);
    SET_COLOR(colorG, m_startColor.g, m_startColorVar.g);
    SET_COLOR(colorB, m_startColor.b, m_startColorVar.b);
    SET_COLOR(colorA, m_startColor.a, m_startColorVar.a);

    SET_COLOR(deltaColorR, m_endColor.r, m_endColorVar.r);
    SET_COLOR(deltaColorG, m_endColor.g, m_endColorVar.g);
    SET_COLOR(deltaColorB, m_endColor.b, m_endColorVar.b);
    SET_COLOR(deltaColorA, m_endColor.a, m_endColorVar.a);

#define SET_DELTA_COLOR(c, dc)                                                                              \
    for (int i = start; i < m_particleCount; ++i)                                                            \
    {                                                                                                       \
        m_particleData[i].dc = (m_particleData[i].dc - m_particleData[i].c) / m_particleData[i].timeToLive; \
    }

    SET_DELTA_COLOR(colorR, deltaColorR);
    SET_DELTA_COLOR(colorG, deltaColorG);
    SET_DELTA_COLOR(colorB, deltaColorB);
    SET_DELTA_COLOR(colorA, deltaColorA);

    //size
    for (int i = start; i < m_particleCount; ++i)
    {
        m_particleData[i].size = m_startSize + m_startSizeVar * RANDOM_M11(&RANDSEED);
        m_particleData[i].size = (std::max)(0.0f, m_particleData[i].size);
    }

    if (m_endSize != START_SIZE_EQUAL_TO_END_SIZE)
    {
        for (int i = start; i < m_particleCount; ++i)
        {
            float endSize = m_endSize + m_endSizeVar * RANDOM_M11(&RANDSEED);
            endSize = (std::max)(0.0f, endSize);
            m_particleData[i].deltaSize = (endSize - m_particleData[i].size) / m_particleData[i].timeToLive;
        }
    }
    else
    {
        for (int i = start; i < m_particleCount; ++i)
        {
            m_particleData[i].deltaSize = 0.0f;
        }
    }

    // rotation
    for (int i = start; i < m_particleCount; ++i)
    {
        m_particleData[i].rotation = m_startSpin + m_startSpinVar * RANDOM_M11(&RANDSEED);
    }
    for (int i = start; i < m_particleCount; ++i)
    {
        float endA = m_endSpin + m_endSpinVar * RANDOM_M11(&RANDSEED);
        m_particleData[i].deltaRotation = (endA - m_particleData[i].rotation) / m_particleData[i].timeToLive;
    }

    // position
    Vec2 pos;
    pos.x = m_x;
    pos.y = m_y;

    for (int i = start; i < m_particleCount; ++i)
    {
        m_particleData[i].startPosX = pos.x;
    }
    for (int i = start; i < m_particleCount; ++i)
    {
        m_particleData[i].startPosY = pos.y;
    }

    // Mode Gravity: A
    if (m_emitterMode == Mode::GRAVITY)
    {

        // radial accel
        for (int i = start; i < m_particleCount; ++i)
        {
            m_particleData[i].modeA.radialAccel = modeA.radialAccel + modeA.radialAccelVar * RANDOM_M11(&RANDSEED);
        }

        // tangential accel
        for (int i = start; i < m_particleCount; ++i)
        {
            m_particleData[i].modeA.tangentialAccel = modeA.tangentialAccel + modeA.tangentialAccelVar * RANDOM_M11(&RANDSEED);
        }

        // rotation is dir
        if (modeA.rotationIsDir)
        {
            for (int i = start; i < m_particleCount; ++i)
            {
                float a = Deg2Rad(m_angle + m_angleVar * RANDOM_M11(&RANDSEED));
                Vec2 v(cosf(a), sinf(a));
                float s = modeA.speed + modeA.speedVar * RANDOM_M11(&RANDSEED);
                Vec2 dir = v * s;
                m_particleData[i].modeA.dirX = dir.x;    //v * s ;
                m_particleData[i].modeA.dirY = dir.y;
                m_particleData[i].rotation = -Rad2Deg(dir.getAngle());
            }
        }
        else
        {
            for (int i = start; i < m_particleCount; ++i)
            {
                float a = Deg2Rad(m_angle + m_angleVar * RANDOM_M11(&RANDSEED));
                Vec2 v(cosf(a), sinf(a));
                float s = modeA.speed + modeA.speedVar * RANDOM_M11(&RANDSEED);
                Vec2 dir = v * s;
                m_particleData[i].modeA.dirX = dir.x;    //v * s ;
                m_particleData[i].modeA.dirY = dir.y;
            }
        }
    }

    // Mode Radius: B
    else
    {
        for (int i = start; i < m_particleCount; ++i)
        {
            m_particleData[i].modeB.radius = modeB.startRadius + modeB.startRadiusVar * RANDOM_M11(&RANDSEED);
        }

        for (int i = start; i < m_particleCount; ++i)
        {
            m_particleData[i].modeB.angle = Deg2Rad(m_angle + m_angleVar * RANDOM_M11(&RANDSEED));
        }

        for (int i = start; i < m_particleCount; ++i)
        {
            m_particleData[i].modeB.degreesPerSecond = Deg2Rad(modeB.rotatePerSecond + modeB.rotatePerSecondVar * RANDOM_M11(&RANDSEED));
        }

        if (modeB.endRadius == START_RADIUS_EQUAL_TO_END_RADIUS)
        {
            for (int i = start; i < m_particleCount; ++i)
            {
                m_particleData[i].modeB.deltaRadius = 0.0f;
            }
        }
        else
        {
            for (int i = start; i < m_particleCount; ++i)
            {
                float endRadius = modeB.endRadius + modeB.endRadiusVar * RANDOM_M11(&RANDSEED);
                m_particleData[i].modeB.deltaRadius = (endRadius - m_particleData[i].modeB.radius) / m_particleData[i].timeToLive;
            }
        }
    }
}

void ParticleSystem::stopSystem()
{
    m_isActive = false;
    m_elapsed = m_duration;
    m_emitCounter = 0;
}

void ParticleSystem::resetSystem()
{
    m_isActive = true;
    m_elapsed = 0;
    for (int i = 0; i < m_particleCount; ++i)
    {
        //m_particleData[i].timeToLive = 0.0f;
    }
}

bool ParticleSystem::isFull()
{
    return (m_particleCount == m_totalParticles);
}

// ParticleSystem - MainLoop
void ParticleSystem::update()
{
    float dt = 1.0 / 25;
    if (m_isActive && m_emissionRate)
    {
        float rate = 1.0f / m_emissionRate;
        int totalParticles = m_totalParticles;

        //issue #1201, prevent bursts of particles, due to too high emitCounter
        if (m_particleCount < totalParticles)
        {
            m_emitCounter += dt;
            if (m_emitCounter < 0.f)
            {
                m_emitCounter = 0.f;
            }
        }

        int emitCount = (std::min)(1.0f * (totalParticles - m_particleCount), m_emitCounter / rate);
        addParticles(emitCount);
        m_emitCounter -= rate * emitCount;

        m_elapsed += dt;
        if (m_elapsed < 0.f)
        {
            m_elapsed = 0.f;
        }
        if (m_duration != DURATION_INFINITY && m_duration < m_elapsed)
        {
            this->stopSystem();
        }
    }

    for (int i = 0; i < m_particleCount; ++i)
    {
        m_particleData[i].timeToLive -= dt;
    }

    // rebirth
    for (int i = 0; i < m_particleCount; ++i)
    {
        if (m_particleData[i].timeToLive <= 0.0f)
        {
            int j = m_particleCount - 1;
            //while (j > 0 && m_particleData[i].timeToLive <= 0)
            //{
            //    m_particleCount--;
            //    j--;
            //}
            m_particleData[i] = m_particleData[m_particleCount - 1];
            --m_particleCount;
        }
    }

    if (m_emitterMode == Mode::GRAVITY)
    {
        for (int i = 0; i < m_particleCount; ++i)
        {
            Pointf tmp, radial = { 0.0f, 0.0f }, tangential;

            // radial acceleration
            if (m_particleData[i].posx || m_particleData[i].posy)
            {
                normalize_point(m_particleData[i].posx, m_particleData[i].posy, &radial);
            }
            tangential = radial;
            radial.x *= m_particleData[i].modeA.radialAccel;
            radial.y *= m_particleData[i].modeA.radialAccel;

            // tangential acceleration
            std::swap(tangential.x, tangential.y);
            tangential.x *= -m_particleData[i].modeA.tangentialAccel;
            tangential.y *= m_particleData[i].modeA.tangentialAccel;

            // (gravity + radial + tangential) * dt
            tmp.x = radial.x + tangential.x + modeA.gravity.x;
            tmp.y = radial.y + tangential.y + modeA.gravity.y;
            tmp.x *= dt;
            tmp.y *= dt;

            m_particleData[i].modeA.dirX += tmp.x;
            m_particleData[i].modeA.dirY += tmp.y;

            // this is cocos2d-x v3.0
            // if (m_configName.length()>0 && m_yCoordFlipped != -1)

            // this is cocos2d-x v3.0
            tmp.x = m_particleData[i].modeA.dirX * dt * m_yCoordFlipped;
            tmp.y = m_particleData[i].modeA.dirY * dt * m_yCoordFlipped;
            m_particleData[i].posx += tmp.x;
            m_particleData[i].posy += tmp.y;
        }
    }
    else
    {
        for (int i = 0; i < m_particleCount; ++i)
        {
            m_particleData[i].modeB.angle += m_particleData[i].modeB.degreesPerSecond * dt;
            m_particleData[i].modeB.radius += m_particleData[i].modeB.deltaRadius * dt;
            m_particleData[i].posx = -cosf(m_particleData[i].modeB.angle) * m_particleData[i].modeB.radius;
            m_particleData[i].posy = -sinf(m_particleData[i].modeB.angle) * m_particleData[i].modeB.radius * m_yCoordFlipped;
        }
    }

    //color, size, rotation
    for (int i = 0; i < m_particleCount; ++i)
    {
        m_particleData[i].colorR += m_particleData[i].deltaColorR * dt;
        m_particleData[i].colorG += m_particleData[i].deltaColorG * dt;
        m_particleData[i].colorB += m_particleData[i].deltaColorB * dt;
        m_particleData[i].colorA += m_particleData[i].deltaColorA * dt;
        m_particleData[i].size += (m_particleData[i].deltaSize * dt);
        m_particleData[i].size = (std::max)(0.0f, m_particleData[i].size);
        m_particleData[i].rotation += m_particleData[i].deltaRotation * dt;
    }
}

// ParticleSystem - Texture protocol
void ParticleSystem::setTexture(SDL_Texture* var)
{
    if (m_texture != var)
    {
        m_texture = var;
    }
}

void ParticleSystem::draw()
{
    if (m_texture == nullptr)
    {
        return;
    }
    for (int i = 0; i < m_particleCount; i++)
    {
        auto& p = m_particleData[i];
        if (p.size <=0 || p.colorA <= 0)
        {
            continue;
        }
        SDL_Rect r = { int(p.posx + p.startPosX - p.size / 2), int(p.posy + p.startPosY - p.size / 2), int(p.size), int(p.size) };
        SDL_Color c = { Uint8(p.colorR * 255), Uint8(p.colorG * 255), Uint8(p.colorB * 255), Uint8(p.colorA * 255) };
        SDL_SetTextureColorMod(m_texture, c.r, c.g, c.b);
        SDL_SetTextureAlphaMod(m_texture, c.a);
        SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopyEx(m_renderer, m_texture, nullptr, &r, p.rotation, nullptr, SDL_FLIP_NONE);
    }
    update();
}

SDL_Texture* ParticleSystem::getTexture()
{
    return m_texture;
}

// ParticleSystem - Properties of Gravity Mode
void ParticleSystem::setTangentialAccel(float t)
{
    modeA.tangentialAccel = t;
}

float ParticleSystem::getTangentialAccel() const
{
    return modeA.tangentialAccel;
}

void ParticleSystem::setTangentialAccelVar(float t)
{
    modeA.tangentialAccelVar = t;
}

float ParticleSystem::getTangentialAccelVar() const
{
    return modeA.tangentialAccelVar;
}

void ParticleSystem::setRadialAccel(float t)
{
    modeA.radialAccel = t;
}

float ParticleSystem::getRadialAccel() const
{
    return modeA.radialAccel;
}

void ParticleSystem::setRadialAccelVar(float t)
{
    modeA.radialAccelVar = t;
}

float ParticleSystem::getRadialAccelVar() const
{
    return modeA.radialAccelVar;
}

void ParticleSystem::setRotationIsDir(bool t)
{
    modeA.rotationIsDir = t;
}

bool ParticleSystem::getRotationIsDir() const
{
    return modeA.rotationIsDir;
}

void ParticleSystem::setGravity(const Vec2& g)
{
    modeA.gravity = g;
}

const Vec2& ParticleSystem::getGravity()
{
    return modeA.gravity;
}

void ParticleSystem::setSpeed(float speed)
{
    modeA.speed = speed;
}

float ParticleSystem::getSpeed() const
{
    return modeA.speed;
}

void ParticleSystem::setSpeedVar(float speedVar)
{

    modeA.speedVar = speedVar;
}

float ParticleSystem::getSpeedVar() const
{

    return modeA.speedVar;
}

// ParticleSystem - Properties of Radius Mode
void ParticleSystem::setStartRadius(float startRadius)
{
    modeB.startRadius = startRadius;
}

float ParticleSystem::getStartRadius() const
{
    return modeB.startRadius;
}

void ParticleSystem::setStartRadiusVar(float startRadiusVar)
{
    modeB.startRadiusVar = startRadiusVar;
}

float ParticleSystem::getStartRadiusVar() const
{
    return modeB.startRadiusVar;
}

void ParticleSystem::setEndRadius(float endRadius)
{
    modeB.endRadius = endRadius;
}

float ParticleSystem::getEndRadius() const
{
    return modeB.endRadius;
}

void ParticleSystem::setEndRadiusVar(float endRadiusVar)
{
    modeB.endRadiusVar = endRadiusVar;
}

float ParticleSystem::getEndRadiusVar() const
{

    return modeB.endRadiusVar;
}

void ParticleSystem::setRotatePerSecond(float degrees)
{
    modeB.rotatePerSecond = degrees;
}

float ParticleSystem::getRotatePerSecond() const
{
    return modeB.rotatePerSecond;
}

void ParticleSystem::setRotatePerSecondVar(float degrees)
{
    modeB.rotatePerSecondVar = degrees;
}

float ParticleSystem::getRotatePerSecondVar() const
{
    return modeB.rotatePerSecondVar;
}

bool ParticleSystem::isActive() const
{
    return m_isActive;
}

int ParticleSystem::getTotalParticles() const
{
    return m_totalParticles;
}

void ParticleSystem::setTotalParticles(int var)
{
    m_totalParticles = var;
}

bool ParticleSystem::isAutoRemoveOnFinish() const
{
    return m_isAutoRemoveOnFinish;
}

void ParticleSystem::setAutoRemoveOnFinish(bool var)
{
    m_isAutoRemoveOnFinish = var;
}

////don't use a transform matrix, this is faster
//void ParticleSystem::setScale(float s)
//{
//    m_transformSystemDirty = true;
//    Node::setScale(s);
//}
//
//void ParticleSystem::setRotation(float newRotation)
//{
//    m_transformSystemDirty = true;
//    Node::setRotation(newRotation);
//}
//
//void ParticleSystem::setScaleX(float newScaleX)
//{
//    m_transformSystemDirty = true;
//    Node::setScaleX(newScaleX);
//}
//
//void ParticleSystem::setScaleY(float newScaleY)
//{
//    m_transformSystemDirty = true;
//    Node::setScaleY(newScaleY);
//}

bool ParticleSystem::isPaused() const
{
    return m_paused;
}

void ParticleSystem::pauseEmissions()
{
    m_paused = true;
}

void ParticleSystem::resumeEmissions()
{
    m_paused = false;
}
