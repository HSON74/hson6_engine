#include "ResourceManager.h"

using namespace m_Types;

std::string ResourceManager::getPath()
{
	return rSPath;
}

ResourceManager::ResourceManager()
{
	rSPath = "./";
	rsP = getCurrentPath();
}

std::filesystem::path ResourceManager::stringToPath(std::string pString)
{
	
	return std::filesystem::path(pString);
}

std::string ResourceManager::pathToString(std::filesystem::path pPath)
{
	std::string tmpPath = pPath.string();
	return tmpPath;
}

std::filesystem::path ResourceManager::getCurrentPath()
{
	return std::filesystem::current_path();
}

void ResourceManager::StartUp()
{
	
}

void ResourceManager::Shutdown()
{
}




