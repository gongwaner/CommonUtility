#pragma once

#include <vtkVector.h>

namespace Color
{
    //VTK color
    const std::string ColorArrayName = "Colors";

    //fundamental colors
    const vtkVector4d White{255, 255, 255, 255};
    const vtkVector4d Black{0, 0, 0, 255};
    const vtkVector4d Red{255, 0, 0, 255};
    const vtkVector4d Green{0, 255, 0, 255};
    const vtkVector4d Blue{0, 0, 255, 255};

    //red Colors
    const vtkVector4d IndianRed{205, 92, 92, 255};
    const vtkVector4d LightCoral{240, 128, 128, 255};
    const vtkVector4d DarkSalmon{233, 150, 122, 255};
    const vtkVector4d Crimson{220, 20, 60, 255};
    const vtkVector4d FireBrick{178, 34, 34, 255};
    const vtkVector4d DarkRed{139, 0, 0, 255};

    //pink colors
    const vtkVector4d MistyRose{255, 228, 225, 255};
    const vtkVector4d Pink{255, 192, 203, 255};
    const vtkVector4d LightPink{255, 182, 193, 255};
    const vtkVector4d HotPink{255, 105, 180, 255};
    const vtkVector4d DeepPink{255, 20, 147, 255};
    const vtkVector4d MediumVioletRed{199, 21, 133, 255};
    const vtkVector4d PaleVioletRed{219, 112, 147, 255};

    //orange colors
    const vtkVector4d LightSalmon{255, 160, 122, 255};
    const vtkVector4d Coral{255, 127, 80, 255};
    const vtkVector4d Tomato{255, 99, 71, 255};
    const vtkVector4d OrangeRed{255, 69, 0, 255};
    const vtkVector4d DarkOrange{255, 140, 0, 255};
    const vtkVector4d Orange{255, 165, 0, 255};

    //yellow colors
    const vtkVector4d Gold{255, 215, 0, 255};
    const vtkVector4d Yellow{255, 255, 0, 255};
    const vtkVector4d LightYellow{255, 255, 224, 255};
    const vtkVector4d LemonChiffon{255, 250, 205, 255};
    const vtkVector4d PapayaWhip{255, 239, 213, 255};
    const vtkVector4d PeachPuff{255, 218, 185, 255};
    const vtkVector4d Khaki{240, 230, 140, 255};
    const vtkVector4d DarkKhaki{189, 183, 107, 255};

    //purple colors
    const vtkVector4d Lavender{230, 230, 250, 255};
    const vtkVector4d LavenderBlush{255, 240, 245, 255};
    const vtkVector4d Thistle{216, 191, 216, 255};
    const vtkVector4d Plum{221, 160, 221, 255};
    const vtkVector4d Orchid{218, 112, 214, 255};
    const vtkVector4d Magenta{255, 0, 255, 255};
    const vtkVector4d MediumOrchid{186, 85, 211, 255};
    const vtkVector4d MediumPurple{147, 112, 219, 255};
    const vtkVector4d RebeccaPurple{102, 51, 153, 255};
    const vtkVector4d DarkViolet{148, 0, 211, 255};
    const vtkVector4d DarkMagenta{139, 0, 139, 255};
    const vtkVector4d Indigo{75, 0, 130, 255};

    //green colors
    const vtkVector4d GreenYellow{173, 255, 47, 255};
    const vtkVector4d LawnGreen{124, 252, 0, 255};
    const vtkVector4d LimeGreen{50, 205, 50, 255};
    const vtkVector4d LightGreen{144, 238, 144, 255};
    const vtkVector4d SpringGreen{0, 255, 127, 255};
    const vtkVector4d MediumSeaGreen{60, 179, 113, 255};
    const vtkVector4d SeaGreen{46, 139, 87, 255};
    const vtkVector4d ForestGreen{34, 139, 34, 255};
    const vtkVector4d DarkGreen{0, 100, 0, 255};
    const vtkVector4d YellowGreen{154, 205, 50, 255};
    const vtkVector4d OliveDrab{107, 142, 35, 255};
    const vtkVector4d Olive{128, 128, 0, 255};
    const vtkVector4d DarkOliveGreen{85, 107, 47, 255};
    const vtkVector4d MediumAquamarine{102, 205, 170, 255};
    const vtkVector4d DarkSeaGreen{143, 188, 139, 255};
    const vtkVector4d LightSeaGreen{32, 178, 170, 255};
    const vtkVector4d Cyan{0, 255, 255, 255};
    const vtkVector4d DarkCyan{0, 139, 139, 255};
    const vtkVector4d Teal{0, 128, 128, 255};

