#include "EntityFactory.h"
#include "Serializer.h"

namespace OpenGLFun {
	EntityFactory::EntityFactory() {}

	EntityId EntityFactory::CreateEntityFromFile(const char* filepath) {
		std::string json_from_file = Serializer::GetFileContents(filepath);

		rapidjson::Document document;
		if (document.Parse(json_from_file.c_str()).HasParseError()) {
			// I put filepath into std::string, so that I can append new strings or char pointers via add operator
			throw SimpleException(std::string(filepath) + " has an incorrect JSON format");
		}

		// the root must be a JSON object
		if (!document.IsObject())
			throw SimpleException(std::string(filepath) + " must start with a JSON object");

		// Declare entity id for use in subsequent codes
		EntityId entityId;

		// Initialise entityId
		std::string parent("");
		if (document.HasMember("parent") && document["parent"].IsString())
			parent = document["parent"].GetString();
		if (parent != "") { // there's a parent, so load parent by calling CreateEntityFromFile to parse the parent's JSON file
			entityId = CreateEntityFromFile(parent.c_str());
		}
		else {
			entityId = ENTITY_MANAGER->SpawnEntity();
		}

		// Load components
		if (document.HasMember("components")) {
			// "components" must be an array
			if (!document["components"].IsArray())
				throw JsonReadException(filepath, "Components", "JSON array");

			const rapidjson::Value& compArr = document["components"];
			for (rapidjson::SizeType i = 0; i < compArr.Size(); i++) {
				// not exactly parsed or is of basic C++ type, so called it compJson
				const rapidjson::Value& compJson = compArr[i];

				// Component must be an object
				if (!compJson.IsObject())
					throw JsonReadException(filepath, std::string("components[") + std::to_string(i) + "]", "JSON object");

				// "type" member must be a string
				if (!compJson.HasMember("type") || !compJson["type"].IsString())
					throw JsonReadException(filepath, std::string("components[") + std::to_string(i) + "]", "type", "string");
				//throw SimpleException(std::string("Component") + std::to_string(i) + "'s type must be a string, in " + filepath);

				std::string compType = compJson["type"].GetString();
				// Could not find the component type, so throw exception!
				if (COMPONENT_MANAGER->mComponentCreatorsMap.find(compType) == COMPONENT_MANAGER->mComponentCreatorsMap.end()) {
					throw SimpleException(std::string("Could not find component type \"") + compType + "\" for " + filepath);
				}

				IComponent* component = nullptr;
				try {
					component = COMPONENT_MANAGER->mComponentCreatorsMap.find(compType)->second->Create();
					component->mOwner = entityId; // assign entity id, cannot forget!!!
					component->Deserialize(compJson); // after creating an instance of a component, serialise it!!!
					COMPONENT_MANAGER->AddComponent(component);
				}
				catch (std::exception const& e) { // catch all parse errors, so that we can include the filename in the error too!
					if (component != nullptr)
						delete component;
					throw SimpleException(std::string("Failed to parse ") + filepath + ", here's the parse error for components[" + std::to_string(i) + "]: " + e.what());
				}
			}
		}

		return entityId; // failed to create an entity, so -1
	}
}