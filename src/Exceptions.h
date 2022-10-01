#pragma once
#include <exception>
#include <string>

namespace OpenGLFun {
	class SimpleException : public std::exception {
	protected:
		std::string _message;
	public:
		SimpleException(std::string message) : _message(message) {}

		virtual const char* what() const noexcept {
			return _message.c_str();
		}
	};

	// To simplify writing the message string for json read errors
	class JsonReadException : public SimpleException {
	public:
		/**
		 * @brief Takes in 4 parameters
		 * @param filename The filename of the json parsing, or the parent property
		 * @param propertyOwner Parent property of the property you are parsing
		 * @param propertyName The key of the property you are parsing
		 * @param propertyJsonType The value type of the property you are parsing
		*/
		JsonReadException(std::string filename, std::string propertyOwner, std::string propertyName, std::string propertyJsonType) : SimpleException("In ") {
			_message += filename + ", " + propertyOwner + "'s \"" + propertyName + "\" must have a value of a" + propertyJsonType;
		}

		JsonReadException(std::string filename, std::string propertyName, std::string propertyJsonType) : SimpleException("In ") {
			_message += filename + ", \"" + propertyName + "\" must have a value of a" + propertyJsonType;
		}
	};
}