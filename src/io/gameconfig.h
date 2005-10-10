/** \file gameconfig.h
 ** \brief Defines the Config class
 **/

#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <string>
#include <map>

/// Reads from a configuration file and allows access to the options in the file
class Config {
public:
	typedef std::map<std::string, std::string> param_list;

	// Constructor
	Config();

	// Destructor
	~Config();

	void read_config();

	int get_int(std::string param);
	bool get_bool(std::string param);
	std::string get_string(std::string param);

	static Config* get_ptr();

private:
	/// Stores paramaters read from configuration file
	param_list m_params;
};

#endif // GAMECONFIG_H
