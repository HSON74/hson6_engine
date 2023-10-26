#pragma
#include "Types.h"
namespace path = std::filesystem;

class ResourceManager {
	public:
		path::path rsP;
		std::string rSPath;
		std::string getPath();
		ResourceManager();
		std::filesystem::path stringToPath(std::string);
		std::string pathToString(std::filesystem::path pPath);
		std::filesystem::path getCurrentPath();
		void StartUp();
		void Shutdown();
};