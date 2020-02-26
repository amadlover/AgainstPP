#pragma once

#include "scene.hpp"

class test_scene : public scene
{
public:
    test_scene ();
    ~test_scene ();

	egraphics_result init () override;
	void process_keyboard_input (WPARAM wParam, LPARAM lParam);
	egraphics_result main_loop () override;
	void exit () override;

protected:
	egraphics_result update () override;
	egraphics_result draw () const override;
};