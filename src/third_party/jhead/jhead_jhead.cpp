//--------------------------------------------------------------------------
// Program to pull the information out of various types of EXIF digital 
// camera files and show it in a reasonably consistent way
//
// Version 2.97
//
// Compiling under Windows:  
//   Make sure you have Microsoft's compiler on the path, then run make.bat
//
// Dec 1999 - Jan 2013
//
// by Matthias Wandel   www.sentex.net/~mwandel
//--------------------------------------------------------------------------
#ifdef _WIN32
    #include <io.h>
#endif

#include "jhead.h"

namespace jhead {

#include <sys/stat.h>

#define JHEAD_VERSION "2.97"

// This #define turns on features that are too very specific to 
// how I organize my photos.  Best to ignore everything inside #ifdef MATTHIAS
//#define MATTHIAS


// Bitmasks for DoModify:
#define MODIFY_ANY  1
#define READ_ANY    2
#define JPEGS_ONLY  4
#define MODIFY_JPEG 5
#define READ_JPEG   6

static const char * CurrentFile;

#ifdef _WIN32
static int RenameAssociatedFiles = FALSE;
#endif
       int ShowTags     = FALSE;    // Do not show raw by default.
       int DumpExifMap  = FALSE;

static int SupressNonFatalErrors = FALSE; // Wether or not to pint warnings on recoverable errors

//--------------------------------------------------------------------------
// Error exit handler
//--------------------------------------------------------------------------
void ErrFatal(const char * msg)
{
    fprintf(stderr,"\nError : %s\n", msg);
    if (CurrentFile) fprintf(stderr,"in file '%s'\n",CurrentFile);
	// exit(EXIT_FAILURE);
} 

//--------------------------------------------------------------------------
// Report non fatal errors.  Now that microsoft.net modifies exif headers,
// there's corrupted ones, and there could be more in the future.
//--------------------------------------------------------------------------
void ErrNonfatal(const char * msg, int a1, int a2)
{
    if (SupressNonFatalErrors) return;

    fprintf(stderr,"\nNonfatal Error : ");
    if (CurrentFile) fprintf(stderr,"'%s' ",CurrentFile);
    fprintf(stderr, msg, a1, a2);
    fprintf(stderr, "\n");
}

//--------------------------------------------------------------------------
// Set file time as exif time.
//--------------------------------------------------------------------------
void FileTimeAsString(char * TimeStr)
{
    struct tm ts;
    ts = *localtime(&ImageInfo.FileDateTime);
    strftime(TimeStr, 20, "%Y:%m:%d %H:%M:%S", &ts);
}

}


