#include "GraphicsPCH.h"
#include "ContentManager.h"
#include "Mesh.h"

CContentManager::CContentManager()
{
}

CContentManager::~CContentManager()
{
}

void CContentManager::Update(float _deltaTime)
{
	//for (std::list<CEntity*>::iterator itr = m_entities.begin(); itr != m_entities.end(); itr++)
	//{
	//	(*itr)->Update(_deltaTime);
	//}

	for (auto itr : m_entities)
	{
		itr->Update(_deltaTime);

		if (RotateAll)
		{
			itr->Rotate(_deltaTime);
		}

		if (Move)
		{
			itr->Move(_deltaTime);
		}
	}

	CleanUp();
}

void CContentManager::Render()
{
	for (auto itr : m_entities)
	{
		if (itr->GetComponent<CMesh>() != nullptr)
		{
			itr->GetComponent<CMesh>()->Render();
		}
	}
}

bool CContentManager::AddEntity(CEntity* _entity)
{
	if (!_entity || ContainsEntity(_entity))
	{
		return false;
	}

	if (_entity->Init())
	{
		m_entities.push_back(_entity);
	}

	return false;
}

bool CContentManager::RemoveEntity(CEntity* _entity)
{
	if (!_entity || !ContainsEntity(_entity))
	{
		return false;
	}

	m_entitiesToDelete.push_back(_entity);
	return true;
}

bool CContentManager::ContainsEntity(CEntity* _entity)
{
	if (!_entity)
	{
		return false;
	}

	for (auto itr : m_entities)
	{
		if (itr == _entity)
		{
			return true;
		}
	}

	return false;
}

void CContentManager::CleanUp()
{
	for (auto itr : m_entitiesToDelete)
	{
		m_entities.remove(itr);
		itr->DeInit();
		delete(itr);
	}

	m_entitiesToDelete.clear();

}