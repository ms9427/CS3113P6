#include "menu.h"
#include "Utility.h"

#define MENU_WIDTH 20
#define MENU_HEIGHT 20


unsigned int MENU_DATA[] =
{
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void menu::initialise()
{
	GLuint map_texture_id = Utility::load_texture("tileset.png");
	m_state.map = new Map(MENU_WIDTH, MENU_HEIGHT, MENU_DATA, map_texture_id, 1.0f, 12, 11);

	m_state.player = new Entity();
	m_state.player->set_entity_type(PLAYER);
	m_state.player->set_position(glm::vec3(2.0f, 0.0f, 0.0f));
	m_state.player->set_movement(glm::vec3(0.0f));
	m_state.player->set_speed(2.5f);
	m_state.player->m_texture_id = Utility::load_texture("wizard.png");
	m_state.player->set_height(0.8f);
	m_state.player->set_width(0.8f);
}

void menu::update(float delta_time)
{
	m_state.player->update(delta_time, m_state.player, NULL, 0, m_state.map);
}

void menu::render(ShaderProgram* program)
{
	GLuint font_texture_id = Utility::load_texture("font1.png");
	Utility::draw_text(program, font_texture_id, "Wizarding World", 0.5f, 0.001f, glm::vec3(-1.85, 2, 0));
	Utility::draw_text(program, font_texture_id, "Collect the Potions!", 0.4f, 0.001f, glm::vec3(-1.85, 1, 0));
	Utility::draw_text(program, font_texture_id, "USE SPACE & ARROW KEYS", 0.4f, 0.001f, glm::vec3(-1.85, 0, 0));
	Utility::draw_text(program, font_texture_id, "Press ENTER to Start", 0.4f, 0.001f, glm::vec3(-1.85, -1, 0));
}