#include "LevelA.h"
#include "Utility.h"

#define LEVELA_WIDTH 20
#define LEVELA_HEIGHT 20

#define LEVELA_ENEMY_COUNT 8
#define LEVELA_POTION_COUNT 8

unsigned int LEVELA_DATA[] =
{
1, 2, 3, 12, 12, 12, 12, 12, 12, 1, 2, 3, 12, 12, 1, 2, 2, 2, 2, 3,
13, 48, 15, 12, 12, 12, 12, 12, 12, 13, 48, 15, 12, 12, 13, 48, 48, 48, 48, 15,
13, 48, 15, 1, 2, 2, 2, 3, 12, 13, 48, 15, 12, 12, 13, 48, 48, 48, 48, 15,
13, 48, 15, 13, 48, 48, 48, 15, 12, 13, 48, 15, 12, 12, 13, 48, 48, 48, 48, 15,
13, 48, 16, 17, 48, 48, 48, 15, 12, 13, 48, 15, 1, 2, 17, 48, 48, 48, 4, 27,
13, 48, 48, 48, 48, 48, 48, 15, 12, 13, 48, 16, 17, 48, 48, 48, 48, 48, 15, 12,
13, 48, 48, 48, 48, 48, 48, 16, 2, 17, 48, 48, 48, 48, 48, 48, 48, 48, 15, 12,
25, 5, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 4, 26, 27, 12,
1, 17, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 16, 2, 3, 12,
13, 48, 48, 48, 48, 4, 26, 26, 26, 26, 26, 26, 5, 48, 48, 48, 48, 48, 16, 3,
13, 48, 48, 48, 48, 16, 2, 2, 2, 2, 2, 2, 17, 48, 48, 48, 48, 48, 48, 15,
13, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 15,
13, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 15,
13, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 4, 26, 26, 26, 26, 27,
13, 48, 48, 48, 48, 48, 48, 48, 4, 5, 48, 48, 48, 48, 15, 12, 12, 12, 12, 12,
13, 48, 4, 26, 26, 26, 26, 26, 27, 13, 48, 48, 48, 48, 15, 12, 1, 2, 2, 3,
13, 48, 15, 1, 2, 2, 2, 2, 2, 17, 48, 48, 48, 48, 15, 12, 13, 48, 48, 15,
13, 48, 16, 17, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 16, 2, 17, 48, 48, 15,
13, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 15,
25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 27


};

LevelA::~LevelA()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.shoot_sfx);
    Mix_FreeMusic(m_state.bgm);

}

