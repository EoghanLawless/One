#include "soundmanager.h"

#include <iostream>

namespace pixel {
	namespace audio {

		SoundManager::SoundManager(HWND hwnd, unsigned int frequency, unsigned int latency, unsigned int seconds, unsigned int sound_pool) {
			_context = cs_make_context(hwnd, frequency, latency, seconds, sound_pool);
			cs_spawn_mix_thread(_context);
			cs_thread_sleep_delay(_context, 10);
		}
		SoundManager::~SoundManager() {
			cs_shutdown_context(_context);

			for(Sound* sound : _sounds)
				cs_free_sound(&sound->getLoadedSound());
		}


		cs_playing_sound_t* SoundManager::play(Sound* sound) const {
			return sound->play(_context);
		}
		cs_playing_sound_t* SoundManager::play(const std::string& alias) const {
			return get(alias)->play(_context);
		}
		void SoundManager::pause(Sound* sound) const {
			sound->pause();
		}
		void SoundManager::pause(const std::string& alias) const {
			get(alias)->pause();
		}
		void SoundManager::resume(Sound* sound) const {
			sound->resume();
		}
		void SoundManager::resume(const std::string& alias) const {
			get(alias)->resume();
		}
		void SoundManager::loop(Sound* sound) const {
			sound->loop();
		}
		void SoundManager::loop(const std::string& alias) const {
			get(alias)->loop();
		}
		void SoundManager::stop(Sound* sound) const {
			sound->stop();
		}
		void SoundManager::stop(const std::string& alias) const {
			get(alias)->stop();
		}

		void SoundManager::setPan(Sound* sound, float pan) const {
			sound->setPan(pan);
		}
		void SoundManager::setPan(const std::string& alias, float pan) const {
			get(alias)->setPan(pan);
		}
		void SoundManager::setVolume(Sound* sound, float volume) const {
			sound->setVolume(volume);
		}
		void SoundManager::setVolume(const std::string& alias, float volume) const {
			get(alias)->setVolume(volume);
		}

		void SoundManager::add(Sound* sound) {
			_sounds.push_back(sound);
		}
		Sound* SoundManager::get(const std::string& alias) const {
			for (Sound* sound: _sounds)
				if (sound->getAlias() == alias)
					return sound;

			return nullptr;
		}
		void SoundManager::clean() {
			for (int index = 0; index < _sounds.size(); index++)
				delete _sounds[index];
		}

		cs_context_t* SoundManager::getContext() const {
			return _context;
		}

	}
}