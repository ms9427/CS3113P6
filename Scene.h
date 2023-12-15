#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Utility.h"
#include "Entity.h"
#include "Map.h"

struct GameState
{
    Map* map;
    Entity* player;
    Entity* enemies;
    Entity* potions;
    Entity* fireballs;

    Mix_Music* bgm;
    Mix_Chunk* shoot_sfx;
};

class Scene {
public:
    int m_number_of_enemies = 1;
    int m_number_of_potions = 1;

    GameState m_state;

    
    virtual void initialise() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render(ShaderProgram* program) = 0;
    GameState const get_state() const { return m_state; }

    //ADDED
    void const set_number_of_enemies(int num_enemies) { m_number_of_enemies = num_enemies; }
    void const set_number_of_keys(int num_potions) { m_number_of_potions = num_potions; }
    int const get_number_of_enemies() const { return m_number_of_enemies; }
    int const get_number_of_keys() const { return m_number_of_potions; }
};