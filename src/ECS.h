#pragma

#include "Types.h"


typedef int64_t EntityID;
typedef std::type_index ComponentIndex;


class SparseSetHolder {
public:
	// A virtual destructor, since subclasses need their destructors to run to free memory.
	virtual ~SparseSetHolder() = default;
	virtual bool Has(EntityID) const = 0;
	virtual void Drop(EntityID) = 0;

	SparseSetHolder() = default;
};
// Subclasses are templated on the component type they hold.
template< typename T > class SparseSet : public SparseSetHolder {
public:
	std::unordered_map< EntityID, T > data;
	bool Has(EntityID e) const override { return data.count(e) > 0; };
	void Drop(EntityID e) override { data.erase(e); };
};

class ECS
{
public:
	std::unordered_map< ComponentIndex, std::unique_ptr< SparseSetHolder > > m_components;
	std::vector<EntityID> environmentManipulation;
	template< typename T > T& Get(EntityID e);
	EntityID sizeEntity = 0;
	ECS();
	//Create a Entity
	EntityID Create(std::string m_name, std::string m_path);
	void Shutdown();
	//Get the Entity with compnenets. 
	template< typename T >
	std::unordered_map< EntityID, T >&
		GetAppropriateSparseSet() {
		// Get the index for T's SparseSet
		const ComponentIndex index = std::type_index(typeid(T));
		// Create the actual sparse set if needed.
		if (m_components[index] == nullptr) m_components[index] = std::make_unique< SparseSet<T> >();
		// It's safe to cast the SparseSetHolder to its subclass and return the std::unordered_map< EntityID, T > inside.
		return static_cast<SparseSet<T>&>(*m_components[index]).data;
	}
	
	//Public Function
	// Drop a component from an entity.
	template< typename T >
	void Drop(EntityID e) {
		GetAppropriateSparseSet<T>().erase(e);
		
	}
	/*template <typename T, typename... Rest>
	bool HasAll(EntityID entity) {
		bool result = true;
		if constexpr (sizeof...(Rest) > 0) { result = HasAll<Rest...>(entity); }
		return result && GetAppropriateSparseSet<T>().count(entity) > 0;
	}*/


	void setPosition(EntityID e, m_Types::vec3 v);
	bool Collide(EntityID e, std::string tag);
	bool BoxCollide(EntityID e1, EntityID e2);
	bool CheckBoxCollide(EntityID e1, EntityID e2);
	m_Types::vec3 BoxCollidePostion(EntityID e1, EntityID e2);
	EntityID BoxEntity(EntityID e1, std::string tag);
	void CreateUI(std::string m_name);
	void ChangeText(EntityID e, std::string m_name);



	// Destroy the entity by removing all components.
	void Destroy(EntityID e) {
		for (const auto& [index, comps] : m_components) { comps->Drop(e); }
	}
	typedef std::function<void(EntityID)> ForEachCallback;
	template<typename EntitiesThatHaveThisComponent, typename... AndAlsoTheseComponents>
	void ForEach(const ForEachCallback& callback) {
		// Get a vector of ComponentIndex we can use with `m_components[ index ]->Has( entity )`.
		std::vector<ComponentIndex> also{ std::type_index(typeid(AndAlsoTheseComponents))... };
		// Iterate over entities in the first container.
		// If the entity has all components in `also`, call `callback( entity )`.
		// ... your code goes here ...
		
		auto& container = GetAppropriateSparseSet<EntitiesThatHaveThisComponent>();
		for (const auto& [entity, value] : container) {
			callback(entity);
		}
		/*for (EntityID i = 0; i < this->sizeEntity; i++) {

			callback(i);
		}*/
	}
};

template<typename T>
inline T& ECS::Get(EntityID e)
{
	// TODO: insert return statement here
	return GetAppropriateSparseSet<T>()[e];
}



