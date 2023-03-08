#pragma once

#ifdef BUILDLOCATIONFINDER
#define EXPORTLOCATIONFINDER __declspec(dllexport)
#else
#define EXPORTLOCATIONFINDER __declspec(dllimport)
#endif

#define VISUAL_STUDIO 0x01
#define PYTHON 0x02
#define NODE_JS 0x03

typedef struct _InstallPath
{
	char **path;
	int versions;
	int type;
} InstallPath;

#ifdef __cplusplus
extern "C"
{
#endif
	EXPORTLOCATIONFINDER int GetInstallPath(InstallPath *);
	EXPORTLOCATIONFINDER void ReleaseInstallPath(InstallPath *);
#ifdef __cplusplus
}
#endif
