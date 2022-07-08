#pragma once
#include "PCH.h"
namespace Utils
{

	
}


/// <summary>
/// Helper class to load a bunch of forms from a single plugin without passing the plugin name as an argument every time.
/// </summary>
class formLoader
{
private:
	RE::BSFixedString _pluginName;
	RE::TESDataHandler* _dataHandler;
	int _loadedForms;

public:
	formLoader(RE::BSFixedString pluginName) {
		_pluginName = pluginName;
		_dataHandler = RE::TESDataHandler::GetSingleton();
		if (!_dataHandler) {
			logger::critical("Error: TESDataHandler not found.");
		}
		if (!_dataHandler->LookupModByName(pluginName)) {
			logger::critical("Error: {} not loaded.", pluginName);
		}
		logger::info("Loading from plugin {}...", pluginName);
	}

	void printReport() {
		logger::info("Loaded {} forms from {}", _loadedForms, _pluginName);
	}

	template <class formType>
	void load(formType*& formRet, RE::FormID formID)
	{
		formRet = _dataHandler->LookupForm<formType>(formID, _pluginName);
		if (!formRet) {
			logger::critical("Error: null formID or wrong form type when loading {} from {}", formID, _pluginName);
		}
		_loadedForms++;
	}


};
