#pragma once
#include "Base/Main/Common.h"

#include <boost/uuid/uuid.hpp>

namespace S2DE::GameObjects
{
	class S2DE_API GameObjectIDGenerator
	{
	public:
		GameObjectIDGenerator();
		~GameObjectIDGenerator();

		//Set uuid by string 
		void							SetUUID(const std::string id_str);

		//Set uuid by existing uuid
		void							SetUUID(boost::uuids::uuid uuid);

		//Get current uuid in string format 
		inline std::string				GetUUIDString() const;

		//Get current uuid
		inline boost::uuids::uuid		GetUUID() const { return m_uuid; };

		//Generate new UUID
		void							RegenerateUUID();

		//Convert string to uuid 
		static boost::uuids::uuid		ConvertStringToUUID(const std::string id);

		//Convert uuid to string 
		static std::string				ConvertUUIDToString(boost::uuids::uuid uuid);

	private:
		boost::uuids::uuid m_uuid;
	};
}