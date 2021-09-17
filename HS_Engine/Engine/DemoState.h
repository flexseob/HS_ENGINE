/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Semester: Spring 2021
Project: CS250
Primary Author: Hoseob Jeong
-----------------------------------------------------------------*/

#pragma once
#include <iostream>
#include <string>
#include <GL\glew.h>

namespace EndGame_Engine
{
	class DemoState
	{
	public:
		virtual void Load() {};
		virtual void Update([[maybe_unused]]double dt) {};
		virtual std::string GetCurrentDemoName() { return "Null demo"; };
		virtual void UnLoad() { std::cout << "Unload : " << GetCurrentDemoName() << std::endl; };
		void ErrorCheck(){
			
				GLenum errorCode = glGetError();

				if (errorCode == GL_NO_ERROR)
					return;

				std::string error = "Unknown error";
				std::string description = "No description";

				switch (errorCode)
				{
				case GL_INVALID_ENUM:
				{
					error = "GL_INVALID_ENUM";
					description = "An unacceptable value has been specified for an enumerated argument.";
					break;
				}

				case GL_INVALID_VALUE:
				{
					error = "GL_INVALID_VALUE";
					description = "A numeric argument is out of range.";
					break;
				}

				case GL_INVALID_OPERATION:
				{
					error = "GL_INVALID_OPERATION";
					description = "The specified operation is not allowed in the current state.";
					break;
				}

				case GL_STACK_OVERFLOW:
				{
					error = "GL_STACK_OVERFLOW";
					description = "This command would cause a stack overflow.";
					break;
				}

				case GL_STACK_UNDERFLOW:
				{
					error = "GL_STACK_UNDERFLOW";
					description = "This command would cause a stack underflow.";
					break;
				}

				case GL_OUT_OF_MEMORY:
				{
					error = "GL_OUT_OF_MEMORY";
					description = "There is not enough memory left to execute the command.";
					break;
				}

				default:
				{
					error += " " + std::to_string(errorCode);
					break;
				}
				}

				std::cerr << GetCurrentDemoName() <<" : " << error << "\n   " << description << "\n\n";

			
		}
	private:

	};
}