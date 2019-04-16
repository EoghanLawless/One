#pragma once

#include <string>

#include "cute-sound.h"

namespace pixel {
	namespace audio {

		class Sound {
		private:
			std::string _alias;
			std::string _file;

			cs_loaded_sound_t _loaded_sound;
			cs_play_sound_def_t _sound_def;
			cs_playing_sound_t* _playing_sound;

		public:
			Sound(const std::string& alias, const std::string& file);
			~Sound();

			cs_playing_sound_t* play(cs_context_t* context);
			void pause();
			void resume();
			void loop();
			void stop();

			inline cs_loaded_sound_t getLoadedSound() const { return _loaded_sound; }

			inline const std::string& getAlias() const { return _alias; }
			inline const std::string& getFile() const { return _file; }
		};

	}
}