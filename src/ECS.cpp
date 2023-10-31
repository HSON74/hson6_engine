
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
	this->Get<BoxCollider>(old).IsStatis = true;
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
	this->Get<Sprite>(e).position += v;
	this->Get<Position>(e).x += v.x;
	this->Get<Position>(e).y += v.y;
	this->Get<Position>(e).z += v.z;
	this->Get<BoxCollider>(e).offset += v;
}
bool ECS::Collide(EntityID e, std::string tag)
{
	bool result = 0;
	Sprite coll_s = Get<Sprite>(e);
	ForEach<Sprite>([&](EntityID e1) {
		Sprite coll_s1 = Get<Sprite>(e1);
		if (coll_s1.tag.compare(tag)) {
			m_Types::vec3 a = this->Get<Collider>(e).offset;
			m_Types::vec2 a_size = this->Get<BoxCollider>(e).size;
			m_Types::vec3 b = this->Get<Collider>(e1).offset;
			m_Types::vec2 b_size = this->Get<BoxCollider>(e1).size;
			if ((a.x-a_size.x < b.x + b_size.x || a.x + a_size.x > b.x - b_size.x)&&(a.y + a_size.y > b.y - b_size.y || a.y - a_size.y < b.y + b_size.y)) {
				BoxCollide(e, e1);
				result = true;
			}
		}
		
	});
	return false;
}
bool ECS::BoxCollide(EntityID e1, EntityID e2) {
	//std::cout << "Solve 1" << std::endl;
	m_Types::vec3 c = this->Get<BoxCollider>(e1).offset;
	m_Types::vec2 c_size = this->Get<BoxCollider>(e1).size;
	m_Types::vec3 d = this->Get<BoxCollider>(e2).offset;
	m_Types::vec2 d_size = this->Get<BoxCollider>(e2).size;
	m_Types::vec3 tmp = this->Get<BoxCollider>(e1).offset;
	c_size.x = 100;
	c_size.y = 100;
	d_size.x = 100;
	d_size.y = 100;
	std::cout << "CX: " << c.x << std::endl;
	std::cout << "CY: " << c.y << std::endl;
	std::cout << "C_Size.X: " << c_size.x << std::endl;
	std::cout << "C_Size.Y: " << c_size.y << std::endl;
	std::cout << "DX: " << d.x << std::endl;
	std::cout << "DY: " << d.y << std::endl;
	std::cout << "D_Size.X: " << d_size.x << std::endl;
	std::cout << "D_Size.Y: " << d_size.y << std::endl;
	std::cout << "C-C_SizeX: " << (c.x - c_size.x) << std::endl;
	std::cout << "C+C_SizeX: " << (c.x + c_size.x) << std::endl;
	std::cout << "C-C_SizeY: " << (c.y - c_size.y) << std::endl;
	std::cout << "C+C_SizeY: " << (c.y + c_size.y) << std::endl;
	std::cout << "D-D_SizeX: " << (d.x - d_size.x) << std::endl;
	std::cout << "D+D_SizeX: " << (d.x + d_size.x) << std::endl;
	std::cout << "D-D_SizeY: " << (d.y - d_size.y) << std::endl;
	std::cout << "D+D_SizeY: " << (d.y + d_size.y) << std::endl;
	bool result = false;
	if (c.x < d.x && c.x + c_size.x > d.x - d_size.x) {
		if (this->Get<BoxCollider>(e1).IsStatis) {
			tmp.x = c.x + ((d.x - d_size.x) + (c.x + c_size.x));
			result = true;
		}
	}else if(c.x > d.x && c.x - c_size.x < d.x + d_size.x) {
		if (this->Get<BoxCollider>(e1).IsStatis) {
			tmp.x = c.x + ((d.x + d_size.x) + (c.x - c_size.x));
			result = true;
		}
	}
	if (c.y > d.y && c.y - c_size.y < d.y + d_size.y) {
		std::cout << "Solve:1 "<< std::endl;
		if (this->Get<BoxCollider>(e1).IsStatis) {
			tmp = this->Get<BoxCollider>(e1).offset;
			result = true;
		}

	}else if (c.y < d.y &&  c.y + c_size.y > d.y - d_size.y) {
		std::cout << "Solve:2" << std::endl;
		if (this->Get<BoxCollider>(e1).IsStatis) {
			tmp = this->Get<BoxCollider>(e1).offset;
			result = true;
		}
		
	}
	if (result) {
		setPosition(e1, tmp);
	}
	return result;
}
