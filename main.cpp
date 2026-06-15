// main.cpp
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

int fileExists(const char* path) {
    FILE* f = fopen(path, "rb");
    if (f) { fclose(f); return 1; }
    return 0;
}

void getOutputFileName(const char* inputPath, const char* newExt, char* outPath, int maxLen) {
    char drive[4] = "", dir[256] = "", name[256] = "", ext[10] = "";
    _splitpath(inputPath, drive, dir, name, ext);
    _makepath(outPath, drive, dir, name, newExt);
    if (!fileExists(outPath)) return;

    int idx = 1;
    char temp[256];
    while (1) {
        sprintf(temp, "%s_%d", name, idx);
        _makepath(outPath, drive, dir, temp, newExt);
        if (!fileExists(outPath)) break;
        idx++;
    }
}

int main(int argc, char* argv[]) {
	SetConsoleTitle("ChiliEdit v1.00");
	printf("ChiliEdit v1.00 (build #0001)\nMade by ColorProgrammy & Rus Joy\n\nA utility that helps easily edit userdata.ngh in Neighbours from Hell\n\n");
    if (argc != 2) {
		printf("Usage: drag and drop a .ngh or .xml file onto this executable.\n");
        system("pause");
        return 1;
    }

	

    const char* inputFile = argv[1];
    const char* dot = strrchr(inputFile, '.');
    if (!dot) {
        printf("Oh noes! Input file has no extension.\n");
        system("pause");
        return 1;
    }

    char inputExt[10];
    strcpy(inputExt, dot + 1);
    for (int i = 0; inputExt[i]; i++) inputExt[i] = tolower(inputExt[i]);

    const char* outputExt = NULL;
    if (strcmp(inputExt, "ngh") == 0) outputExt = "xml";
    else if (strcmp(inputExt, "xml") == 0) outputExt = "ngh";
    else {
        printf("Oh noes! Unsupported extension '%s'.\n", inputExt);
        system("pause");
        return 1;
    }

    char outputFile[256];
    getOutputFileName(inputFile, outputExt, outputFile, 256);

    FILE* fin = fopen(inputFile, "rb");
    if (!fin) {
        printf("Oops! Cannot open input file for some reason.\n");
        system("pause");
        return 1;
    }

    FILE* fout = fopen(outputFile, "wb");
    if (!fout) {
        printf("Oops! Cannot create output file for some reason.\n");
        fclose(fin);
        system("pause");
        return 1;
    }

    const unsigned char key = 0x2F;
    unsigned char buf[4096];
    size_t bytes;
    while ((bytes = fread(buf, 1, sizeof(buf), fin)) > 0) {
        for (size_t i = 0; i < bytes; i++) buf[i] ^= key;
        fwrite(buf, 1, bytes, fout);
    }

    fclose(fin);
    fclose(fout);
    printf("Well done! The file was saved at %s\n.", outputFile);
    system("pause");
    return 0;
}
