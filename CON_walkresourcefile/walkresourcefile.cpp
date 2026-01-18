// walkresourcefile.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// walk through the entries of a binary resource file
// see Resource file formats
//    URL: https://learn.microsoft.com/en-us/windows/win32/menurc/resource-file-formats
//
// A binary resource file consists of a number of concatenated resource entries. Each entry consists of a resource header
// and the data for that resource. A resource header is DWORD-aligned in the file and consists of the following:
//
//     A DWORD that contains the size of the resource header
//     A DWORD that contains the size of the resource data
//     The resource type
//     The resource name
//     Additional resource information
// The RESOURCEHEADER structure describes the format of this header.The data for the resource follows the resource header
// and is specific to each type of resource.Some resources also employ a resource - specific group header structure to
// provide information about a group of resources.


#include <Windows.h>
#include <WinUser.h>

#include <string.h>
#include <stdio.h>

// following strut more of a guideline than an actual rule. The first three
// members are 
typedef struct {
    DWORD DataSize;        // size of the data block following the header in bytes
    DWORD HeaderSize;      // size of the header in bytes
    DWORD TYPE;            // indicates the type of data. see MAKEINTRESOURCE() 
    DWORD NAME;            // contains the name. amy be integer constant or a wchar_t string of 10 characters.
    DWORD DataVersion;
    WORD  MemoryFlags;
    WORD  LanguageId;
    DWORD Version;
    DWORD Characteristics;
} RESOURCEHEADER;

typedef struct {
    DWORD NAME;            // contains the name. amy be integer constant or a wchar_t string of 10 characters.
    DWORD DataVersion;
    WORD  MemoryFlags;
    WORD  LanguageId;
    DWORD Version;
    DWORD Characteristics;
}RESOURCEHEADER1;

typedef struct {
    DWORD DataVersion;
    WORD  MemoryFlags;
    WORD  LanguageId;
    DWORD Version;
    DWORD Characteristics;
}RESOURCEHEADER2;

