#pragma once

#include <sstream>
#include <fstream>

struct R_String {
	std::stringstream S_String;

	R_String() {
		std::stringstream S_String;
	}

	R_String& nl(uint8_t Write_Tab = 0, uint8_t Local_Tab = 0, uint8_t Lines = 1) {
		while (Lines--) {
			S_String << "\n";
		}
		while (Write_Tab--) {
			S_String << "\t";
		}
		while (Local_Tab--) {
			S_String << "\t";
		}
		return *this;
	}

	R_String& operator<< (std::stringstream& Input) {
		S_String << Input.str();
		return *this;
	}

	R_String& operator<< (bool Input) {
		if (Input == true) {
			S_String << "On";
		}
		else {
			S_String << "Off";
		}
		return *this;
	}

	R_String& operator<< (float Input) {
		S_String << Input;
		return *this;
	}

	R_String& operator<< (std::string Input) {
		S_String << Input;
		return *this;
	}

	R_String& operator<< (const char* Input) {
		S_String << Input;
		return *this;
	}

	R_String& operator<< (int Input) {
		S_String << Input;
		return *this;
	}

	R_String& operator<< (long int Input) {
		S_String << Input;
		return *this;
	}

	R_String& operator<< (long long int Input) {
		S_String << Input;
		return *this;
	}

	R_String& operator<< (long double Input) {
		S_String << Input;
		return *this;
	}

	R_String& operator<< (unsigned long long int Input) {
		S_String << Input;
		return *this;
	}

	std::string write() {
		std::string string = S_String.str();
		return string;
	}
};