    //blue colors
    const vtkVector4d AliceBlue{240, 248, 255, 255};
    const vtkVector4d LightCyan{224, 255, 255, 255};
    const vtkVector4d PaleTurquoise{175, 238, 238, 255};
    const vtkVector4d Turquoise{64, 224, 208, 255};
    const vtkVector4d MediumTurquoise{72, 209, 204, 255};
    const vtkVector4d DarkTurquoise{0, 206, 209, 255};
    const vtkVector4d CadetBlue{95, 158, 160, 255};
    const vtkVector4d SteelBlue{70, 130, 180, 255};
    const vtkVector4d LightSteelBlue{176, 196, 222, 255};
    const vtkVector4d PowderBlue{176, 224, 230, 255};
    const vtkVector4d LightBlue{173, 216, 230, 255};
    const vtkVector4d SkyBlue{135, 206, 235, 255};
    const vtkVector4d DeepSkyBlue{0, 191, 255, 255};
    const vtkVector4d CornflowerBlue{100, 149, 237, 255};
    const vtkVector4d SlateBlue{106, 90, 205, 255};
    const vtkVector4d DarkSlateBlue{72, 61, 139, 255};
    const vtkVector4d MediumBlue{0, 0, 205, 255};
    const vtkVector4d DarkBlue{0, 0, 139, 255};
    const vtkVector4d Navy{0, 0, 128, 255};
    const vtkVector4d MidnightBlue{25, 25, 112, 255};

    //brown colors
    const vtkVector4d Wheat{245, 222, 179, 255};
    const vtkVector4d BurlyWood{222, 184, 135, 255};
    const vtkVector4d RosyBrown{188, 143, 143, 255};
    const vtkVector4d Chocolate{210, 105, 30, 255};
    const vtkVector4d SaddleBrown{139, 69, 19, 255};
    const vtkVector4d Goldenrod{218, 165, 32, 255};
    const vtkVector4d Brown{165, 42, 42, 255};
    const vtkVector4d Maroon{128, 0, 0, 255};

    //white colors
    const vtkVector4d Snow{255, 250, 250, 255};
    const vtkVector4d MintCream{245, 255, 250, 255};
    const vtkVector4d Azure{240, 255, 255, 255};
    const vtkVector4d Beige{245, 245, 220, 255};
    const vtkVector4d Ivory{255, 255, 240, 255};
    const vtkVector4d AntiqueWhite{250, 235, 215, 255};
    const vtkVector4d Linen{250, 240, 230, 255};

    //gray colors
    const vtkVector4d LightGray{211, 211, 211, 255};
    const vtkVector4d DarkGray{169, 169, 169, 255};
    const vtkVector4d Gray{128, 128, 128, 255};
    const vtkVector4d DimGray{105, 105, 105, 255};
    const vtkVector4d LightSlateGray{119, 136, 153, 255};
    const vtkVector4d SlateGray{112, 128, 144, 255};
    const vtkVector4d DarkSlateGray = {47, 79, 79, 255};

