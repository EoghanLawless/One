#pragma once

#include <vector>

#include "sound.h"

namespace pixel {
	namespace audio {

		class SoundManager {
		private:
			cs_context_t* _context;
			std::vector<Sound*> _sounds;

		public:
			SoundManager(HWND hwnd, unsigned int frequency, unsigned int latency, unsigned int seconds, unsigned int sound_pool);
			~SoundManager();

			cs_playing_sound_t* play(Sound* sound) const;
			cs_playing_sound_t* play(const std::string& alias) const;
			void pause(Sound* sound) const;
			void pause(const std::string& alias) const;
			void resume(Sound* sound) const;
			void resume(const std::string& alias) const;
			void loop(Sound* sound) const;
			void loop(const std::string& alias) const;
			void stop(Sound* sound) const;
			void stop(const std::string& alias) const;

			void add(Sound* sound);
			Sound* get(const std::string& alias) const;
			void clean();

			cs_context_t* getContext() const;
		};

	}
}