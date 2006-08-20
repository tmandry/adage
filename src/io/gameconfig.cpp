/** \file gameconfig.cpp
 ** \brief Contains the source code for the Config class
 **/

#include "config.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>

#include "gameconfig.h"

Config::Config()
{
	m_params["screenw"] = "640";
	m_params["screenh"] = "480";
	m_params["bpp"] = "32";
	m_params["fullscreen"] = "FALSE";
}

Config::~Config()
{
}


void Config::read_config()
{
}


int Config::get_int(std::string param)
{
	param_list::iterator i = m_params.find(param);
	if (i == m_params.end()) return 0;

	std::stringstream ss(i->second);

	int ret;
	
	ss >> ret;
	return ret;
}

bool Config::get_bool(std::string param)
{
	param_list::iterator i = m_params.find(param);
	if (i == m_params.end()) return false;
	
	if (i->second.compare(std::string("TRUE")) == 0) return true;
	if (i->second.compare(std::string("FALSE")) == 0) return false;
	return get_int(param);
}

std::string Config::get_string(std::string param)
{
	param_list::iterator i = m_params.find(param);
	if (i == m_params.end()) return false;

	return i->second;
}


Config* Config::get_ptr()
{
	static Config config;
	return &config;
}