int main()
{
    // this Visual Studio console project contains a resource file that is
    // compiled resulting in the binary resource file walkresourcefile.res
    // which is included for testing purposes. Since we just run this app in
    // the debugger of Visual Studio, we will just access that file which is
    // in the Debug folder of our solution directory where all the build products
    // end up when compiling in Debug mode.
    char sFileName[256] = { "Debug\\walkresourcefile.res"};

    RESOURCEHEADER    resourceHeader = { 0 };
    FILE *fStream;
    errno_t  myError;
    long lOffset = 0;

    ULONG ulTest = (ULONG)RT_STRING;

    // create an array of labels for the standard resource types and
    // populate the array with the labels. We are using the values of
    // the standard types as the array index to make lookup easy.
    const char* pStrings[20] = { 0 };
    pStrings[(ULONG)RT_STRING] = "RT_STRING";
    pStrings[(ULONG)RT_DIALOG] = "RT_DIALOG";
    pStrings[(ULONG)RT_BITMAP] = "RT_BITMAP";
    pStrings[(ULONG)RT_FONT] = "RT_FONT";
    pStrings[(ULONG)RT_CURSOR] = "RT_CURSOR";
    pStrings[(ULONG)RT_GROUP_CURSOR] = "RT_GROUP_CURSOR";
    pStrings[(ULONG)RT_MENU] = "RT_MENU";
    pStrings[(ULONG)RT_ICON] = "RT_ICON";
    pStrings[(ULONG)RT_GROUP_ICON] = "RT_GROUP_ICON";
    pStrings[(ULONG)RT_RCDATA] = "RT_RCDATA";
    pStrings[(ULONG)RT_VERSION] = "RT_VERSION";

    myError = fopen_s(&fStream, sFileName, "rb");

    for (int i = 0; i < 3000; i++) {    // we'll just put an arbitrary though large value on the amount of output.

        fseek(fStream, lOffset, SEEK_SET);
        if (fread(&resourceHeader, sizeof(resourceHeader), 1, fStream) == 0) break;

        printf(" lOffset = %d: ", lOffset);

        // A variable type member is called a Name or Ordinal member, and it is used
        // in most places in the resource file where an identifier appears. The first
        // WORD of a Name or Ordinal type member indicates whether the member is a
        // numeric value or a string. If the first WORD in the member is equal to the
        // value 0xffff, which is an invalid Unicode character, then the following WORD
        // is a type number. Otherwise, the member contains a Unicode string and the
        // first WORD in the member is the first character in the name string. For
        // additional information about resource definition statements, see Resource-Definition Statements.
        //
        // It appears from testing with this application that there is a max length when both TYPE and NAME
        // are both strings. They share this max length so as TYPE gets longer then NAME gets shorter.
        ULONG ulTest = 0;
        if (LOWORD(resourceHeader.TYPE) == 0xFFFF) {
            ulTest = HIWORD(resourceHeader.TYPE);
            if (ulTest < sizeof(pStrings) / sizeof(pStrings[0]) && pStrings[ulTest]) {
                printf("%s ", pStrings[ulTest]);
            }
 
            if ((LOWORD(resourceHeader.NAME) != 0xFFFF)) {
                wchar_t nameLabel[16] = { 0 };
                for (int i = 0; i < 10; i++) {      // from testing, it appears that ten characters is the max length
                    nameLabel[i] = ((wchar_t*)&resourceHeader.NAME)[i];
                }
                printf(" DataSize = %ul   HeaderSize = %ul, Type = %ul 0x%x, Name = %d 0x%x  %S \n", resourceHeader.DataSize, resourceHeader.HeaderSize, resourceHeader.TYPE, resourceHeader.TYPE, HIWORD(resourceHeader.NAME), LOWORD(resourceHeader.NAME), nameLabel);
            }
            else {
                printf(" DataSize = %ul   HeaderSize = %ul, Type = %ul 0x%x, Name = %d %d/0x%x \n", resourceHeader.DataSize, resourceHeader.HeaderSize, resourceHeader.TYPE, resourceHeader.TYPE, HIWORD(resourceHeader.NAME), LOWORD(resourceHeader.NAME), LOWORD(resourceHeader.NAME));
            }
        }
        else {
            // TYPE is a string and not a constant integer value
            wchar_t* pType = (wchar_t*)&resourceHeader.TYPE;
            size_t lTypeLen = wcslen(pType);

            RESOURCEHEADER1 * pResource1 = (RESOURCEHEADER1 *)(pType + lTypeLen + 1);
            wchar_t* pName = (wchar_t*)&pResource1->NAME;

            printf(" %S ", (wchar_t*)&resourceHeader.TYPE);
            printf(" DataSize = %ul   HeaderSize = %ul, ", resourceHeader.DataSize, resourceHeader.HeaderSize);
            if ((LOWORD(resourceHeader.NAME) != 0xFFFF)) {
                wchar_t nameLabel[16] = { 0 };
                for (int i = 0; i < 10; i++) {      // from testing, it appears that ten characters is the max length
                    if (pName[i] > 0xff) break;
                    nameLabel[i] = pName[i];
                }
                printf("Name = % d 0x % x % S \n", HIWORD(pResource1->NAME), LOWORD(pResource1->NAME), nameLabel);
            }
            else {
                printf("Name = %d %d/0x%x \n", HIWORD(resourceHeader.NAME), LOWORD(resourceHeader.NAME), LOWORD(resourceHeader.NAME));
            }
        }

        if (ulTest == (ULONG)RT_STRING) {
            wchar_t   bundleBuff[4096] = { 0 };    // large buffer sized for 16 strings of 256 characters.
            ULONG ulStrBundle = lOffset + resourceHeader.HeaderSize;         // file offset to beginning of string bundle

            // seek to where the string bundle begins in the file then read the string bundle in
            // and then print the individual strings. strings have length in first entry
            fseek(fStream, ulStrBundle, SEEK_SET);             
            if (fread(bundleBuff, resourceHeader.DataSize, 1, fStream) != 0) {
                LPWSTR pwsz = bundleBuff;
                for (int i = 0; i < 16; i++) {
                    if (*pwsz > 0) {
                        // replace the character after the end of the sring with a
                        // binary zero string terminator, print the string, then put back.
                        wchar_t temp = *(pwsz + *pwsz + 1);
                        *(pwsz + *pwsz + 1) = 0;
                        printf("    %d: %S\n", i, pwsz + 1);
                        *(pwsz + *pwsz + 1) = temp;
                    }
                    pwsz += 1 + *pwsz;
                }
            }
        }

        lOffset = resourceHeader.HeaderSize + resourceHeader.DataSize + lOffset;    // compute file offset to the next resource in the file
        if ((lOffset % 4) != 0) lOffset += 2;     // make sure the file offset is on a DWORD boundary.
    }
    fclose(fStream);

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
