#pragma once

#include <godot_cpp/classes/control.hpp>

namespace godot {

	class LoadingScreen : public Control {
		GDCLASS(LoadingScreen, Control)

	private:
	protected:
		static void _bind_methods();

	public:
		LoadingScreen();
		~LoadingScreen();
        void _process(double delta) override;
	};

}