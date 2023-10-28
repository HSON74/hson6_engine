
#include "ECS.h"
#include "ResourceManager.h"
#include "stb_image.h"
//IT working


std::shared_ptr<ResourceManager> ecsr = std::make_shared<ResourceManager>();
ECS::ECS()
{
	
}

EntityID ECS::Create(std::string m_name, std::string m_path)
{
	EntityID old = this->sizeEntity++;
	this->Get<Sprite>(old).imageName = m_name;
	this->Get<Sprite>(old).EntityN = old;
	std::string tmp = this-> Get<Sprite>(old).imageName;
	std::ostringstream oss;
	oss << this->Get<Sprite>(old).EntityN;
	tmp += oss.str();
	this->Get<Sprite>(old).imageName = tmp;
	this->Get<Sprite>(old).position = m_Types::vec3(0, 0, 90);
	this->Get<Sprite>(old).scale = m_Types::vec3(1, 1,1);
	this->Get<Sprite>(old).z_value = 10;
	this->Get<Health>(old).percent = DBL_MIN;
	this->Get<Sprite>(old).imagePath = ecsr->pathToString(ecsr->getCurrentPath() / ecsr->stringToPath("assets") / ecsr->stringToPath(m_path));
	if (!m_path.empty()) {
		int width, height, channels;
		unsigned char* data = stbi_load(this->Get<Sprite>(old).imagePath.c_str(), &width, &height, &channels, 4);
		this->Get<BoxCollider>(old).size = m_Types::vec3(width, height, 0);
		this->Get<BoxCollider>(old).offset = m_Types::vec3(0, 0, 0);
		stbi_image_free(data);
		

	}
	else {
		this->Get<BoxCollider>(old).size = m_Types::vec3(100, 100, 100);
		this->Get<BoxCollider>(old).offset = m_Types::vec3(0, 0, 0);

	}
	this->Get<BoxCollider>(old).active = true;
	this->Get<Sprite>(old).active = true;
	this->Get<Sprite>(old).in_active = false;
	this->Get<Position>(old).x = 0;
	this->Get<Position>(old).y = 0;
	this->Get<Position>(old).z = 0;
	//std::cout << this->Get<Sprite>(old).imageName << std::endl;
	//std::cout << this->Get<Sprite>(old).imagePath << std::endl;
	return old;
}
void ECS::setPosition(EntityID e, m_Types::vec3 v)
{
	this->Get<Position>(e).x = v.x;
	this->Get<Position>(e).y = v.y;
	this->Get<Position>(e).z = v.z;
	this->Get<BoxCollider>(e).offset = v;
}
bool ECS::Collider(EntityID e, std::string tag)
{
	bool result = 0;
	Sprite coll_s = Get<Sprite>(e);
	ForEach<Sprite>([&](EntityID e1) {
		Sprite coll_s1 = Get<Sprite>(e1);
		if (coll_s1.tag._Equal(tag)) {
			result = true;
		}
		
	});
	return false;
}
