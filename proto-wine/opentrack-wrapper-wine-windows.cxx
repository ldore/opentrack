#ifndef __WIN32
#   error "bad cross"
#endif

#define SHM_TYPE_NAME shm_impl_winelib
#define SHM_FUN_PREFIX shm_impl_winelib_
#define SHMXX_TYPE_NAME mem_winelib
#include "compat/shm.cpp"

#include "wine-shm.h"
#include "compat/library-path.hpp"

#include <cstring>

#include <windows.h>
#include <winreg.h>

static void write_path(const char* key, const char* subkey)
{
    char dir[8192];
    dir[sizeof(dir)-1] = '\0';

    if (GetCurrentDirectoryA(8192, dir) < 8190)
    {
        HKEY hkpath;
        if (RegCreateKeyExA(HKEY_CURRENT_USER,
                            key,
                            0,
                            NULL,
                            0,
                            KEY_ALL_ACCESS,
                            NULL,
                            &hkpath,
                            NULL) == ERROR_SUCCESS)
        {
            for (int i = 0; dir[i]; i++)
                if (dir[i] == '\\')
                    dir[i] = '/';
            // there's always a leading and trailing slash
            strcat(dir, OPENTRACK_LIBRARY_PATH);
            //strcat(dir, "/");
            (void) RegSetValueExA(hkpath, subkey, 0, REG_SZ, (BYTE*) dir, strlen(dir) + 1);
            RegCloseKey(hkpath);
        }
    }
}

void create_registry_key(void) {
    write_path("Software\\NaturalPoint\\NATURALPOINT\\NPClient Location", "Path");
    write_path("Software\\Freetrack\\FreeTrackClient", "Path");
}
