#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"

Entity::Entity()
{
    // ����� PHYSICS ����� //
    m_position = glm::vec3(0.0f);
    m_velocity = glm::vec3(0.0f);
    m_acceleration = glm::vec3(0.0f);

    // ����� TRANSLATION ����� //
    m_movement = glm::vec3(0.0f);
    m_speed = 0;
    m_model_matrix = glm::mat4(1.0f);
}

Entity::~Entity()
{
  
}


void Entity::ai_activate(Entity* player)
{
    switch (m_ai_type)
    {
    case WALKER:
        ai_walk();
        break;

    case GUARD:
        ai_guard(player);
        break;

    default:
        break;
    }
}

void Entity::ai_walk()
{
    m_movement = glm::vec3(-1.0f, 0.0f, 0.0f);
}

float x_direction = 0.0f;
float y_direction = 0.0f;

void Entity::ai_guard(Entity* player)
{
    switch (m_ai_state) {
    case IDLE:
        if (glm::distance(m_position, player->get_position()) < 3.0f) m_ai_state = WALKING;
        break;

    case WALKING:
      
        if (m_position.x > player->get_position().x) {
            x_direction = -1.0f;
        }
        else if (m_position.x < player->get_position().x) {
            x_direction = 1.0f;
        }

        if (m_position.y > player->get_position().y) {
            y_direction = -1.0f;
        }
        else if (m_position.y < player->get_position().y) {
            y_direction = 1.0f;
        }

        m_movement = glm::vec3(x_direction, y_direction, 0.0f);
        break;

    case ATTACKING:
        break;

    default:
        break;
    }
}


void Entity::update(float delta_time, Entity* player, Entity* objects, int object_count, Map* map)
{
    if (!m_is_active) return;

    m_collided_top = false;
    m_collided_bottom = false;
    m_collided_left = false;
    m_collided_right = false;

    if (m_entity_type == ENEMY) ai_activate(player);

    m_velocity.x = m_movement.x * m_speed;
    m_velocity.y = m_movement.y * m_speed;
    m_velocity += m_acceleration * delta_time;

    // We make two calls to our check_collision methods, one for the collidable objects and one for
    // the map.
    m_position.y += m_velocity.y * delta_time;
    check_collision_y(objects, object_count);
    check_collision_y(map);

    m_position.x += m_velocity.x * delta_time;
    check_collision_x(objects, object_count);
    check_collision_x(map);



    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
    m_model_matrix = glm::scale(m_model_matrix, glm::vec3(m_width, m_height, 1.0f));
}

void const Entity::check_collision_y(Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];

        if (check_collision(collidable_entity))
        {
            if (m_entity_type == PLAYER && collidable_entity->m_entity_type == POTION)
            {
                collidable_entity->deactivate();
                num_potions_collected += 1;
            }

            if (m_entity_type == ENEMY && collidable_entity->m_entity_type == PLAYER)
            {
                   collidable_entity->m_is_active = false;
                
            }
            if (m_entity_type == FIREBALL && collidable_entity->m_entity_type == ENEMY)
            {
                collidable_entity->m_is_active = false;
                m_is_active = false;
            }

            float y_distance = fabs(m_position.y - collidable_entity->get_position().y);
            float y_overlap = fabs(y_distance - (m_height / 2.0f) - (collidable_entity->get_height() / 2.0f));
            if (m_velocity.y > 0) {
                m_position.y -= y_overlap;
                m_velocity.y = 0;
                m_collided_top = true;
            }
            else if (m_velocity.y < 0) {
                m_position.y += y_overlap;
                m_velocity.y = 0;
                m_collided_bottom = true;
            }
        }
    }
}