    const std::vector<vtkVector4d> ColorJet
    {
        vtkVector4d(128, 0, 0, 255),
        vtkVector4d(132, 0, 0, 255),
        vtkVector4d(137, 0, 0, 255),
        vtkVector4d(141, 0, 0, 255),
        vtkVector4d(146, 0, 0, 255),
        vtkVector4d(150, 0, 0, 255),
        vtkVector4d(155, 0, 0, 255),
        vtkVector4d(159, 0, 0, 255),
        vtkVector4d(164, 0, 0, 255),
        vtkVector4d(169, 0, 0, 255),
        vtkVector4d(173, 0, 0, 255),
        vtkVector4d(178, 0, 0, 255),
        vtkVector4d(182, 0, 0, 255),
        vtkVector4d(187, 0, 0, 255),
        vtkVector4d(191, 0, 0, 255),
        vtkVector4d(196, 0, 0, 255),
        vtkVector4d(201, 0, 0, 255),
        vtkVector4d(205, 0, 0, 255),
        vtkVector4d(210, 0, 0, 255),
        vtkVector4d(214, 0, 0, 255),
        vtkVector4d(219, 0, 0, 255),
        vtkVector4d(223, 0, 0, 255),
        vtkVector4d(228, 0, 0, 255),
        vtkVector4d(232, 0, 0, 255),
        vtkVector4d(237, 3, 0, 255),
        vtkVector4d(242, 7, 0, 255),
        vtkVector4d(246, 11, 0, 255),
        vtkVector4d(251, 15, 0, 255),
        vtkVector4d(255, 18, 0, 255),
        vtkVector4d(255, 22, 0, 255),
        vtkVector4d(255, 26, 0, 255),
        vtkVector4d(255, 29, 0, 255),
        vtkVector4d(255, 33, 0, 255),
        vtkVector4d(255, 37, 0, 255),
        vtkVector4d(255, 41, 0, 255),
        vtkVector4d(255, 44, 0, 255),
        vtkVector4d(255, 48, 0, 255),
        vtkVector4d(255, 52, 0, 255),
        vtkVector4d(255, 55, 0, 255),
        vtkVector4d(255, 59, 0, 255),
        vtkVector4d(255, 63, 0, 255),
        vtkVector4d(255, 67, 0, 255),
        vtkVector4d(255, 70, 0, 255),
        vtkVector4d(255, 74, 0, 255),
        vtkVector4d(255, 78, 0, 255),
        vtkVector4d(255, 81, 0, 255),
        vtkVector4d(255, 85, 0, 255),
        vtkVector4d(255, 89, 0, 255),
        vtkVector4d(255, 93, 0, 255),
        vtkVector4d(255, 96, 0, 255),
        vtkVector4d(255, 100, 0, 255),
        vtkVector4d(255, 104, 0, 255),
        vtkVector4d(255, 108, 0, 255),
        vtkVector4d(255, 111, 0, 255),
        vtkVector4d(255, 115, 0, 255),
        vtkVector4d(255, 119, 0, 255),
        vtkVector4d(255, 122, 0, 255),
        vtkVector4d(255, 126, 0, 255),
        vtkVector4d(255, 130, 0, 255),
        vtkVector4d(255, 134, 0, 255),
        vtkVector4d(255, 137, 0, 255),
        vtkVector4d(255, 141, 0, 255),
        vtkVector4d(255, 145, 0, 255),
        vtkVector4d(255, 148, 0, 255),
        vtkVector4d(255, 152, 0, 255),
        vtkVector4d(255, 156, 0, 255),
        vtkVector4d(255, 160, 0, 255),
        vtkVector4d(255, 163, 0, 255),
        vtkVector4d(255, 167, 0, 255),
        vtkVector4d(255, 171, 0, 255),
        vtkVector4d(255, 174, 0, 255),
        vtkVector4d(255, 178, 0, 255),
        vtkVector4d(255, 182, 0, 255),
        vtkVector4d(255, 186, 0, 255),
        vtkVector4d(255, 189, 0, 255),
        vtkVector4d(255, 193, 0, 255),
        vtkVector4d(255, 197, 0, 255),
        vtkVector4d(255, 200, 0, 255),
        vtkVector4d(255, 204, 0, 255),
        vtkVector4d(255, 208, 0, 255),
        vtkVector4d(255, 212, 0, 255),
        vtkVector4d(255, 215, 0, 255),
        vtkVector4d(255, 219, 0, 255),
        vtkVector4d(255, 223, 0, 255),
        vtkVector4d(255, 226, 0, 255),
        vtkVector4d(255, 230, 0, 255),
        vtkVector4d(255, 234, 0, 255),
        vtkVector4d(255, 238, 0, 255),
        vtkVector4d(251, 241, 0, 255),
        vtkVector4d(248, 245, 0, 255),
        vtkVector4d(245, 249, 2, 255),
        vtkVector4d(242, 253, 5, 255),
        vtkVector4d(238, 255, 8, 255),
        vtkVector4d(235, 255, 12, 255),
        vtkVector4d(232, 255, 15, 255),
        vtkVector4d(229, 255, 18, 255),
        vtkVector4d(225, 255, 21, 255),
        vtkVector4d(222, 255, 25, 255),
        vtkVector4d(219, 255, 28, 255),
        vtkVector4d(216, 255, 31, 255),
        vtkVector4d(212, 255, 34, 255),
        vtkVector4d(209, 255, 38, 255),
        vtkVector4d(206, 255, 41, 255),
        vtkVector4d(203, 255, 44, 255),
        vtkVector4d(199, 255, 47, 255),
        vtkVector4d(196, 255, 51, 255),
        vtkVector4d(193, 255, 54, 255),
        vtkVector4d(190, 255, 57, 255),
        vtkVector4d(187, 255, 60, 255),
        vtkVector4d(183, 255, 63, 255),
        vtkVector4d(180, 255, 67, 255),
        vtkVector4d(177, 255, 70, 255),
        vtkVector4d(174, 255, 73, 255),
        vtkVector4d(170, 255, 76, 255),
        vtkVector4d(167, 255, 80, 255),
        vtkVector4d(164, 255, 83, 255),
        vtkVector4d(161, 255, 86, 255),
        vtkVector4d(157, 255, 89, 255),
        vtkVector4d(154, 255, 93, 255),
        vtkVector4d(151, 255, 96, 255),
        vtkVector4d(148, 255, 99, 255),
        vtkVector4d(144, 255, 102, 255),
        vtkVector4d(141, 255, 106, 255),
        vtkVector4d(138, 255, 109, 255),
        vtkVector4d(135, 255, 112, 255),
        vtkVector4d(131, 255, 115, 255),
        vtkVector4d(128, 255, 119, 255),
        vtkVector4d(125, 255, 122, 255),
        vtkVector4d(119, 255, 128, 255),
        vtkVector4d(115, 255, 131, 255),
        vtkVector4d(112, 255, 135, 255),
        vtkVector4d(109, 255, 138, 255),
        vtkVector4d(106, 255, 141, 255),
        vtkVector4d(102, 255, 144, 255),
        vtkVector4d(99, 255, 148, 255),
        vtkVector4d(96, 255, 151, 255),
        vtkVector4d(93, 255, 154, 255),
        vtkVector4d(89, 255, 157, 255),
        vtkVector4d(86, 255, 161, 255),
        vtkVector4d(83, 255, 164, 255),
        vtkVector4d(80, 255, 167, 255),
        vtkVector4d(76, 255, 170, 255),
        vtkVector4d(73, 255, 174, 255),
        vtkVector4d(70, 255, 177, 255),
        vtkVector4d(67, 255, 180, 255),
        vtkVector4d(63, 255, 183, 255),
        vtkVector4d(60, 255, 187, 255),
        vtkVector4d(57, 255, 190, 255),
        vtkVector4d(54, 255, 193, 255),
        vtkVector4d(51, 255, 196, 255),
        vtkVector4d(47, 255, 199, 255),
        vtkVector4d(44, 255, 203, 255),
        vtkVector4d(41, 255, 206, 255),
        vtkVector4d(38, 255, 209, 255),
        vtkVector4d(34, 255, 212, 255),
        vtkVector4d(31, 255, 216, 255),
        vtkVector4d(28, 255, 219, 255),
        vtkVector4d(25, 255, 222, 255),
        vtkVector4d(21, 255, 225, 255),
        vtkVector4d(18, 253, 229, 255),
        vtkVector4d(15, 249, 232, 255),
        vtkVector4d(12, 245, 235, 255),
        vtkVector4d(8, 241, 238, 255),
        vtkVector4d(5, 237, 242, 255),
        vtkVector4d(2, 233, 245, 255),
        vtkVector4d(0, 229, 248, 255),
        vtkVector4d(0, 225, 251, 255),
        vtkVector4d(0, 221, 255, 255),
        vtkVector4d(0, 217, 255, 255),
        vtkVector4d(0, 213, 255, 255),
        vtkVector4d(0, 209, 255, 255),
        vtkVector4d(0, 205, 255, 255),
        vtkVector4d(0, 201, 255, 255),
        vtkVector4d(0, 197, 255, 255),
        vtkVector4d(0, 193, 255, 255),
        vtkVector4d(0, 189, 255, 255),
        vtkVector4d(0, 185, 255, 255),
        vtkVector4d(0, 181, 255, 255),
        vtkVector4d(0, 177, 255, 255),
        vtkVector4d(0, 173, 255, 255),
        vtkVector4d(0, 169, 255, 255),
        vtkVector4d(0, 165, 255, 255),
        vtkVector4d(0, 161, 255, 255),
        vtkVector4d(0, 157, 255, 255),
        vtkVector4d(0, 153, 255, 255),
        vtkVector4d(0, 149, 255, 255),
        vtkVector4d(0, 145, 255, 255),
        vtkVector4d(0, 141, 255, 255),
        vtkVector4d(0, 137, 255, 255),
        vtkVector4d(0, 133, 255, 255),
        vtkVector4d(0, 129, 255, 255),
        vtkVector4d(0, 124, 255, 255),
        vtkVector4d(0, 120, 255, 255),
        vtkVector4d(0, 116, 255, 255),
        vtkVector4d(0, 112, 255, 255),
        vtkVector4d(0, 108, 255, 255),
        vtkVector4d(0, 104, 255, 255),
        vtkVector4d(0, 100, 255, 255),
        vtkVector4d(0, 96, 255, 255),
        vtkVector4d(0, 92, 255, 255),
        vtkVector4d(0, 88, 255, 255),
        vtkVector4d(0, 84, 255, 255),
        vtkVector4d(0, 80, 255, 255),
        vtkVector4d(0, 76, 255, 255),
        vtkVector4d(0, 72, 255, 255),
        vtkVector4d(0, 68, 255, 255),
        vtkVector4d(0, 64, 255, 255),
        vtkVector4d(0, 60, 255, 255),
        vtkVector4d(0, 56, 255, 255),
        vtkVector4d(0, 52, 255, 255),
        vtkVector4d(0, 48, 255, 255),
        vtkVector4d(0, 44, 255, 255),
        vtkVector4d(0, 40, 255, 255),
        vtkVector4d(0, 36, 255, 255),
        vtkVector4d(0, 32, 255, 255),
        vtkVector4d(0, 28, 255, 255),
        vtkVector4d(0, 24, 255, 255),
        vtkVector4d(0, 20, 255, 255),
        vtkVector4d(0, 16, 255, 255),
        vtkVector4d(0, 12, 255, 255),
        vtkVector4d(0, 8, 255, 255),
        vtkVector4d(0, 4, 255, 255),
        vtkVector4d(0, 0, 255, 255),
        vtkVector4d(0, 0, 255, 255),
        vtkVector4d(0, 0, 255, 255),
        vtkVector4d(0, 0, 255, 255),
        vtkVector4d(0, 0, 255, 255),
        vtkVector4d(0, 0, 251, 255),
        vtkVector4d(0, 0, 246, 255),
        vtkVector4d(0, 0, 242, 255),
        vtkVector4d(0, 0, 237, 255),
        vtkVector4d(0, 0, 232, 255),
        vtkVector4d(0, 0, 228, 255),
        vtkVector4d(0, 0, 223, 255),
        vtkVector4d(0, 0, 219, 255),
        vtkVector4d(0, 0, 214, 255),
        vtkVector4d(0, 0, 210, 255),
        vtkVector4d(0, 0, 205, 255),
        vtkVector4d(0, 0, 201, 255),
        vtkVector4d(0, 0, 196, 255),
        vtkVector4d(0, 0, 191, 255),
        vtkVector4d(0, 0, 187, 255),
        vtkVector4d(0, 0, 182, 255),
        vtkVector4d(0, 0, 178, 255),
        vtkVector4d(0, 0, 173, 255),
        vtkVector4d(0, 0, 169, 255),
        vtkVector4d(0, 0, 164, 255),
        vtkVector4d(0, 0, 159, 255),
        vtkVector4d(0, 0, 155, 255),
        vtkVector4d(0, 0, 150, 255),
        vtkVector4d(0, 0, 146, 255),
        vtkVector4d(0, 0, 141, 255),
        vtkVector4d(0, 0, 137, 255),
        vtkVector4d(0, 0, 132, 255),
        vtkVector4d(0, 0, 128, 255),
    };
}
