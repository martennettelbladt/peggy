//
//    PPPP      EEEEE     GGGG      GGGG     Y   Y
//    P   P     E        G         G          Y Y
//    PPPP      EEEE     G  GG     G  GG       Y
//    P         E        G   G     G   G       Y
//    P         EEEEE     GGGG      GGGG       Y
//
//    pattern.h  //  Mårten Nettelbladt  //  2022-11-13


// -------------------------------- N O T E   P A T T E R N
const byte pattern[48][16];

// -------------------------------- N O T E   P A T T E R N
const byte notePattern[8][16] =
{
  {1, 3, 5, 3, 5, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 2, 3, 2, 4, 2, 3, 2, 1, 2, 3, 2, 5, 2, 3, 2},
  {1, 2, 3, 4, 2, 3, 4, 5, 3, 4, 5, 6, 2, 3, 4, 5},
  {1, 2, 3, 4, 6, 5, 4, 3, 2, 3, 4, 5, 6, 5, 4, 3},
  {1, 2, 3, 4, 3, 2, 3, 2, 1, 2, 3, 4, 5, 4, 3, 2},
  {1, 2, 3, 2, 1, 2, 3, 4, 3, 2, 1, 2, 3, 4, 3, 2},
  {1, 2, 3, 4, 1, 2, 3, 5, 1, 2, 3, 6, 1, 2, 3, 5},
  {1, 1, 3, 3, 1, 2, 3, 5, 1, 2, 3, 6, 1, 2, 3, 5}
};