void const Entity::check_collision_x(Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];

        if (check_collision(collidable_entity))
        {
            if (m_entity_type == PLAYER && collidable_entity->m_entity_type == POTION)
            {
                collidable_entity->deactivate();
                num_potions_collected += 1;
            }

            if (m_entity_type == ENEMY && collidable_entity->m_entity_type == PLAYER)
            {
                    collidable_entity->m_is_active = false;

            }

            if (m_entity_type == FIREBALL && collidable_entity->m_entity_type == ENEMY)
            {
                collidable_entity->m_is_active = false;
                m_is_active = false;
            }

         

            float x_distance = fabs(m_position.x - collidable_entity->get_position().x);
            float x_overlap = fabs(x_distance - (m_width / 2.0f) - (collidable_entity->get_width() / 2.0f));
            if (m_velocity.x > 0) {
                m_position.x -= x_overlap;
                m_velocity.x = 0;
                m_collided_right = true;
            }
            else if (m_velocity.x < 0) {
                m_position.x += x_overlap;
                m_velocity.x = 0;
                m_collided_left = true;
            }
        }
    }
}

void const Entity::check_collision_y(Map* map)
{
    // Probes for tiles above
    glm::vec3 top = glm::vec3(m_position.x, m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_left = glm::vec3(m_position.x - (m_width / 2), m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_right = glm::vec3(m_position.x + (m_width / 2), m_position.y + (m_height / 2), m_position.z);

    // Probes for tiles below
    glm::vec3 bottom = glm::vec3(m_position.x, m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_left = glm::vec3(m_position.x - (m_width / 2), m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_right = glm::vec3(m_position.x + (m_width / 2), m_position.y - (m_height / 2), m_position.z);

    float penetration_x = 0;
    float penetration_y = 0;


    // If the map is solid, check the top three points
    if (map->is_solid(top, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
        if (m_entity_type == FIREBALL)
        {
            m_is_active = false;
        }

    }
    else if (map->is_solid(top_left, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
        if (m_entity_type == FIREBALL)
        {
            m_is_active = false;
        }

    }
    else if (map->is_solid(top_right, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
        if (m_entity_type == FIREBALL)
        {
            m_is_active = false;
        }

    }

    // And the bottom three points
    if (map->is_solid(bottom, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
        if (m_entity_type == FIREBALL)
        {
            m_is_active = false;
        }

    }
    else if (map->is_solid(bottom_left, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
        if (m_entity_type == FIREBALL)
        {
            m_is_active = false;
        }

    }
    else if (map->is_solid(bottom_right, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
        if (m_entity_type == FIREBALL)
        {
            m_is_active = false;
        }


    }
}

void const Entity::check_collision_x(Map* map)
{
    // Probes for tiles; the x-checking is much simpler
    glm::vec3 left = glm::vec3(m_position.x - (m_width / 2), m_position.y, m_position.z);
    glm::vec3 right = glm::vec3(m_position.x + (m_width / 2), m_position.y, m_position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->is_solid(left, &penetration_x, &penetration_y) && m_velocity.x < 0)
    {
        m_position.x += penetration_x;
        m_velocity.x = 0;
        m_collided_left = true;
        if (m_entity_type == FIREBALL)
        {
            m_is_active = false;
        }

    }
    if (map->is_solid(right, &penetration_x, &penetration_y) && m_velocity.x > 0)
    {
        m_position.x -= penetration_x;
        m_velocity.x = 0;
        m_collided_right = true;
        if (m_entity_type == FIREBALL)
        {
            m_is_active = false;
        }

    }
}



void Entity::render(ShaderProgram* program)
{
    if (!m_is_active) { return; }

    program->set_model_matrix(m_model_matrix);


    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());



}

bool const Entity::check_collision(Entity* other) const
{
    if (other == this) return false;
    // If either entity is inactive, there shouldn't be any collision
    if (!m_is_active || !other->m_is_active) return false;

    float x_distance = fabs(m_position.x - other->m_position.x) - ((m_width + other->m_width) / 2.0f);
    float y_distance = fabs(m_position.y - other->m_position.y) - ((m_height + other->m_height) / 2.0f);

    return x_distance < 0.0f && y_distance < 0.0f;
}