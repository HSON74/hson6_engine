#pragma

#include "Types.h"

class ScriptsManager {
public:

	std::unordered_map<std::string, sol::load_result> ScriptMap;
	sol::state lua;
	void StartUp();
	void Shutdown();
	bool LoadScript(const std::string& name, const std::string& path);
};