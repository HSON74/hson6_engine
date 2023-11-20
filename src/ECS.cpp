
#include "ECS.h"
#include "ResourceManager.h"
#include "stb_image.h"
//IT working


std::shared_ptr<ResourceManager> ecsr = std::make_shared<ResourceManager>();
ECS::ECS()
{
	
}
void ECS::Shutdown() {
	this->environmentManipulation.clear();
	this->ForEach<Sprite>([&](EntityID e) {
		this->Destroy(e);
		});
	if (!this->m_components.empty()) { this->m_components.clear(); }
	
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
	this->Get<Sprite>(old).position = m_Types::vec3(0, 0, 0);
	this->Get<Sprite>(old).rotation = m_Types::vec3(0, 0, 45);
	this->Get<Sprite>(old).scale = m_Types::vec3(1, 1,1);
	this->Get<Sprite>(old).z_value = 10;
	this->Get<Health>(old).percent = DBL_MIN;
	this->Get<Sprite>(old).imagePath = ecsr->pathToString(ecsr->getCurrentPath() / ecsr->stringToPath("assets") / ecsr->stringToPath(m_path));
	this->Get<BoxCollider>(old).size = m_Types::vec3(100, 100, 100);
	this->Get<BoxCollider>(old).offset = m_Types::vec3(0, 0, 0);
	this->Get<BoxCollider>(old).active = true;
	this->Get<BoxCollider>(old).IsStatis = true;
	this->Get<Sprite>(old).active = true;
	this->Get<Sprite>(old).in_active = false;
	this->Get<Position>(old).x = 0;
	this->Get<Position>(old).y = 0;
	this->Get<Position>(old).z = 0;
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
	bool result = false;
	Sprite coll_s = Get<Sprite>(e);
	ForEach<Sprite>([&](EntityID e1) {
		Sprite coll_s1 = Get<Sprite>(e1);
		if (coll_s1.tag == tag) {
			result = CheckBoxCollide(e, e1);
		}
	});
	return result;
}
bool ECS::BoxCollide(EntityID e1, EntityID e2) {
	m_Types::vec2 m_size  = m_Types::vec2(90,90);
	m_Types::vec3 c = this->Get<BoxCollider>(e1).offset;
	m_Types::vec2 c_size = this->Get<BoxCollider>(e1).size;
	m_Types::vec3 d = this->Get<BoxCollider>(e2).offset;
	m_Types::vec2 d_size = this->Get<BoxCollider>(e2).size;;
	m_Types::vec3 tmp = m_Types::vec3(0, 0, 0);
	/*std::cout << "CX: " << c.x << std::endl;
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
	std::cout << "D+D_SizeY: " << (d.y + d_size.y) << std::endl;*/
	bool result = false;
	/*if ((c.x <= d.x && c.x + c_size.x >= d.x - d_size.x || c.x >= d.x && c.x - c_size.x <= d.x + d_size.x)) {
		std::cout << "x is true" << std::endl;
	}
	else {
		std::cout << "x is false" << std::endl;
	}
	if ((c.y >= d.y && c.y - c_size.y <= d.y + d_size.y) || (c.y <= d.y && c.y + c_size.y >= d.y - d_size.y)) {
		std::cout << "y is true" << std::endl;
	}
	else {
		std::cout << "y is false" << std::endl;
	}*/
	if (((c.x <= d.x && c.x + c_size.x >= d.x - d_size.x || c.x >= d.x && c.x - c_size.x <= d.x + d_size.x)) &&
		((c.y >= d.y && c.y - c_size.y <= d.y + d_size.y) || (c.y <= d.y && c.y + c_size.y >= d.y - d_size.y))) {
		m_Types::vec3 tmptmp = m_Types::vec3(0, 0, 0);
		if (c.x <= d.x && c.x + c_size.x >= d.x - d_size.x) {
			tmptmp.x -= ((c.x + c_size.x) - (d.x - d_size.x));
			result = true;
		}else if (c.x >= d.x && c.x - c_size.x <= d.x + d_size.x) {
			tmptmp.x -= ((c.x - c_size.x) - (d.x + d_size.x));
			//std::cout << "We are one" << std::endl;
			result = true;
		}
		if (c.y >= d.y && c.y - c_size.y <= d.y + d_size.y) {
			tmptmp.y -= ((c.y - c_size.y) - (d.y + d_size.y));
			result = true;
		}else if (c.y <= d.y && c.y + c_size.y >= d.y - d_size.y) {
			tmptmp.y -= ((c.y + c_size.y) - (d.y - d_size.y));
			result = true;
		}
		if (std::abs(tmptmp.x) < std::abs(tmptmp.y)) {
			tmp.x = tmptmp.x;
		}
		else  if (std::abs(tmptmp.x) > std::abs(tmptmp.y)) {
			tmp.y = tmptmp.y;
		}
		else {
			tmp.x = tmptmp.x;
			tmp.y = tmptmp.y;
		}
	}
	
	if (result && !(this->Get<BoxCollider>(e1).IsTrigger)) {
		setPosition(e1, tmp);
	}
	return result;
}
bool ECS::CheckBoxCollide(EntityID e1, EntityID e2) {
	m_Types::vec2 m_size = m_Types::vec2(90, 90);
	m_Types::vec3 c = this->Get<BoxCollider>(e1).offset;
	m_Types::vec2 c_size = this->Get<BoxCollider>(e1).size;
	m_Types::vec3 d = this->Get<BoxCollider>(e2).offset;
	m_Types::vec2 d_size = this->Get<BoxCollider>(e2).size;;
	m_Types::vec3 tmp = m_Types::vec3(0, 0, 0);
	bool check_result = false;

	if (((c.x <= d.x && c.x + c_size.x >= d.x - d_size.x || c.x >= d.x && c.x - c_size.x <= d.x + d_size.x)) &&
		((c.y >= d.y && c.y - c_size.y <= d.y + d_size.y) || (c.y <= d.y && c.y + c_size.y >= d.y - d_size.y))) {
		check_result = true;
	}
	return check_result;
}
m_Types::vec3 ECS::BoxCollidePostion(EntityID e1, EntityID e2) {
	m_Types::vec2 m_size = m_Types::vec2(90, 90);
	m_Types::vec3 c = this->Get<BoxCollider>(e1).offset;
	m_Types::vec2 c_size = this->Get<BoxCollider>(e1).size;
	m_Types::vec3 d = this->Get<BoxCollider>(e2).offset;
	m_Types::vec2 d_size = this->Get<BoxCollider>(e2).size;;
	m_Types::vec3 tmp = m_Types::vec3(0, 0, 0);
	bool check_result = false;

	if (((c.x <= d.x && c.x + c_size.x >= d.x - d_size.x || c.x >= d.x && c.x - c_size.x <= d.x + d_size.x)) &&
		((c.y >= d.y && c.y - c_size.y <= d.y + d_size.y) || (c.y <= d.y && c.y + c_size.y >= d.y - d_size.y))) {
		m_Types::vec3 tmptmp = m_Types::vec3(0, 0, 0);
		if (c.x <= d.x && c.x + c_size.x >= d.x - d_size.x) {
			tmptmp.x -= ((c.x + c_size.x) - (d.x - d_size.x));
			check_result = true;
		}
		else if (c.x >= d.x && c.x - c_size.x <= d.x + d_size.x) {
			tmptmp.x -= ((c.x - c_size.x) - (d.x + d_size.x));
			//std::cout << "We are one" << std::endl;
			check_result = true;
		}
		if (c.y >= d.y && c.y - c_size.y <= d.y + d_size.y) {
			tmptmp.y -= ((c.y - c_size.y) - (d.y + d_size.y));
			check_result = true;
		}
		else if (c.y <= d.y && c.y + c_size.y >= d.y - d_size.y) {
			tmptmp.y -= ((c.y + c_size.y) - (d.y - d_size.y));
			check_result = true;
		}
		if (std::abs(tmptmp.x) < std::abs(tmptmp.y)) {
			tmp.x = tmptmp.x;
		}
		else  if (std::abs(tmptmp.x) > std::abs(tmptmp.y)) {
			tmp.y = tmptmp.y;
		}
		else {
			tmp.x = tmptmp.x;
			tmp.y = tmptmp.y;
		}
	}
	return tmp;
}
EntityID ECS::BoxEntity(EntityID e, std::string tag)
{
	EntityID result = -1;
	Sprite coll_s = Get<Sprite>(e);
	ForEach<Sprite>([&](EntityID e1) {
		Sprite coll_s1 = Get<Sprite>(e1);
		if (coll_s1.tag == tag) {
			if (CheckBoxCollide(e, e1)) {
				result = e1;
				return;
			}
		}
	});
	return result;
}
void ECS::CreateUI(std::string m_name)
{
	EntityID old = this->sizeEntity++;
	this->Get<Sprite>(old).imageName = m_name;
	this->Get<Sprite>(old).EntityN = old;
	this->Get<Sprite>(old).tag = "UI";
	this->Get<Sprite>(old).imageName = m_name;
	this->Get<Sprite>(old).position = m_Types::vec3(0, 0, 0);
	this->Get<Sprite>(old).rotation = m_Types::vec3(0, 0, 0);
	this->Get<Sprite>(old).scale = m_Types::vec3(1, 1, 1);
	this->Get<Sprite>(old).z_value = 10;
	this->Get<Health>(old).percent = DBL_MIN;
	this->Get<Sprite>(old).active = true;
	this->Get<Sprite>(old).in_active = false;
	this->Get<Position>(old).x = 0;
	this->Get<Position>(old).y = 0;
	this->Get<Position>(old).z = 0;
	this->Get<UI>(old).text = "";
	this->Get<UI>(old).tex_text = "";
}
void ECS::ChangeText(EntityID e, std::string m_name)
{
	this->Get<UI>(e).text = m_name;
	this->Get<UI>(e).tex_text = m_name;

}
