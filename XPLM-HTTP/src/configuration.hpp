#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Configuration {
	std::string pluginName;
	std::string pluginSignature;
	std::string pluginDescription;

	std::string bind_url;
	int bind_port;

	bool enable_crossOrigin = false;

	std::unordered_map<std::string, std::string> headers = {};

	std::unordered_map<std::string, std::string> static_files = {};


	Configuration(std::string const& filepath)
	{
		std::ifstream file(filepath);
		if (!file.is_open()) {
			return;
		}
		json j;
		file >> j;

		if (j.contains("plugin"))
		{
			auto plugin = j["plugin"];
			pluginName = plugin.value("name", "XPLM_HTTP");
			pluginSignature = plugin.value("signature", "rdessart.network.http");
			pluginDescription = plugin.value("description", "HTTP SERVER");
		}

		if (j.contains("endpoint"))
		{
			auto server = j["endpoint"];
			bind_url = server.value("url", "127.0.0.1");
			bind_port = server.value("port", 28080);
		}
		
		if (j.contains("cors"))
		{
			enable_crossOrigin = j["cors"].value("enable", false);
			if (enable_crossOrigin)
			{
				for (auto& [key, value] : j["cors"]["data"].items())
				{
					headers[key] = value.get<std::string>();
				}
			}
		}

		if (j.contains("static_files"))
		{
			auto files = j["static_files"];
			for (auto& value : files)
			{
				static_files[value["path"]] = value["url"].get<std::string>();
			}
		}

		file.close();
	}
};