// -------------------------------- B Y T E   P A T T E R N
const byte bytePattern[5][16] =
{
  {1, 2, 4, 8, 16, 32, 64, 64}, // Decimal
  {128, 64, 32, 16, 8, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0}, // Decimal
  {0x70, 0x07, 0x70, 0x07, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Hexadecimal
  {0b1111011, 0b1111011, 0b1111011, 0b1111011, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Binary
  {0b1111011, 0b1111011, 0b1111011, 0b1111011, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  // Binary
  // Ganska coolt att välja ett högre tal än antal mönster. Glitch!
};

static const byte PROGMEM peggySineTable[256] =
{
	0,   0,   0,   0,   1,   1,   2,   2,   3,   3,   4,   5,   6,   7,   8,   9,
	10,  12,  13,  14,  16,  17,  19,  21,  22,  24,  26,  28,  30,  32,  34,  36,
	38,  41,  43,  45,  48,  50,  53,  55,  58,  61,  63,  66,  69,  72,  74,  77,
	80,  83,  86,  89,  92,  95,  98,  101, 104, 107, 110, 113, 117, 120, 123, 126,
	129, 132, 135, 138, 142, 145, 148, 151, 154, 157, 160, 163, 166, 169, 172, 175,
	178, 181, 183, 186, 189, 192, 194, 197, 200, 202, 205, 207, 210, 212, 214, 217,
	219, 221, 223, 225, 227, 229, 231, 233, 234, 236, 238, 239, 241, 242, 243, 245,
	246, 247, 248, 249, 250, 251, 252, 252, 253, 253, 254, 254, 255, 255, 255, 255, 
	255, 255, 255, 255, 254, 254, 253, 253, 252, 252, 251, 250, 249, 248, 247, 246,
	245, 243, 242, 241, 239, 238, 236, 234, 233, 231, 229, 227, 225, 223, 221, 219,
	217, 214, 212, 210, 207, 205, 202, 200, 197, 194, 192, 189, 186, 183, 181, 178,
	175, 172, 169, 166, 163, 160, 157, 154, 151, 148, 145, 142, 138, 135, 132, 129,
	126, 123, 120, 117, 113, 110, 107, 104, 101, 98,  95,  92,  89,  86,  83,  80,
	77,  74,  72,  69,  66,  63,  61,  58,  55,  53,  50,  48,  45,  43,  41,  38,
	36,  34,  32,  30,  28,  26,  24,  22,  21,  19,  17,  16,  14,  13,  12,  10,
	9,   8,   7,   6,   5,   4,   3,   3,   2,   2,   1,   1,   0,   0,   0,   0
};

/*
static const uint8_t PROGMEM _NeoPixelSineTable[256] = {
    128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170,
    173, 176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211,
    213, 215, 218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240,
    241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254,
    254, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 251,
    250, 250, 249, 248, 246, 245, 244, 243, 241, 240, 238, 237, 235, 234, 232,
    230, 228, 226, 224, 222, 220, 218, 215, 213, 211, 208, 206, 203, 201, 198,
    196, 193, 190, 188, 185, 182, 179, 176, 173, 170, 167, 165, 162, 158, 155,
    152, 149, 146, 143, 140, 137, 134, 131, 128, 124, 121, 118, 115, 112, 109,
    106, 103, 100, 97,  93,  90,  88,  85,  82,  79,  76,  73,  70,  67,  65,
    62,  59,  57,  54,  52,  49,  47,  44,  42,  40,  37,  35,  33,  31,  29,
    27,  25,  23,  21,  20,  18,  17,  15,  14,  12,  11,  10,  9,   7,   6,
    5,   5,   4,   3,   2,   2,   1,   1,   1,   0,   0,   0,   0,   0,   0,
    0,   1,   1,   1,   2,   2,   3,   4,   5,   5,   6,   7,   9,   10,  11,
    12,  14,  15,  17,  18,  20,  21,  23,  25,  27,  29,  31,  33,  35,  37,
    40,  42,  44,  47,  49,  52,  54,  57,  59,  62,  65,  67,  70,  73,  76,
    79,  82,  85,  88,  90,  93,  97,  100, 103, 106, 109, 112, 115, 118, 121,
    124};
	
    @brief   An 8-bit integer sine wave function, not directly compatible
             with standard trigonometric units like radians or degrees.
    @param   x  Input angle, 0-255; 256 would loop back to zero, completing
                the circle (equivalent to 360 degrees or 2 pi radians).
                One can therefore use an unsigned 8-bit variable and simply
                add or subtract, allowing it to overflow/underflow and it
                still does the expected contiguous thing.
    @return  Sine result, 0 to 255, or -128 to +127 if type-converted to
             a signed int8_t, but you'll most likely want unsigned as this
             output is often used for pixel brightness in animation effects.
  
  static uint8_t sine8(uint8_t x) {
    return pgm_read_byte(&_NeoPixelSineTable[x]); // 0-255 in, 0-255 out
  }
*/

/*Octave  C     C#    D     D#    E     F     F#    G     G#    A     A#    B
  -1      0     1     2     3     4     5     6     7     8     9     10    11
  0       12    13    14    15    16    17    18    19    20    21    22    23
  1       24    25    26    27    28    29    30    31    32    33    34    35
  2       36    37    38    39    40    41    42    43    44    45    46    47
  3       48    49    50    51    52    53    54    55    56    57    58    59
  4       60    61    62    63    64    65    66    67    68    69    70    71
  5       72    73    74    75    76    77    78    79    80    81    82    83
  6       84    85    86    87    88    89    90    91    92    93    94    95
  7       96    97    98    99    100   101   102   103   104   105   106   107
  8       108   109   110   111   112   113   114   115   116   117   118   119
  9       120   121   122   123   124   125   126   127

  Octave  C     C#    D     D#    E     F     F#    G     G#    A     A#    B
  -1      0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07  0x08  0x09  0x0a  0x0b
  0       0x0c  0x0d  0x0e  0x0f  0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17
  1       0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f  0x20  0x21  0x22  0x23
  2       0x24  0x25  0x26  0x27  0x28  0x29  0x2a  0x2b  0x2c  0x2d  0x2e  0x2f
  3       0x30  0x31  0x32  0x33  0x34  0x35  0x36  0x37  0x38  0x39  0x3A  0x3B
  4       0x3C  0x3D  0x3E  0x3F  0x40  0x41  0x42  0x43  0x44  0x45  0x46  0x47
  5       0x48  0x49  0x4A  0x4B  0x4C  0x4D  0x4E  0x4F  0x50  0x51  0x52  0x53
  6       0x54  0x55  0x56  0x57  0x58  0x59  0x5A  0x5B  0x5C  0x5D  0x5E  0x5F
  7       0x60  0x61  0x62  0x63  0x64  0x65  0x66  0x67  0x68  0x69  0x6A  0x6B
  8       0x6C  0x6D  0x6E  0x6F  0x70  0x71  0x72  0x73  0x74  0x75  0x76  0x77
  9       0x78  0x79  0x7A  0x7B  0x7C  0x7D  0x7E  0x7F

  PEGGY logo bytes: 
  0x3e, 0x0a, 0x0e, 0x00, 0x3e, 0x2a, 0x0e, 0x00, 0x3e, 0xaa, 0xee, 0x00, 0x3e, 0xaa, 0xee, 0x00, 0x0e, 0x28, 0x3e
  p                       e                       g                       g                       y
  
  .byte $3E ; |  XXXXX |
  .byte $0A ; |    X X |
  .byte $0E ; |    XXX |
  .byte $00 ; |        |
  .byte $3E ; |  XXXXX |
  .byte $2A ; |  X X X |
  .byte $0E ; |    XXX |
  .byte $00 ; |        |
  .byte $3E ; |  XXXXX |
  .byte $AA ; |X X X X |
  .byte $EE ; |XXX XXX |
  .byte $00 ; |        |
  .byte $0E ; |    XXX |
  .byte $28 ; |  X X   |
  .byte $3E ; |  XXXXX |
  .byte $00 ; |        |
  https://www.masswerk.at/vcs-tools/TinySpriteEditor/

*/