void LevelA::initialise()
{
    GLuint map_texture_id = Utility::load_texture("tilemap_packed.png");
    m_state.map = new Map(LEVELA_WIDTH, LEVELA_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 12, 11);

    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(7.0f, -13.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->m_texture_id = Utility::load_texture("wizard.png");
    m_state.player->set_height(0.9f);
    m_state.player->set_width(0.9f);

    GLuint fireball_texture_id = Utility::load_texture("fireball.png");
    m_state.fireballs = new Entity();
    m_state.fireballs->set_entity_type(FIREBALL);
    m_state.fireballs->set_speed(7.5f);
    m_state.fireballs->m_texture_id = fireball_texture_id;
    m_state.fireballs->set_height(0.5f);
    m_state.fireballs->set_width(0.5f);
    m_state.fireballs->m_is_active = false;

    /** Enemies' stuff */
    GLuint enemy_texture_id1 = Utility::load_texture("evil_wizard.png");
    GLuint enemy_texture_id2 = Utility::load_texture("evil_wizard.png");
    GLuint enemy_texture_id3 = Utility::load_texture("evil_wizard.png");
    GLuint enemy_texture_id4 = Utility::load_texture("evil_wizard.png");
    GLuint enemy_texture_id5 = Utility::load_texture("evil_wizard.png");
    GLuint enemy_texture_id6 = Utility::load_texture("evil_wizard.png");
    GLuint enemy_texture_id7 = Utility::load_texture("evil_wizard.png");
    GLuint enemy_texture_id8 = Utility::load_texture("evil_wizard.png");
    m_state.enemies = new Entity[LEVELA_ENEMY_COUNT];

    for (int i = 0; i < LEVELA_ENEMY_COUNT; i++)
    {
        if (i == 0)
        {
            m_state.enemies[i].set_entity_type(ENEMY);
            m_state.enemies[i].set_ai_type(GUARD);
            m_state.enemies[i].set_ai_state(IDLE);
            m_state.enemies[i].m_texture_id = enemy_texture_id1;
            m_state.enemies[i].set_movement(glm::vec3(0.0f));
            m_state.enemies[i].set_speed(0.8f);
            m_state.enemies[i].set_width(0.8f);
            m_state.enemies[i].set_height(0.8f);
            m_state.enemies[i].set_position(glm::vec3(1.0f, -5.0f, 0.0f)); 
        }
        else if (i == 1)
        {
            m_state.enemies[i].set_entity_type(ENEMY);
            m_state.enemies[i].set_ai_type(GUARD);
            m_state.enemies[i].set_ai_state(IDLE);
            m_state.enemies[i].m_texture_id = enemy_texture_id2;
            m_state.enemies[i].set_movement(glm::vec3(0.0f));
            m_state.enemies[i].set_speed(0.8f);
            m_state.enemies[i].set_width(0.8f);
            m_state.enemies[i].set_height(0.8f);
            m_state.enemies[i].set_position(glm::vec3(3.0f, -8.0f, 0.0f)); 
        }
        else if (i == 2)
        {
            m_state.enemies[i].set_entity_type(ENEMY);
            m_state.enemies[i].set_ai_type(GUARD);
            m_state.enemies[i].set_ai_state(IDLE);
            m_state.enemies[i].m_texture_id = enemy_texture_id3;
            m_state.enemies[i].set_movement(glm::vec3(0.0f));
            m_state.enemies[i].set_speed(0.8f);
            m_state.enemies[i].set_width(0.8f);
            m_state.enemies[i].set_height(0.8f);
            m_state.enemies[i].set_position(glm::vec3(3.0f, -11.0f, 0.0f)); 
        }
        else if (i == 3)
        {
            m_state.enemies[i].set_entity_type(ENEMY);
            m_state.enemies[i].set_ai_type(GUARD);
            m_state.enemies[i].set_ai_state(IDLE);
            m_state.enemies[i].m_texture_id = enemy_texture_id4;
            m_state.enemies[i].set_movement(glm::vec3(0.0f));
            m_state.enemies[i].set_speed(0.8f);
            m_state.enemies[i].set_width(0.8f);
            m_state.enemies[i].set_height(0.8f);
            m_state.enemies[i].set_position(glm::vec3(2.0f, -18.0f, 0.0f)); 
        }
        else if (i == 4)
        {
            m_state.enemies[i].set_entity_type(ENEMY);
            m_state.enemies[i].set_ai_type(GUARD);
            m_state.enemies[i].set_ai_state(IDLE);
            m_state.enemies[i].m_texture_id = enemy_texture_id5;
            m_state.enemies[i].set_movement(glm::vec3(0.0f));
            m_state.enemies[i].set_speed(0.8f);
            m_state.enemies[i].set_width(0.8f);
            m_state.enemies[i].set_height(0.8f);
            m_state.enemies[i].set_position(glm::vec3(10.0f, -5.0f, 0.0f)); 
        }
        else if (i == 5)
        {
            m_state.enemies[i].set_entity_type(ENEMY);
            m_state.enemies[i].set_ai_type(GUARD);
            m_state.enemies[i].set_ai_state(IDLE);
            m_state.enemies[i].m_texture_id = enemy_texture_id6;
            m_state.enemies[i].set_movement(glm::vec3(0.0f));
            m_state.enemies[i].set_speed(0.8f);
            m_state.enemies[i].set_width(0.8f);
            m_state.enemies[i].set_height(0.8f);
            m_state.enemies[i].set_position(glm::vec3(14.0f, -12.0f, 0.0f)); 
        }
        else if (i == 6)
        {
            m_state.enemies[i].set_entity_type(ENEMY);
            m_state.enemies[i].set_ai_type(GUARD);
            m_state.enemies[i].set_ai_state(IDLE);
            m_state.enemies[i].m_texture_id = enemy_texture_id7;
            m_state.enemies[i].set_movement(glm::vec3(0.0f));
            m_state.enemies[i].set_speed(0.8);
            m_state.enemies[i].set_width(0.8f);
            m_state.enemies[i].set_height(0.8f);
            m_state.enemies[i].set_position(glm::vec3(15.0f, -18.0f, 0.0f)); 
        }
        else
        {
            m_state.enemies[i].set_entity_type(ENEMY);
            m_state.enemies[i].set_ai_type(GUARD);
            m_state.enemies[i].set_ai_state(IDLE);
            m_state.enemies[i].m_texture_id = enemy_texture_id8;
            m_state.enemies[i].set_movement(glm::vec3(0.0f));
            m_state.enemies[i].set_speed(0.8f);
            m_state.enemies[i].set_width(0.8f);
            m_state.enemies[i].set_height(0.8f);
            m_state.enemies[i].set_position(glm::vec3(16.0f, -4.0f, 0.0f)); 
        }
    }

     //Initlaizing Potions
    GLuint potion_texture_id1 = Utility::load_texture("potion.png");
    GLuint potion_texture_id2 = Utility::load_texture("potion.png");
    GLuint potion_texture_id3 = Utility::load_texture("potion.png");
    GLuint potion_texture_id4 = Utility::load_texture("potion.png");
    GLuint potion_texture_id5 = Utility::load_texture("potion.png");
    GLuint potion_texture_id6 = Utility::load_texture("potion.png");
    GLuint potion_texture_id7 = Utility::load_texture("potion.png");
    GLuint potion_texture_id8 = Utility::load_texture("potion.png");
    m_state.potions = new Entity[LEVELA_POTION_COUNT];

    for (int i = 0; i < LEVELA_POTION_COUNT; i++)
    {
        if (i == 0)
        {
            m_state.potions[i].set_entity_type(POTION);
            m_state.potions[i].set_width(0.6f);
            m_state.potions[i].set_height(0.6f);
            m_state.potions[i].m_texture_id = potion_texture_id1;
            m_state.potions[i].set_position(glm::vec3(1.0f, -3.0f, 0.0f));
        }
        else if (i == 1)
        {
            m_state.potions[i].set_entity_type(POTION);
            m_state.potions[i].set_width(0.6f);
            m_state.potions[i].set_height(0.6f);
            m_state.potions[i].m_texture_id = potion_texture_id2;
            m_state.potions[i].set_position(glm::vec3(2.0f, -10.0f, 0.0f)); 
        }
        else if (i == 2)
        {
            m_state.potions[i].set_entity_type(POTION);
            m_state.potions[i].set_width(0.6f);
            m_state.potions[i].set_height(0.6f);
            m_state.potions[i].m_texture_id = potion_texture_id3;
            m_state.potions[i].set_position(glm::vec3(4.0f, -17.0f, 0.0f)); 
        }
        else if (i == 3)
        {
            m_state.potions[i].set_entity_type(POTION);
            m_state.potions[i].set_width(0.6f);
            m_state.potions[i].set_height(0.6f);
            m_state.potions[i].m_texture_id = potion_texture_id4;
            m_state.potions[i].set_position(glm::vec3(10.0f, -2.0f, 0.0f)); 
        }
        else if (i == 4)
        {
            m_state.potions[i].set_entity_type(POTION);
            m_state.potions[i].set_width(0.6f);
            m_state.potions[i].set_height(0.6f);
            m_state.potions[i].m_texture_id = potion_texture_id5;
            m_state.potions[i].set_position(glm::vec3(11.0f, -15.0f, 0.0f)); 
        }
        else if (i == 5)
        {
            m_state.potions[i].set_entity_type(POTION);
            m_state.potions[i].set_width(0.6f);
            m_state.potions[i].m_texture_id = potion_texture_id6;
            m_state.potions[i].set_height(0.6f);
            m_state.potions[i].set_position(glm::vec3(18.0f, -2.0f, 0.0f)); 
        }
        else if (i == 6)
        {
            m_state.potions[i].set_entity_type(POTION);
            m_state.potions[i].set_width(0.6f);
            m_state.potions[i].set_height(0.6f);
            m_state.potions[i].m_texture_id = potion_texture_id7;
            m_state.potions[i].set_position(glm::vec3(18.0f, -12.0f, 0.0f));
        }
        else
        {
            m_state.potions[i].set_entity_type(POTION);
            m_state.potions[i].set_width(0.6f);
            m_state.potions[i].set_height(0.6f); 
            m_state.potions[i].m_texture_id = potion_texture_id8;
            m_state.potions[i].set_position(glm::vec3(18.0f, -16.0f, 0.0f));
        }
    }

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("background.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(10.0f);

    m_state.shoot_sfx = Mix_LoadWAV("shooting.wav");

}

void LevelA::throw_fire(glm::vec3 movement)
{
    m_state.fireballs->set_position(m_state.player->get_position());
    m_state.fireballs->set_movement(movement);
}

void LevelA::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.potions, LEVELA_POTION_COUNT, m_state.map);

    for (int i = 0; i < LEVELA_ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, m_state.player, 1, m_state.map);
    }

    for (int i = 0; i < LEVELA_POTION_COUNT; i++)
    {
        m_state.potions[i].update(delta_time, m_state.player, m_state.player, 1, m_state.map);
    }

    m_state.fireballs->update(delta_time, m_state.player, m_state.enemies, LEVELA_ENEMY_COUNT, m_state.map);
}

void LevelA::render(ShaderProgram* program)
{
   
    m_state.map->render(program);
    m_state.player->render(program);
    m_state.fireballs->render(program);

    for (int i = 0; i < LEVELA_ENEMY_COUNT; i++) {
        m_state.enemies[i].render(program);
    }
    for (int i = 0; i < LEVELA_POTION_COUNT; i++)
    {
        m_state.potions[i].render(program);

    }

    if (m_state.player->num_potions_collected == LEVELA_POTION_COUNT)

    {
        for (int i = 0; i < LEVELA_ENEMY_COUNT; i++) {
            m_state.enemies[i].m_is_active = false;
        }
        Utility::draw_text(program, Utility::load_texture("font1.png"), "You Win!", 0.5f, -0.25f, glm::vec3(m_state.player->get_position().x -1.0f, m_state.player->get_position().y - 1.0f, 0.0f));
    }

    if (m_state.player->m_is_active == false) {
        Utility::draw_text(program, Utility::load_texture("font1.png"), "You Lose!", 0.5f, -0.25f, glm::vec3(m_state.player->get_position().x, m_state.player->get_position().y - 1.0f, 0.0f));
    }
}