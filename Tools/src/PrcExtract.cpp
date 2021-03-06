/* This file is part of HSPlasma.
 *
 * HSPlasma is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HSPlasma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HSPlasma.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ResManager/plResManager.h>
#include <Debug/hsExceptions.hpp>
#include <Stream/hsStdioStream.h>
#include <Debug/plDebug.h>
#include <string.h>
#include <time.h>
#ifdef WIN32
  #include <windows.h>
  #define SLASH '\\'
  #define SLASH_S "\\"
#else
  #include <unistd.h>
  #define SLASH '/'
  #define  SLASH_S "/"
#endif
#include <sys/stat.h>

void doHelp() {
    printf("Usage: PrcExtract [options] filename.prp\n\n");
    printf("Objects are written to Age_PRC\\filename.prc\n\n");
    printf("options:\n");
    printf("    -vtx   include vertex data\n");
    printf("    -tex   include texture data\n");
    printf("    -help  display this help message\n\n");
}

plString filenameConvert(const char* filename) {
    plString name = filename;
    if (name.rfind('.') >= 0)
        name = name.left(name.rfind('.')) + ".prc";
    else
        name += ".prc";
    if (name.rfind(SLASH) >= 0)
        return name.mid(name.rfind(SLASH) + 1);
    else
        return name;
}

plString getOutputDir(const char* filename, plPageInfo* page) {
    plString name = filename;
    if (name.rfind(SLASH) >= 0)
        name = name.left(name.rfind(SLASH) + 1);
    else
        name = "";
    return name + page->getAge() + "_PRC" SLASH_S;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        doHelp();
        return 0;
    }

    std::vector<plString> fFiles;

    plDebug::Init(plDebug::kDLAll);
    plResManager rm;
    plPageInfo* page;
    plString outDir, outFile;
    bool exVtx = true, exTex = true, noHdr = false;
    for (int i=1; i<argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') argv[i]++;
            if (strcmp(argv[i], "-help") == 0) {
                doHelp();
                return 0;
            } else if (strcmp(argv[i], "-vtx") == 0) {
                exVtx = false;
                fprintf(stderr, "Warning: Including Vertex data\n");
            } else if (strcmp(argv[i], "-tex") == 0) {
                exTex = false;
                fprintf(stderr, "Note: Texture data unsupported. -tex will "
                                "be ignored for this export\n");
            } else if (strcmp(argv[i], "-nohdr") == 0) {
                noHdr = true;
            } else {
                fprintf(stderr, "Error: Unrecognized option %s\n", argv[i]);
                return 1;
            }
        } else {
            fFiles.push_back(plString(argv[i]));
        }
    }

    for (size_t i=0; i<fFiles.size(); i++) {
        try {
            page = rm.ReadPage(fFiles[i]);
        } catch (hsException& e) {
            fprintf(stderr, "%s:%lu: %s\n", e.File(), e.Line(), e.what());
            return 1;
        } catch (std::exception& e) {
            fprintf(stderr, "PrcExtract Exception: %s\n", e.what());
            return 1;
        } catch (...) {
            fprintf(stderr, "Undefined error!\n");
            return 1;
        }
        outDir = getOutputDir(fFiles[i], page);
        outFile = outDir + filenameConvert(fFiles[i]);
      #ifdef WIN32
        CreateDirectory(outDir.cstr(), NULL);
      #else
        mkdir(outDir.cstr(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
      #endif

        printf("Writing %s\n", outFile.cstr());
        hsFileStream* S = new hsFileStream();
        if (!S->open(outFile, fmWrite)) {
            fprintf(stderr, "Error opening %s for writing!\n", outFile.cstr());
            delete S;
            return 1;
        }
        S->setVer(rm.getVer());
        pfPrcHelper* prc = new pfPrcHelper(S);
        if (exVtx) prc->exclude(pfPrcHelper::kExcludeVertexData);
        if (exTex) prc->exclude(pfPrcHelper::kExcludeTextureData);
        if (!noHdr) {
            prc->writeComment("Generator: PrcExtract");
            prc->writeComment(plString("Source: ") + fFiles[i]);
            time_t ts = time(NULL);
            char buf[256];
            strftime(buf, 256, "Created: %Y/%m/%d %H:%M:%S", localtime(&ts));
            prc->writeComment(buf);
            S->writeStr("\n");
        }
        try {
            rm.WritePagePrc(prc, page);
        } catch (hsException& e) {
            fprintf(stderr, "%s:%lu: %s\n", e.File(), e.Line(), e.what());
            return 1;
        } catch (std::exception& e) {
            fprintf(stderr, "PrcExtract Exception: %s\n", e.what());
            return 1;
        } catch (...) {
            fprintf(stderr, "Undefined error!\n");
            return 1;
        }

        delete prc;
        S->close();
        delete S;
    }

    return 0;
}
