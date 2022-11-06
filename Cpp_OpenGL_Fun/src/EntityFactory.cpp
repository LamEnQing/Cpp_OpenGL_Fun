#include "EntityFactory.h"
#include "FileOps.h"

namespace OpenGLFun {
	EntityFactory::EntityFactory() {}
	
	EntityId EntityFactory::DeserializeEntity(std::string const& filename, rapidjson::Value const& jsonObj, bool allowParent) {
		// Declare entity id for use in subsequent codes
		EntityId entityId;

		// Initialise entityId
		std::string parent("");
		if (allowParent && jsonObj.HasMember("parent") && jsonObj["parent"].IsString())
			parent = jsonObj["parent"].GetString();
		if (parent != "") { // there's a parent, so load parent by calling CreateEntityFromFile to parse the parent's JSON file
			entityId = CreateEntityFromFile(parent.c_str());
		}
		else {
			entityId = ENTITY_MANAGER->SpawnEntity();
		}

		// Load components
		if (jsonObj.HasMember("components")) {
			// "components" must be an array
			if (!jsonObj["components"].IsObject())
				throw JsonReadException(filename, "components", "JSON object");

			for (auto& compJson : jsonObj["components"].GetObject()) {
				std::string compType = compJson.name.GetString();
				// Could not find the component type, so throw exception!
				if (COMPONENT_MANAGER->mComponentTypeMap.find(compType) == COMPONENT_MANAGER->mComponentTypeMap.end()) {
					throw SimpleException(std::string("Could not find component type \"") + compType + "\" for " + filename);
				}

				IComponent* component = nullptr;
				try {
					component = COMPONENT_MANAGER->mComponentCreatorsMap.at(COMPONENT_MANAGER->mComponentTypeMap.at(compType))->Create();
					component->mOwner = entityId; // assign entity id, cannot forget!!!
					component->Deserialize(compJson.value.GetObject()); // after creating an instance of a component, serialise it!!!
					COMPONENT_MANAGER->AddComponent(component);
				}
				catch (std::exception const& e) { // catch all parse errors, so that we can include the filename in the error too!
					if (component != nullptr)
						delete component;
					throw SimpleException(std::string("Failed to parse ") + filename + ", here's the parse error: " + e.what());
				}
			}
		}

		return entityId; // failed to create an entity, so -1
	}

	EntityId EntityFactory::CreateEntityFromFile(std::string const& filepath) {
		std::string json_from_file = Serializer::GetFileContents(filepath.c_str());

		rapidjson::Document document;
		if (document.Parse(json_from_file.c_str()).HasParseError()) {
			// I put filepath into std::string, so that I can append new strings or char pointers via add operator
			throw SimpleException(filepath + " has an incorrect JSON format");
		}

		// the root must be a JSON object
		if (!document.IsObject())
			throw SimpleException(filepath + " must start with a JSON object");

		return DeserializeEntity(filepath, document);
	}
}