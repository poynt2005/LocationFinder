#include <LocationFinder.h>
#include <stdio.h>

int main()
{
    InstallPath installPath;
    installPath.type = NODE_JS;

    if (GetInstallPath(&installPath))
    {
        for (int i = 0; i < installPath.versions; i++)
        {
            printf("%s\n", installPath.path[i]);
        }
    }
    else
    {
        printf("Cannot found\n");
    }

    ReleaseInstallPath(&installPath);

    printf("Search finished\n");

    return 0;
}