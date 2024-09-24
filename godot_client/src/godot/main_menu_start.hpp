#pragma once

#include <godot_cpp/classes/button.hpp>

namespace godot {

	class MainMenuStart : public Button {
		GDCLASS(MainMenuStart, Button)

	private:
	protected:
		static void _bind_methods();

	public:
		MainMenuStart();
		~MainMenuStart();
		void _pressed() override;
	};

}