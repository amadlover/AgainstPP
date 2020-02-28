#pragma once

#include "scene.hpp"
#include "mesh.hpp"
#include "actor.hpp"

class game;

class test_scene : public scene
{
public:
    test_scene ();
	test_scene (game* ptr);
    ~test_scene ();

	egraphics_result init () override;
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	egraphics_result main_loop () override;
	void exit () override;

protected:
	egraphics_result update () override;
	egraphics_result draw () const override { return egraphics_result::success; };
	egraphics_result update_command_buffers () const;
	egraphics_result present ();

private:
	egraphics_result spawn_ship ();

	std::vector<mesh> meshes;
	std::list<actor> actors;
};