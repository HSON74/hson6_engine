#include "SoundManager.h"
#include "ResourceManager.h"

std::shared_ptr<ResourceManager> s_resources = std::shared_ptr<ResourceManager>();
bool SoundManager::LoadSound(const std::string& name, const std::string& path)
{
	std::string tmp_p = this->sSPath + "assets\\" + path;
	if (my_sound_dict.count(name) != 0) {
		std::cout << "The name: " << name << "is already taken" << std::endl;
		return false;
	}
	else {
		my_sound_dict[name].load(tmp_p.c_str());
		std::cout << "Load sound " << tmp_p << std::endl;
	}
	return true;
}
SoundManager::SoundManager() {

	std::string sSPath = s_resources->pathToString(s_resources->getCurrentPath());
}

void SoundManager::StartUp()
{
	sound_m.init();
}

void SoundManager::Play(const std::string& name)
{
	if (my_sound_dict.size() != 0 && my_sound_dict.contains(name)) {
		std::cout << "Play Sound" << std::endl;
		double t = my_sound_dict[name].getLength();
		double t_1 = 1;
		int handle = sound_m.play(my_sound_dict[name]);
		//sound_m.setVolume(handle, 1.5f);
		
	}
	else {
		std::cout << "Sound Don't Exist" << std::endl;
	}
}

void SoundManager::Loop(const std::string& name, bool activecode)
{
	if (my_sound_dict.size() != 0 && my_sound_dict.contains(name)) {
		if (activecode) {
			my_sound_dict[name].setLooping(1);
		}
		else {
			my_sound_dict[name].setLooping(0);
		}
	}else {
		std::cout << "Sound Don't Exist" << std::endl;
	}
}

void SoundManager::Stop(const std::string& name)
{
	if (my_sound_dict.size() != 0 && my_sound_dict.contains(name)) {
		std::cout << "Stop Sound" << std::endl;
		my_sound_dict[name].stop();
	}
	else {
		std::cout << "Sound Don't Exist" << std::endl;
	}
}

void SoundManager::SetVolume(const std::string& name, float vol)
{
	if (my_sound_dict.size() != 0 && my_sound_dict.contains(name)) {
		my_sound_dict[name].setVolume(vol * 0.01f);
	}
	else {
		std::cout << "Sound Don't Exist" << std::endl;
	}
}

void SoundManager::Shutdown()
{
	if (!my_sound_dict.empty()) {
		for (auto m_sound = my_sound_dict.begin(); m_sound != my_sound_dict.end(); m_sound++) {
			auto cur = m_sound->first;
			my_sound_dict.erase(cur);
		}
		my_sound_dict.clear();

	}
	sound_m.deinit();
}
