struct online_character {
    int CodePoint, X, Y, Width, Height, OriginX, OriginY;
};

struct online_font {
    const char *FontName;
    const char *FileName;
    int Size, Bold, Italic, Width, Height, CharacterCount;
    online_character *Characters;
};

static online_character GlobalCharactersCambria[] = {
    {' ', 406, 75, 12, 12, 6, 6},
    {'!', 676, 41, 16, 33, 3, 27},
    {'"', 260, 75, 21, 19, 4, 28},
    {'#', 364, 41, 29, 33, 4, 27},
    {'$', 263, 0, 25, 38, 4, 30},
    {'%', 288, 0, 37, 36, 4, 28},
    {'&', 441, 0, 33, 34, 5, 28},
    {'\'', 281, 75, 16, 19, 4, 28},
    {'(', 98, 0, 21, 41, 3, 28},
    {')', 119, 0, 21, 41, 5, 28},
    {'*', 168, 75, 24, 24, 5, 28},
    {'+', 75, 75, 26, 26, 4, 22},
    {',', 243, 75, 17, 20, 6, 9},
    {'-', 386, 75, 20, 14, 4, 15},
    {'.', 347, 75, 15, 15, 4, 9},
    {'/', 0, 0, 26, 41, 5, 28},
    {'0', 873, 0, 26, 34, 4, 28},
    {'1', 925, 0, 25, 34, 3, 28},
    {'2', 899, 0, 26, 34, 4, 28},
    {'3', 950, 0, 25, 34, 4, 28},
    {'4', 393, 41, 28, 33, 5, 27},
    {'5', 416, 0, 25, 35, 4, 28},
    {'6', 684, 0, 27, 34, 4, 28},
    {'7', 612, 41, 26, 33, 4, 27},
    {'8', 711, 0, 27, 34, 4, 28},
    {'9', 738, 0, 27, 34, 5, 28},
    {':', 153, 75, 15, 26, 3, 20},
    {';', 715, 41, 17, 30, 5, 20},
    {'<', 101, 75, 26, 26, 4, 22},
    {'=', 217, 75, 26, 21, 4, 20},
    {'>', 127, 75, 26, 26, 4, 22},
    {'?', 1000, 0, 23, 34, 5, 28},
    {'@', 325, 0, 36, 36, 4, 28},
    {'A', 474, 0, 32, 34, 6, 28},
    {'B', 421, 41, 28, 33, 4, 27},
    {'C', 765, 0, 27, 34, 4, 28},
    {'D', 274, 41, 30, 33, 4, 27},
    {'E', 477, 41, 27, 33, 4, 27},
    {'F', 504, 41, 27, 33, 4, 27},
    {'G', 598, 0, 29, 34, 4, 28},
    {'H', 119, 41, 31, 33, 4, 27},
    {'I', 638, 41, 19, 33, 4, 27},
    {'J', 225, 0, 20, 39, 6, 27},
    {'K', 150, 41, 31, 33, 4, 27},
    {'L', 531, 41, 27, 33, 4, 27},
    {'M', 84, 41, 35, 33, 4, 27},
    {'N', 181, 41, 31, 33, 4, 27},
    {'O', 568, 0, 30, 34, 4, 28},
    {'P', 558, 41, 27, 33, 4, 27},
    {'Q', 194, 0, 31, 40, 4, 28},
    {'R', 212, 41, 31, 33, 4, 27},
    {'S', 975, 0, 25, 34, 4, 28},
    {'T', 304, 41, 30, 33, 5, 27},
    {'U', 506, 0, 31, 34, 5, 27},
    {'V', 537, 0, 31, 34, 6, 27},
    {'W', 42, 41, 42, 33, 6, 27},
    {'X', 334, 41, 30, 33, 6, 27},
    {'Y', 243, 41, 31, 33, 6, 27},
    {'Z', 585, 41, 27, 33, 5, 27},
    {'[', 140, 0, 20, 41, 3, 28},
    {'\\', 26, 0, 26, 41, 5, 28},
    {']', 160, 0, 19, 41, 5, 28},
    {'^', 192, 75, 25, 23, 4, 28},
    {'_', 362, 75, 24, 14, 6, 4},
    {'`', 328, 75, 19, 18, 5, 28},
    {'a', 732, 41, 26, 28, 5, 21},
    {'b', 361, 0, 28, 35, 5, 28},
    {'c', 809, 41, 24, 28, 4, 21},
    {'d', 389, 0, 27, 35, 4, 28},
    {'e', 784, 41, 25, 28, 4, 21},
    {'f', 0, 41, 23, 34, 5, 28},
    {'g', 792, 0, 27, 34, 5, 21},
    {'h', 656, 0, 28, 34, 5, 28},
    {'i', 657, 41, 19, 33, 5, 27},
    {'j', 245, 0, 18, 39, 6, 27},
    {'k', 627, 0, 29, 34, 5, 28},
    {'l', 23, 41, 19, 34, 5, 28},
    {'m', 856, 41, 37, 27, 5, 21},
    {'n', 930, 41, 28, 27, 5, 21},
    {'o', 758, 41, 26, 28, 4, 21},
    {'p', 819, 0, 27, 34, 5, 21},
    {'q', 846, 0, 27, 34, 4, 21},
    {'r', 27, 75, 24, 27, 5, 21},
    {'s', 833, 41, 23, 28, 5, 21},
    {'t', 692, 41, 23, 31, 6, 25},
    {'u', 958, 41, 28, 27, 5, 21},
    {'v', 986, 41, 28, 27, 6, 21},
    {'w', 893, 41, 37, 27, 6, 21},
    {'x', 0, 75, 27, 27, 6, 21},
    {'y', 449, 41, 28, 33, 6, 21},
    {'z', 51, 75, 24, 27, 5, 21},
    {'{', 52, 0, 23, 41, 5, 28},
    {'|', 179, 0, 15, 41, 2, 28},
    {'}', 75, 0, 23, 41, 5, 28},
    {'~', 297, 75, 31, 18, 4, 18},
};

static online_font GlobalFontCambria = {
    "Cambria",
    "cambria.png",
    32,
    0,
    0,
    1024,
    128,
    95,
    GlobalCharactersCambria};