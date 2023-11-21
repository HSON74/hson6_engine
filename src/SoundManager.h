#pragma

#include "Types.h"

class SoundManager {
	public:
		std::string sSPath = "./";
		SoLoud::Soloud sound_m;
		SoLoud::Wav my_sound;
		std::unordered_map<std::string, SoLoud::Wav> my_sound_dict;
		bool LoadSound(const std::string& name, const std::string& path);
		SoundManager();
		void StartUp();
		void Play(const std::string& name);
		void Loop(const std::string& name, bool activecode);
		void Stop(const std::string& name);
		void SetVolume(const std::string& name, float vol);
		void Shutdown();

};