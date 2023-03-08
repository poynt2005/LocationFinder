#include "LocationFinder.h"

#include <filesystem>
#include <windows.h>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <windows.h>
#include <iostream>
#include <string.h>


bool SystemStrToStdStr(System::String^% inStr, std::string& outStr) {
	auto csPtr = System::Runtime::InteropServices::Marshal::StringToHGlobalUni(inStr);

	const wchar_t* wchars = reinterpret_cast<const wchar_t*>(csPtr.ToPointer());

	DWORD size = WideCharToMultiByte(
		CP_UTF8,
		0,
		wchars,
		-1,
		nullptr,
		0,
		nullptr,
		nullptr
	);

	if (size == 0) {
		return false;
	}

	auto chars = std::make_unique<char[]>(size);

	DWORD result = WideCharToMultiByte(
		CP_UTF8,
		0,
		wchars,
		-1,
		chars.get(),
		size,
		nullptr,
		nullptr
	);

	if (result == 0) {
		return false;
	}

	outStr = std::string(chars.get());

	System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr(reinterpret_cast<void*>(const_cast<wchar_t*>(wchars))));

	return true;
}

bool GetParentPath(const std::string& inStr, std::string& outStr) {
	namespace fs = std::filesystem;

	auto parentPath = fs::path(inStr).parent_path();

	if (fs::exists(parentPath)) {
		outStr = parentPath.string();
		return true;
	}
	return false;
}

bool WmiQuery(const std::string& queryString, std::vector<std::map<std::string, std::string>>& outResult) {
	System::String^ csQueryString = gcnew System::String(queryString.data());

	System::Management::EnumerationOptions^ options = gcnew System::Management::EnumerationOptions();

	options->Rewindable = false;
	options->ReturnImmediately = true;


	System::Management::ManagementObjectSearcher^ searcher = gcnew System::Management::ManagementObjectSearcher("root\\cimv2", csQueryString, options);

	auto processes = searcher->Get();

	for each (System::Management::ManagementObject ^ process in processes)
	{
		std::map<std::string, std::string> tmpMap;
		for each (System::Management::PropertyData ^ prop in process->Properties) {
			if (System::String::Compare(prop->Type.ToString(), "String") == 0 && prop->Value) {
				std::string k;
				std::string v;
				if (SystemStrToStdStr(prop->Name, k) && SystemStrToStdStr(prop->Value->ToString(), v)) {
					tmpMap.insert(std::pair<std::string, std::string>(k, v));
				}
			}
		}
		outResult.emplace_back(std::move(tmpMap));
	}
	return true;
}

bool GetVisualStudioInstallPath(char*** outPath, int* allVersions) {
	if (outPath == NULL) {
		return false;
	}

	std::vector<std::map<std::string, std::string>> versions;
	if (WmiQuery("SELECT * FROM MSFT_VSInstance", versions)) {
		*allVersions = versions.size();
		*outPath = new char* [*allVersions];
		for (int i = 0; i < *allVersions; i++) {
			for (auto&& entry : versions[i]) {
				if (entry.first == "InstallLocation") {
					(*outPath)[i] = new char[entry.second.length() + 1];
					strcpy((*outPath)[i], entry.second.data());
				}
			}
		}
		return true;
	}
	return false;
}

bool GetPythonInstallPath(char*** outPath, int* allVersions) {
	if (outPath == NULL) {
		return false;
	}

	Microsoft::Win32::RegistryKey^ regKeyRoot = Microsoft::Win32::Registry::CurrentUser;
	Microsoft::Win32::RegistryKey^ regKeyRootVal = regKeyRoot->OpenSubKey("SOFTWARE\\Python\\PythonCore\\");

	std::vector<std::string> paths;
	for each (System::String ^ name in regKeyRootVal->GetSubKeyNames())
	{
		Microsoft::Win32::RegistryKey^ regKey = regKeyRootVal->OpenSubKey(name);

		if (regKey != nullptr) {
			auto pythonExePath = regKey->OpenSubKey("InstallPath")->GetValue("ExecutablePath")->ToString();
			std::string cppPythonExePath;
			if (pythonExePath != nullptr && pythonExePath->Length != 0 && SystemStrToStdStr(pythonExePath, cppPythonExePath)) {
				paths.emplace_back(std::move(cppPythonExePath));
			}
		}
	}

	*allVersions = paths.size();
	*outPath = new char* [*allVersions];

	for (int i = 0; i < paths.size(); i++) {
		std::string dirPath;
		if (GetParentPath(paths[i].data(), dirPath)) {
			(*outPath)[i] = new char[dirPath.size() + 1];
			strcpy((*outPath)[i], dirPath.data());
		}
	}

	return true;
}

bool GetNodeJSInstallPath(char*** outPath, int* allVersion) {
	if (outPath == NULL) {
		return false;
	}

	try {
		auto process = gcnew System::Diagnostics::Process();
		auto startInfo = gcnew System::Diagnostics::ProcessStartInfo();
		startInfo->FileName = "node";
		startInfo->Arguments = "-p \"process.argv[0]\"";
		startInfo->RedirectStandardError = true;
		startInfo->RedirectStandardOutput = true;
		startInfo->CreateNoWindow = true;
		startInfo->UseShellExecute = false;
		process->StartInfo = startInfo;
		process->Start();
		process->WaitForExit();

		auto output = process->StandardOutput->ReadToEnd();
		std::string ccStr;

		if (SystemStrToStdStr(output, ccStr)) {
			std::string dirPath;
			if (GetParentPath(ccStr, dirPath)) {
				*allVersion = 1;
				*outPath = new char* [1];
				(*outPath)[0] = new char[dirPath.length() + 1];
				strcpy((*outPath)[0], dirPath.data());
				return true;
			}
			return false;
		}
		return false;
	}
	catch (System::Exception^ ex) {
		return false;
	}

	return false;
}

int GetInstallPath(InstallPath* installPath) {
	bool result;

	installPath->path = NULL;
	installPath->versions = 0;

	switch (installPath->type) {
	case VISUAL_STUDIO:
		result = GetVisualStudioInstallPath(&(installPath->path), &(installPath->versions));
		break;
	case PYTHON:
		result = GetPythonInstallPath(&(installPath->path), &(installPath->versions));
		break;
	case NODE_JS:
		result = GetNodeJSInstallPath(&(installPath->path), &(installPath->versions));
		break;
	default:
		result = false;
		break;
	}
	return static_cast<int>(result);
}

void ReleaseInstallPath(InstallPath* installPath) {
	int allVersions = installPath->versions;
	for (int i = 0; i < allVersions; i++) {
		delete[] installPath->path[i];
	}
	delete[] installPath->path;
}

