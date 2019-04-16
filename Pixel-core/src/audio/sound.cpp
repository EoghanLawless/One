#include "sound.h"

namespace pixel {
	namespace audio {

		Sound::Sound(const std::string& alias, const std::string& file) : _alias(alias), _file(file) {
			_loaded_sound = cs_load_wav(file.c_str());
			_sound_def = cs_make_def(&_loaded_sound);
		}
		Sound::~Sound() {
			cs_free_sound(&_loaded_sound);
		}

		cs_playing_sound_t* Sound::play(cs_context_t* context) {
			if (_playing_sound)
				if(_playing_sound->active)
					return nullptr;

			return _playing_sound = cs_play_sound(context, _sound_def);
		}
		void Sound::resume() {
			if (!_playing_sound)
				return;

			if(_playing_sound->paused)
				cs_pause_sound(_playing_sound, 0);
		}
		void Sound::pause() {
			if (!_playing_sound)
				return;

			if (_playing_sound->paused)
				cs_pause_sound(_playing_sound, 0);
			else
				cs_pause_sound(_playing_sound, 1);
		}
		void Sound::loop() {
			if (!_playing_sound)
				return;

			if (_playing_sound->looped)
				cs_loop_sound(_playing_sound, 0);
			else
				cs_loop_sound(_playing_sound, 1);
		}
		void Sound::stop() {
			if (!_playing_sound)
				return;

			if (_playing_sound->active)
				cs_stop_sound(_playing_sound);
		}

	}
}
