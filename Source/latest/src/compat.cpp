#ifdef WIN32

#include <windows.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <malloc.h>
#include "compat.h"

struct DIR
{
  HANDLE hFind;
  char   szDirName[1];
};

static int countslashes(const char *dirname)
{
  const char *p;
  int n;

  n = 0;
  p = dirname;

  while (*p)
    if (*p++ == '\\')
      ++n;

  return n;
}

DIR * opendir ( const char * dirname )
{
  DIR * dir;
  int   nameLen;
  struct stat st;
  unsigned char flagNetPath;
  unsigned char flagRootOnly;

  if (dirname == NULL || *dirname == 0)
  {
    errno = EINVAL;
    return NULL;
  }

  nameLen = strlen( dirname );
  flagNetPath = 0;
  if (dirname[0] == '\\' && dirname[1] == '\\')
    flagNetPath = 1;
  /* we have to check for root-dir-only case */
  flagRootOnly = 0;
  if (flagNetPath)
  {
    if (countslashes(&dirname[2]) == 2)  /* only the separation for server_name and the root*/
      flagRootOnly = 1;
  }

  if ((dirname[nameLen-1] == '/' || dirname[nameLen-1] == '\\') &&
      (nameLen != 3 || dirname[1] != ':') && nameLen != 1 && !flagRootOnly)
  {
    char * t = (char*)alloca( nameLen );
    memcpy( t, dirname, nameLen );
    t[nameLen-1] = 0;
    dirname = t;
    --nameLen;
  }

  if (stat( dirname, &st ))
    return NULL;

  if ((st.st_mode & S_IFDIR) == 0)
  {
    // this is not a DIR
    errno = ENOTDIR;
    return NULL;
  }

  if ((dir = (DIR*)malloc( sizeof( DIR ) + nameLen + 2 )) == NULL)
  {
    errno = ENOMEM;
    return NULL;
  }

  dir->hFind = INVALID_HANDLE_VALUE;

  memcpy( dir->szDirName, dirname, nameLen );
  if (nameLen && dirname[nameLen-1] != ':' && dirname[nameLen-1] != '\\' &&
      dirname[nameLen-1] != '/')
  {
    dir->szDirName[nameLen++] = '\\';
  }
  dir->szDirName[nameLen] = '*';
  dir->szDirName[nameLen+1] = 0;

  return dir;
};

struct dirent * readdir ( DIR * dir )
{
  static WIN32_FIND_DATAA fData;

  if (dir == NULL)
  {
    errno = EBADF;
    return NULL;
  }

  do
  {
    int ok = 1;

    if (dir->hFind == INVALID_HANDLE_VALUE)
    {
      dir->hFind = FindFirstFileA( dir->szDirName, &fData );
      if (dir->hFind == INVALID_HANDLE_VALUE)
        ok = 0;
    }
    else
    if (!FindNextFileA( dir->hFind, &fData ))
      ok = 0;

    if (!ok)
    {
      switch (GetLastError())
      {
        case ERROR_NO_MORE_FILES:
        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
          errno = ENOENT;
          break;

        case ERROR_NOT_ENOUGH_MEMORY:
          errno = ENOMEM;
          break;

        default:
          errno = EINVAL;
          break;
      }
      return NULL;
    }
  }
  while (fData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);

  return (struct dirent *)&fData.cFileName;
};

int closedir ( DIR * dir )
{
  if (dir == NULL)
  {
    errno = EBADF;
    return -1;
  }
  if (dir->hFind != INVALID_HANDLE_VALUE)
    FindClose( dir->hFind );
  free( dir );
  return 0;
};

void rewinddir ( DIR * dir )
{
  if (dir)
  {
    if (dir->hFind != INVALID_HANDLE_VALUE)
      FindClose( dir->hFind );
    dir->hFind = INVALID_HANDLE_VALUE;
  }
};

void win32_display_error(const char* caption, const char* message)
{
	::MessageBoxA(NULL, message, caption, MB_ICONERROR|MB_OK);
}

int ClipboardGet( char *buffer, size_t size ) {
	HGLOBAL text;
	void *ptr;

	if( !IsClipboardFormatAvailable( CF_TEXT ) ) {
		*buffer = 0; 
		return 1;
	}
	if( !OpenClipboard( GetClipboardOwner() ) ) return 0; 
	if(	!( text = GetClipboardData( CF_TEXT ) ) ||
		!( ptr = GlobalLock( text ) )
	) {
		CloseClipboard();
		return 0;
	}
	strncpy( buffer, (const char*)ptr, size - 1 );
	buffer[size - 1] = 0;
	GlobalUnlock( text );
	CloseClipboard();
	return 1;
}

int ClipboardSet( const char *text ) {
	size_t len;
	HGLOBAL buffer;
	void *ptr;
	
	if( !OpenClipboard( 0 ) ) return 0;
	if( !EmptyClipboard() ) goto _ClipboardSet_Error1;
	len = strlen( text ) + 1;
	if( !( buffer = GlobalAlloc( GMEM_MOVEABLE, len ) ) ) goto _ClipboardSet_Error1;
	if( !( ptr = GlobalLock( buffer ) ) ) goto _ClipboardSet_Error2;
	memcpy( ptr, text, len );
	GlobalUnlock( buffer );
	if( !SetClipboardData( CF_TEXT, buffer ) ) goto _ClipboardSet_Error2;
	CloseClipboard();
	return 1;

_ClipboardSet_Error2:
	GlobalFree( buffer );
_ClipboardSet_Error1:
	CloseClipboard();
	return 0;
}



#else

#include <stddef.h>

int ClipboardSet( const char *text ) {
	return 1;
}

int ClipboardGet( char *buffer, size_t size ) {
	*buffer = 0;
	return 1;
}

#endif
