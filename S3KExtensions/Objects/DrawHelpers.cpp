#include "../S3KExtensions.hpp"
#include "../DevFont.hpp"

using namespace RSDK;

namespace GameLogic {

    // Pulled from Drawing.cpp
    void DrawHelpers::DrawDevString(const char* string, int32 x, int32 y, int32 align, uint32 color) {
        uint16* rgb32To16_R;
        uint16* rgb32To16_G;
        uint16* rgb32To16_B;
        Mod::Engine::GetRGB32To16Buffer(&rgb32To16_R, &rgb32To16_G, &rgb32To16_B);

        uint16 color16 = rgb32To16_B[(color >> 0) & 0xFF] | rgb32To16_G[(color >> 8) & 0xFF] | rgb32To16_R[(color >> 16) & 0xFF];

        int32 charOffset = 0;
        bool32 linesRemain = true;
        while (linesRemain) {
            linesRemain = false;

            int32 lineSize = 0;
            char cur = string[charOffset];
            if (cur != '\n') {
                while (cur) {
                    cur = string[++charOffset];
                    lineSize++;
                    if (cur == '\n') {
                        linesRemain = true;
                        break;
                    }
                }
            }

            if (y >= 0 && y < screenInfo->size.y - 7) {
                int32 offset = 0;
                switch (align) {
                default:
                case ALIGN_LEFT: offset = 0; break;

                case ALIGN_CENTER: offset = 4 * lineSize; break;

                case ALIGN_RIGHT: offset = 8 * lineSize; break;
                }
                int32 drawX = x - offset;

                const char* curChar = &string[charOffset++ - lineSize];

                for (int32 c = 0; c < lineSize; ++c) {
                    if (drawX >= 0 && drawX < screenInfo->size.x - 7) {
                        uint16* frameBuffer = &screenInfo->frameBuffer[drawX + y * screenInfo->pitch];

                        if ((*curChar < '\t' || *curChar > '\n') && *curChar != ' ') {
                            uint8* textStencilPtr = &devTextStencil[0x40 * *curChar];

                            for (int32 h = 0; h < 8; ++h) {
                                for (int32 w = 0; w < 8; ++w) {
                                    if (*textStencilPtr)
                                        *frameBuffer = color16;

                                    ++textStencilPtr;
                                    ++frameBuffer;
                                }

                                frameBuffer += screenInfo->pitch - 8;
                            }
                        }

                        ++curChar;
                        drawX += 8;
                    }
                }
            }

            y += 8;
        }
    }

    void DrawHelpers::DrawDevStringWithShadow(const char* string, int32 x, int32 y, int32 align, uint32 color) {
        DrawHelpers::DrawDevString(string, x + 1, y + 1, align, (color & 0xFCFCFC) >> 2);
        DrawHelpers::DrawDevString(string, x, y, align, color);
    }
} // namespace GameLogic

