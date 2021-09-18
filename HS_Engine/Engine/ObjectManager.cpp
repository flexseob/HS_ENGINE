#include "ObjectManager.h"
#include "Types.h"
#include <iostream>
#include "MeshManager.h"
#include "Object.h"


namespace HS_Engine
{
	void ObjectManager::AddObject(Object* object)
	{
		auto findobject = mObjects.find(object->GetObjectName());

		if(findobject == mObjects.end())
		{
			std::string objectname = object->GetObjectName();
			if (object->GetMesh() == nullptr)
			{
				switch (object->GetMeshTypes())
				{
				case E_MeshTypes::BY_PATHEMESH:
					Mesh* mesh = mMeshManager.AddMesh(object->GetMeshName(), object->GetObjPath());
					object->SetMesh(mesh);
					mObjects.insert({ objectname,object });
					break;

				case E_MeshTypes::BY_PRECEDURALMESH:
					//TODO : PreceduralMesh need to add
					break;
				}
			}
			else
			{
				mObjects.insert({ objectname,object });
			}
		}
		else
		{
			std::cout << "Object name is Exist! : " << object->GetObjectName() << std::endl;
			delete object;
		}

		
	}

	void ObjectManager::DeleteObject(std::string objname)
	{
		auto findobject = mObjects.find(objname);

		if (findobject != mObjects.end())
		{
			Object* object = findobject->second;
			delete object;
			object = nullptr;
			mObjects.erase(findobject);
			std::cout << "Object is deleted " << objname << std::endl;
		}
		else
		{
			std::cout << "Object name is not exist : " << objname << std::endl;
		}
	}

	void ObjectManager::DeleteObjectAll()
	{
		for(auto& object : mObjects)
		{
			Object* objptr = object.second;
			delete objptr;
			objptr = nullptr;
		}
		
		mObjects.clear();
	}

	Object* ObjectManager::GetObject(std::string objname)
	{
		auto findobject = mObjects.find(objname);

		if (findobject != mObjects.end())
		{
			return findobject->second;
		}
		else
		{
			std::cout << "Cannot find the object! : " << objname << std::endl;
			return nullptr;
		}
		
	}
}
