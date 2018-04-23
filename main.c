/*******************************************
 * CECE'S FIRST DEMO
 * =================
 *
 * PETSCII Art:
 * -----------
 * I created the hut.seq petscii art file with the Kaleidoscope V4.0 tool
 *
 * I learnt how to read in seq files via this post on lemon64:
 * - http://www.lemon64.com/forum/viewtopic.php?t=16272&sid=d5f1f7403a682352c2fb1d0fb09bc649
 *
 * Polyphonic Music Engine Adapted from:
 * ------------------------------------
 *
 *     USING MULTIPLE VOICE: Example 2
 *     - BASIC code taken from the Commodore 64 Programmer's reference Guide
 *     - Port to c-code by Gurce Isikyildiz 22/04/2018
 *     - I encoded a few additional fields for repeating sections of the melody
 *     - compile with cc65
 *
 *******************************************/
#include <stdio.h>
#include <conio.h>
#include <string.h>

#define Poke(A,X)  (*(unsigned char *)(A)) = (X)
#define Peek(A)    (*(unsigned char *)(A))
#define PokeW(A,X) (*(unsigned int *)(A)) = (X)
#define PeekW(A)   (*(unsigned int *)(A))

#define _SID_ 0xD400U

// Encoding = 0brm dddd dooo nnnn  (where d=duration, o=octave, n=note)
//
// My extra fields are:
// - m = marker for the place to repeat back to (use nnnn as a counter of how many times to repeat)
// - r = repeat back to marker
// - b = repeat back to beginning (wipe out any prior repeat counters)

#define MARKER 4096U
#define REPEAT_BACK_TO_MARKER 8192U
#define REPEAT_TO_BEGINNING 16384U

// durations
#define D1_16 128
#define D1_8  256
#define D1_8D 384
#define D1_4  512
#define D1_4_1_16 640
#define D1_4D 768
#define D1_2  1024
#define D1_2_1_16 1152
#define D1_2_1_8  1280
#define D1_2D     1536
#define D1        2048

// notes in scale
#define NC  0
#define NCs 1
#define ND  2
#define NDs 3
#define NE  4
#define NF  5
#define NFs 6
#define NG  7
#define NGs 8
#define NA  9
#define NAs 10
#define NB  11

// octaves
#define O0  0
#define O1  16
#define O2  32
#define O3  48
#define O4  64
#define O5  80
#define O6  96
#define O7  112

unsigned int addr;

void init_sid(void)
{
  for (addr = _SID_; addr <= (_SID_+24); addr++)
  {
    Poke(addr, 0);
  }
}

#define VERSE
#define CHORUS


// voice 1
int v1[] =
{
#ifdef VERSE
  MARKER,       // place a marker for the REPEAT_BACK_TO_MARKER token to jump back to

  //BAR1          : Tu ngay hai dua yeu nhau mong
  -D1_8,
  NE + O5 + D1_8,
  NE + O5 + D1_8,
  NG + O5 + D1_8,
  NB + O5 + D1_8,
  NA + O5 + D1_8,
  NG + O5 + D1_8,
  NB + O4 + D1_8,
  //BAR2          : uoc that nhieu
  NB + O5 + D1_4,
  NE + O5 + D1_8,
  NE + O5 + D1_4,
  -D1_4D,

  //BAR3          : tu ngay hai dua yeu nhau long
  -D1_8,
  NE + O5 + D1_8,
  NE + O5 + D1_8,
  NG + O5 + D1_8,
  NB + O5 + D1_8,
  NA + O5 + D1_8,
  NG + O5 + D1_8,
  NB + O4 + D1_8,
  //BAR4          : uoc bao nhieu
  NB + O5 + D1_4,
  NA + O5 + D1_8,
  NA + O5 + D1_4,
  -D1_4D,

  //BAR5          : Mong vang hai dua co chi la
  -D1_8,
  NA + O5 + D1_8,
  NA + O5 + D1_8,
  NB + O5 + D1_8,
  NC + O6 + D1_8,
  NB + O5 + D1_8,
  NA + O5 + D1_8,
  NE + O5 + D1_8,
  //BAR6          : qua xa xoi ta mo mot
  NE + O6 + D1_4,
  NB + O5 + D1_8,
  NB + O5 + D1_8,
  -D1_8,
  NB + O5 + D1_8,
  NB + O5 + D1_8,
  NG + O5 + D1_8,

  //BAR7          : mai nha tranh, ta mo mot
  NB + O5 + D1_4,
  NG + O5 + D1_8,
  NA + O5 + D1_8,
  -D1_8,
  NA + O5 + D1_8,
  NA + O5 + D1_8,
  NG + O5 + D1_8,

  //BAR8          : tup leu tinh. Doi minh dep
  NB + O5 + D1_4,
  NE + O5 + D1_8,
  NE + O5 + D1_8,
  -D1_8,
  NE + O5 + D1_8,
  NE + O5 + D1_8,
  NE + O5 + D1_8,

  //BAR9          : mai voi anh va
  NB + O5 + D1_8,
  -D1_8,
  NB  + O5 + D1_4,
  NFs + O5 + D1_8,
  NA  + O5 + D1_8,
  NFs + O5 + D1_8,
  ND  + O5 + D1_8,
  //BAR10         : em doi minh dep
  NE  + O5 + D1_4,
  -D1_4D,
  NE + O5 + D1_8,
  NE + O5 + D1_8,
  NE + O5 + D1_8,
  //BAR11         : mai duoi tup leu
  NB + O5 + D1_8,
  -D1_8,
  NB + O5 + D1_4,
  NFs + O5 + D1_8,
  NA  + O5 + D1_8,
  NFs + O5 + D1_8,
  ND  + O5 + D1_8,
  //BAR12         : xinh
  NE + O5 +D1,
#endif // VERSE

  REPEAT_BACK_TO_MARKER + 1,  // repeat back to marker once

  // CHORUS
  // BAR1         : tup leu ly
  -D1_4,
  NE + O6 + D1_4,
  NB + O5 + D1_4,
  ND + O6 + D1_4,

  // BAR2         : tuong cua anh va cua em
  NE + O5 + D1_4,
  NE + O5 + D1_8,
  NB + O5 + D1_4,
  NB + O4 + D1_8,
  NB + O4 + D1_8,
  ND + O5 + D1_8,

  // BAR3         : em, dau dau nao anh oi
  NE + O5 + D1_4D,
  NE + O7 + D1_4,
  NE + O7 + D1_8,
  ND + O7 + D1_8,
  NE + O7 + D1_8,

  // BAR4         : oi tup leu ly
  NE + O7 + D1_8,
  -D1_8,
  NE + O6 + D1_4,
  NB + O5 + D1_4,
  ND + O6 + D1_4,

  // BAR5         : tuong cua em va cua
  NE + O5 + D1_4,
  NE + O5 + D1_8,
  NB + O5 + D1_4,
  NE + O5 + D1_8,
  NE + O5 + D1_8,
  NG + O5 + D1_8,

  // BAR6         : anh, dau dau nao em
  NA + O5 + D1_4D,
  NA + O7 + D1_4,
  NA + O7 + D1_8,
  NG + O7 + D1_8,
  NA + O7 + D1_8,
 
  // BAR7         : oh tup leu ly
  NA + O7 + D1_8,
  -D1_8,
  NE + O6 + D1_4,
  NB + O5 + D1_4,
  ND + O6 + D1_4,

  // BAR8         : tuong do ta xay bang
  NE + O5 + D1_4,
  -D1_8,
  NE + O6 + D1_4,
  ND + O6 + D1_8,
  ND + O6 + D1_8,
  NB + O5 + D1_8,

  // BAR9         : duyen bang tinh khong ai ma
  ND + O6 + D1_4,
  NA + O5 + D1_8,
  NA + O5 + D1_8,
  -D1_8,
  NB + O5 + D1_8,
  NB + O5 + D1_8,
  NG + O5 + D1_8,

  // BAR10        : yeu bang minh khi ta dung
  NA + O5 + D1_4,
  NE + O5 + D1_8,
  NE + O5 + D1_8,
  -D1_8,
  NFs + O5 + D1_8,
  NFs + O5 + D1_8,
  NA + O5 + D1_8,

  // BAR11        : nhin mot dan con
  NB + O4 + D1_8,
  -D1_4,
  NB + O4 + D1_8,
  ND + O5 + D1_4,
  NE + O5 + D1_4,

  // BAR12        : xinh
  NE + O5 + D1,

  REPEAT_TO_BEGINNING,

  0
};

// voice 2
int v2[] =

{
#ifdef VERSE
  // BAR1
  NE + O1 + D1_4,
  -D1_8,
  NE + O1 + D1_8,
  -D1_8,
  NE + O1 + D1_8,
  -D1_4,
  // BAR2
  NE + O1 + D1_4,
  -D1_8,
  NE + O1 + D1_8,
  -D1_8,
  NE + O1 + D1_8,
  -D1_4,
  // BAR3
  NE + O1 + D1_4,
  -D1_8,
  NE + O1 + D1_8,
  -D1_8,
  NE + O1 + D1_8,
  -D1_4,
  // BAR4
  NA + O1 + D1_4,
  -D1_8,
  NA + O1 + D1_8,
  -D1_8,
  NA + O1 + D1_8,
  -D1_4,
  // BAR5
  NA + O1 + D1_4,
  -D1_8,
  NA + O1 + D1_8,
  -D1_8,
  NA + O1 + D1_8,
  -D1_4,
  // BAR6
  NB + O0 + D1_4,
  -D1_8,
  NB + O0 + D1_8,
  -D1_8,
  NB + O0 + D1_8,
  -D1_4,
  // BAR7
  NG + O1 + D1_4,
  -D1_8,
  NG + O1 + D1_8,
  -D1_8,
  NG + O1 + D1_8,
  -D1_4,
  // BAR8
  ND + O1 + D1_4,
  -D1_8,
  ND + O1 + D1_8,
  -D1_8,
  ND + O1 + D1_8,
  -D1_4,
  // BAR9
  NC + O1 + D1_4,
  -D1_8,
  NC + O1 + D1_8,
  ND + O1 + D1_4,
  ND + O1 + D1_4,
  // BAR10
  NE + O1 + D1_4,
  -D1_8,
  NE + O1 + D1_8,
  -D1_8,
  NE + O1 + D1_8,
  -D1_4,
  // BAR11
  NC + O1 + D1_4,
  -D1_8,
  NC + O1 + D1_8,
  ND + O1 + D1_4,
  ND + O1 + D1_4,
  // BAR12
  NE + O1 + D1_4,
  -D1_8,
  NE + O1 + D1_8,
  -D1_8,
  NE + O1 + D1_8,
  -D1_4,
#endif // VERSE

  // CHORUS
  // BAR1
  NE + O1 + D1_4,
  -D1_8,
  NE + O1 + D1_8,
  -D1_8,
  NE + O1 + D1_8,
  -D1_4,

  // BAR2
  NE + O1 + D1_4,
  -D1_8,
  NE + O1 + D1_8,
  -D1_8,
  NE + O1 + D1_8,
  -D1_4,

  // BAR3
  NE + O1 + D1_4,
  -D1_8,
  NE + O1 + D1_8,
  -D1_8,
  NE + O1 + D1_8,
  -D1_4,

  // BAR4
  NE + O1 + D1_4,
  -D1_8,
  NE + O1 + D1_8,
  -D1_8,
  NE + O1 + D1_8,
  -D1_4,

  // BAR5
  NE + O1 + D1_4,
  -D1_8,
  NE + O1 + D1_8,
  -D1_8,
  NE + O1 + D1_8,
  -D1_4,

  // BAR6
  NA + O1 + D1_4,
  -D1_8,
  NA + O1 + D1_8,
  -D1_8,
  NA + O1 + D1_8,
  -D1_4,

  // BAR7
  NA + O1 + D1_4,
  -D1_8,
  NA + O1 + D1_8,
  -D1_8,
  NA + O1 + D1_8,
  -D1_4,

  // BAR8
  NB + O0 + D1_4,
  -D1_8,
  NB + O0 + D1_8,
  -D1_8,
  NB + O0 + D1_8,
  -D1_4,

  // BAR9
  NG + O1 + D1_4,
  -D1_8,
  NG + O1 + D1_8,
  -D1_8,
  NG + O1 + D1_8,
  -D1_4,
  // BAR10
  ND + O1 + D1_4,
  -D1_8,
  ND + O1 + D1_8,
  -D1_8,
  ND + O1 + D1_8,
  -D1_4,
  // BAR11
  NC + O1 + D1_4,
  -D1_8,
  NC + O1 + D1_8,
  ND + O1 + D1_4,
  ND + O1 + D1_4,
  // BAR12
  NE + O1 + D1_4,
  -D1_8,
  NE + O1 + D1_8,
  -D1_8,
  NE + O1 + D1_8,
  -D1_4,

  0
};

// voice 3
int v3[] =
{
#ifdef VERSE
  // BAR1
  NE + O2 + D1_4,
  -D1_8,
  NE + O2 + D1_8,
  -D1_8,
  NE + O2 + D1_8,
  -D1_4,
  // BAR2
  NE + O2 + D1_4,
  -D1_8,
  NE + O2 + D1_8,
  -D1_8,
  NE + O2 + D1_8,
  -D1_4,
  // BAR3
  NE + O2 + D1_4,
  -D1_8,
  NE + O2 + D1_8,
  -D1_8,
  NE + O2 + D1_8,
  -D1_4,
  // BAR4
  NA + O2 + D1_4,
  -D1_8,
  NA + O2 + D1_8,
  -D1_8,
  NA + O2 + D1_8,
  -D1_4,
  // BAR5
  NA + O2 + D1_4,
  -D1_8,
  NA + O2 + D1_8,
  -D1_8,
  NA + O2 + D1_8,
  -D1_4,
  // BAR6
  NB + O1 + D1_4,
  -D1_8,
  NB + O1 + D1_8,
  -D1_8,
  NB + O1 + D1_8,
  -D1_4,
  // BAR7
  NG + O2 + D1_4,
  -D1_8,
  NG + O2 + D1_8,
  -D1_8,
  NG + O2 + D1_8,
  -D1_4,
  // BAR8
  ND + O2 + D1_4,
  -D1_8,
  ND + O2 + D1_8,
  -D1_8,
  ND + O2 + D1_8,
  -D1_4,
  // BAR9
  NC + O2 + D1_4,
  -D1_8,
  NC + O2 + D1_8,
  ND + O2 + D1_4,
  ND + O2 + D1_4,
  // BAR10
  NE + O2 + D1_4,
  -D1_8,
  NE + O2 + D1_8,
  -D1_8,
  NE + O2 + D1_8,
  -D1_4,
  // BAR11
  NC + O2 + D1_4,
  -D1_8,
  NC + O2 + D1_8,
  ND + O2 + D1_4,
  ND + O2 + D1_4,
  // BAR12
  NE + O2 + D1_4,
  -D1_8,
  NE + O2 + D1_8,
  -D1_8,
  NE + O2 + D1_8,
  -D1_4,
#endif // VERSE

  // CHORUS
  // BAR1
  NE + O2 + D1_4,
  -D1_8,
  NE + O2 + D1_8,
  -D1_8,
  NE + O2 + D1_8,
  -D1_4,

  // BAR2
  NE + O2 + D1_4,
  -D1_8,
  NE + O2 + D1_8,
  -D1_8,
  NE + O2 + D1_8,
  -D1_4,

  // BAR3
  NE + O2 + D1_4,
  -D1_8,
  NE + O2 + D1_8,
  -D1_8,
  NE + O2 + D1_8,
  -D1_4,

  // BAR4
  NE + O2 + D1_4,
  -D1_8,
  NE + O2 + D1_8,
  -D1_8,
  NE + O2 + D1_8,
  -D1_4,

  // BAR5
  NE + O2 + D1_4,
  -D1_8,
  NE + O2 + D1_8,
  -D1_8,
  NE + O2 + D1_8,
  -D1_4,

  // BAR6
  NA + O2 + D1_4,
  -D1_8,
  NA + O2 + D1_8,
  -D1_8,
  NA + O2 + D1_8,
  -D1_4,

  // BAR7
  NA + O2 + D1_4,
  -D1_8,
  NA + O2 + D1_8,
  -D1_8,
  NA + O2 + D1_8,
  -D1_4,

  // BAR8
  NB + O1 + D1_4,
  -D1_8,
  NB + O1 + D1_8,
  -D1_8,
  NB + O1 + D1_8,
  -D1_4,

  // BAR9
  NG + O2 + D1_4,
  -D1_8,
  NG + O2 + D1_8,
  -D1_8,
  NG + O2 + D1_8,
  -D1_4,
  // BAR10
  ND + O2 + D1_4,
  -D1_8,
  ND + O2 + D1_8,
  -D1_8,
  ND + O2 + D1_8,
  -D1_4,
  // BAR11
  NC + O2 + D1_4,
  -D1_8,
  NC + O2 + D1_8,
  ND + O2 + D1_4,
  ND + O2 + D1_4,
  // BAR12
  NE + O2 + D1_4,
  -D1_8,
  NE + O2 + D1_8,
  -D1_8,
  NE + O2 + D1_8,
  -D1_4,

  0
};

// 30 DIM FQ(11) : REM Dimension array to contain base frequency for each note
// 60 FOR I=0 TO 11: READ FQ(I): NEXT : REM Read in base frequency for each note.

// define frequency table
unsigned int fq[12] =
{
  34334U, 36376U, 38539U, 40830U,
  43258U, 45830U, 48556U, 51443U,
  54502U, 57743U, 61176U, 64814U
};

// Taking a look at the Commodore 64 Programmer's Reference Guide
// "Appendix E - Musical Note Values" can be assesed to learn which notes
// are referenced in the array above
// C7  : 34334U = 0x861E : HI = 0x86 = 134 : LO = 0x1E = 30
// Cs7 : 36376U = 0x8E18 = HI = 0x8E = 142 : LO = 0x18 = 24

// 40 V(0) = 17: V(1) = 65: V(2) = 33 : REM Store waveform control byte for each voice
unsigned int v[3] = { 17, 65, 33 };


// 20 DIM H(2,200), L(2,200), C(2,200)  : REM Dimension array to contain activity of song, 1/16th of a measure per location
unsigned char h[3][400];
unsigned char l[3][400];
unsigned char c[3][400];

char *lyrics[40] =
{
  "     tu ngay hai dua yeu nhau mong      ",
  "          uoc that nhieu...             ",
  "     tu ngay hai dua yeu nhau long      ",
  "          uoc bao nhieu...              ",
  "      mong vang hai dua co chi la       ",
  "         qua xa xoi: ta mo mot          ",
  "       mai nha tranh, ta mo mot         ",
  "      tup leu tinh. doi minh dep        ",
  "            mai voi anh va              ",
  "           em. doi minh dep             ",
  "           mai duoi tup leu             ",
  "                 xinh...                ",
  "    roi minh di kiem lieu xanh minh     ",
  "            ket lam manh.               ",
  "   roi minh xin khom truc xinh trong    ",
  "           het chung quanh.             ",
  "    ngoai vuon hoa cuc, hoa mai nao     ",
  "      khac chi tranh: ban mai minh      ",
  "      ngam mau hoa, dem dem minh        ",
  "     ngam chi hang. du ngheo cuoc       ",
  "           song van them hao            ",
  "           hao. doi minh dep            ",
  "            qua co ai bang              ",
  "                 ta...                  ",
  "              tup leu ly                ",
  "         tuong cua anh va cua           ",
  "         em, dau dau nao anh            ",
  "           oi? tup leu ly               ",
  "         tuong cua em va cua            ",
  "         anh, dau dau nao em            ",
  "           oi? tup leu ly               ",
  "         tuong do ta xay bang           ",
  "     duyen bang tinh, khong ai ma       ",
  "      yeu bang minh, khi ta dung        ",
  "          nhin mot dan con              ",
  "               xinh...                  ",
  NULL
};

// H = high-byte of frequency
// L = low-byte of frequency
// C = control byte of waveform

unsigned char st;   // the returned status value from JSR READST
unsigned char fname_len, lo, hi;
int lyric_idx = 0;

#pragma optimize(off)
void check_st(void)
{
  // check if our last command was successful
  __asm__ ( "JSR $FFB7" );                  // call READST
  __asm__ ( "STA %v", st );                 // store the st value for our c-code
  printf("st = %d\n", (int)st);
}

void intro_screen(void)
{
  // clear the screen
  __asm__ ( "JSR $E544" );
  __asm__ ( "LDA #$00" );
  __asm__ ( "STA $D020" );
  __asm__ ( "STA $D021" );

  __asm__ ( "LDA #159" ); // yellow colour for text
  __asm__ ( "JSR $FFD2" );                  // call CHROUT

  printf("\n\n\n\n");
  printf("      cece's first demo (23/04/2018)\n");
  printf("      =================\n\n\n");

  __asm__ ( "LDA #158" ); // yellow colour for text
  __asm__ ( "JSR $FFD2" );                  // call CHROUT

  printf("  aka. 'vietnamese karaoke machine'!!\n");
  printf("\n\n");

  printf("  tup leu ly tuong - by hoang thi tho\n\n\n");
  printf("         dedicated to my camcam ;)\n\n\n");
  printf("         press any key to begin");
  cgetc();
}

void load_petscii(void)
{
  char* file = "hut,s,r";

  // 5 POKE 53280,0:POKE 53281,0
  Poke(0xd020, 14); // light blue
  Poke(0xd021, 14); // light blue

  // clear the screen
  __asm__ ( "JSR $E544" );

  // 10 OPEN 5,8,5,"TETRIS,S,R"

  fname_len = strlen(file);
  lo = (unsigned char)((int)file & 0xff);
  hi = ((int)file >> 8) & 0xff;

  __asm__ ( "LDA %v", fname_len);           // A = length of the filename string
  __asm__ ( "LDX %v", lo );                 // LDX #<file;
  __asm__ ( "LDY %v", hi );                 // LDY #>file;
  __asm__ ( "JSR $FFBD" );                  // call SETNAM
  //check_st();
  
  // call SETLFS
  __asm__ ( "LDA #$03" );                   // set file-number to 5
  __asm__ ( "LDX #$08" );                   // device-number 8 (disk drive)
  __asm__ ( "LDY #$03" );                   // secondary address
  __asm__ ( "JSR $FFBA" );                  // call SETLFS
  //check_st();

  // call OPEN
  __asm__ ( "JSR $FFC0" );
  //check_st();
 

  while (1)
  {
  // call CHKIN
  __asm__ ( "LDX #$03" );                   // file-number 5
  __asm__ ( "JSR $FFC6" );                  // call CHKIN (file 5 now used as input)
  //check_st();

    // 20 GET #5,T$
    // call CHRIN
    __asm__ ( "JSR $FFCF" );                  // the next char is stored in the accumulator

    // 30 PRINT T$;
    __asm__ ( "JSR $FFD2" );                  // call CHROUT
    
    // 40 IF ST<>64 THEN GOTO 20
    __asm__ ( "JSR $FFB7" );                  // call READST
    __asm__ ( "STA %v", st );                 // store the st value for our c-code

    if (st == 64)
      break;

  }

  // 50 CLOSE 5
  __asm__ ( "LDA #$03" );                     // file-number 5
  __asm__ ( "JSR $FFC3" );                    // call CLOSE

  __asm__ ( "JSR $FFCC" );                    // call CLRCHN
}
#pragma optimize(on)


int main(void)
{
  int t;  // loop variable used for timers
  int i;  // generic loop variable
  int k;  // generic loop variable
  int im = 0;   // the total length of the songs (in units of 1/16th measures)

  int j = 0;
  int wa = 0;
  int wb = 0;
  int dr = 0;
  int oc = 0;
  int nt = 0;
  unsigned long fr = 0;
  unsigned int hf;
  unsigned int lf;

  int marker_pos = 0;     // the positon of the marker to repeat back to
  int repeat_to_marker_pos = 0;   // the position of the repeat-to-marker token
  int repeat_to_marker_count = 0; // the number of times to repeat back to marker
  int repeat_to_beginning_pos = 0; // the position of the repeat-to-beginning token
  int rptcnt = 0;
  
  // switch back to upper-case
  // https://www.cc65.org/mailarchive/2004-09/4446.html
  Poke(0xd018, 0x15);

  // 10 S = 54272 : FOR L = S TO S+24 : POKE L, 0 : NEXT
  init_sid();

  // 50 POKE S+10, 8: POKE S+22, 128: POKE S+23, 244 : REM Set high pulse width for voice 2 : Set high frequency for filter cutoff : Set resonance for filter and filter voice 3
  Poke(_SID_+10, 8);   // Set high pulse width for voice 2
  Poke(_SID_+22, 128); // Set high frequency for filter cutoff
  Poke(_SID_+23, 244); // Set resonance for filter and filter voice 3

  // 100 FOR K = 0 TO 2 : REM Begin decoding loop for each voice.
  for (k = 0; k <= 2; k++)
  {
    // 110 I=0 : REM Initialise pointer to activity array.
    int i = 0;    // index into the activity buffer
    int idx = 0;  // the index into the notes within a single single

    int *pvoice;
    if (k==0) pvoice = v1;
    if (k==1) pvoice = v2;
    if (k==2) pvoice = v3;

    //printf("v=%d,l=%d,h=%d,c=%d\n", k, l[k], h[k], c[k]);

    while (pvoice[idx] != 0)
    {
      // 120 READ NM : REM Read coded note
      int nm = pvoice[idx];

      //printf("i=%d : ",i);

      //else
      //  printf("fine\n");

      // 130 IF NM = 0 THEN 250 : REM If coded note is zero, then next voice.
      if (nm == 0)
        break;

      if ((unsigned int)nm == MARKER)
      {
        marker_pos = i;
        idx++;
        continue;
      }

      if (nm > 0 && (nm & REPEAT_BACK_TO_MARKER))
      {
        repeat_to_marker_pos = i;
        repeat_to_marker_count = (nm - REPEAT_BACK_TO_MARKER);
        idx++;
        continue;
      }

      if ((unsigned int)nm == REPEAT_TO_BEGINNING)
      {
        repeat_to_beginning_pos = i;
        idx++;
        continue;
      }

      // 140 WA = V(K) : WB = WA - 1 : IF NM < 0 THEN NM = -NM : WA = 0 : WB = 0 : REM Set waveform controls to proper voice. If silence, set waveform controls to 0.
      wa = v[k]; // set the waveform control to proper voice
      wb = wa - 1;

      if (nm < 0) // if encoded note value is negative, this equates to silence, so set waveform controls to 0.
      {
        nm = -nm;
        wa = 0;
        wb = 0;
      }

      // 150 DR% = NM / 128 : OC% = (NM - 128 * DR%) / 16 : REM Decode duration and octave.

      // Encoding = dddd dooo nnnn  (where d=duration, o=octave, n=note)
      dr = nm / 128;
      oc = (nm - 128*dr) / 16;  // ok, now I get what they're doing here, overly verbose masking :)

      // 160 NT = NM - 128 * DR% - 16 * OC% : REM Decode note.
      nt = (nm - 128*dr) - 16*oc;   // more overly verbose masking :)

      // 170 FR = FQ(NT) : REM Get base frequency for this note.
      fr = fq[nt];

      if (k == 1) // voice 2 debugging
      {
        //printf("nm=%d dr=%d oc=%d nt=%d fr=%lu\n", nm, dr, oc, nt, fr);
      }

      fr <<= 16;

      // 180 IF OC% = 7 THEN 200 : REM If highest octave, skip division loop.
      if (oc != 7)
      {
        int j;
        // 190 FOR J=6 TO OC% STEP -1: FR = FR / 2: NEXT : REM Divide base frequency by 2 appropriate number of times.
        for (j = 6; j >= oc; j--)
          fr = fr / 2UL;
      }

      fr >>= 16;

      // 200 HF% = FR / 256 : LF% = FR - 256 * HF% : REM Get high and low frequency bytes.
      hf = fr / 256U;
      lf = fr - 256U * hf;

      // 210 IF DR% = 1 THEN H(K,I) = HF% : L(K, I) = LF% : C(K, I) = WA : I=I+1 : GOTO 120: REM If sixteenth note, set activity array: high frequency, low frequency and waveform control (voice on)
      if (dr == 1)
      {
        h[k][i] = hf;
        l[k][i] = lf;
        c[k][i] = wa;
        i++;
        idx++;
        continue;
      }

      // 220 FOR J=1 TO DR%-1: H(K, I) = HF% : L(K, I) = LF% : C(K, I) = WA : I=I+1 : NEXT : REM For all but last beat of note, set activity array: high frequency, low frequency, waveform control (voice on)
      for (j=1; j < dr; j++)
      {
        h[k][i] = hf;
        l[k][i] = lf;
        c[k][i] = wa;
        i++;
      }

      // 230 H(K, I) = HF% : L(K, I) = LF% : C(K, I) = WB : REM For last beat of note, set activity array: high frequency, low frequency, waveform control (voice off)
      h[k][i] = hf;
      l[k][i] = lf;
      c[k][i] = wb;

      //if (k == 0)
      {
        //printf("v=%d,i=%d,hf=%d,lf=%d,wa=%d,wb=%d\n",k,i,hf,lf,wa,wb);
        //cgetc();
      }


      // 240 I = I + 1 : GOTO 120 : REM Increment pointer to activity array. Get next note.
      i++;
      idx++;
    }

    // ok, we finished rendering the present voice in the activity arrays... Now do other stuff...

    // 250 IF I > IM THEN IM = I : REM If longer than before, reset number of activities.
    if (i > im)
      im = i; // keep track of the longest activity

    // 260 NEXT : REM Go back for next voice
  }

  // Aha! Now we play what has been rendered by all 3 voices in those activity buffers...
  // Sheesh... This seems like such a wasteful way of doing this, but oh well, let's see how it goes in c then...

  intro_screen();
  load_petscii();

  // 500 POKE S+5, 0 : POKE S+6, 240 : REM Set Attack/Decay for voice 1 (A=0, D=0) : Set Sustain/Release for voice 1 (S=15, R=0)
  Poke(_SID_+5, 0);   // Set Attack/Decay for voice 1 (A=0, D=0)
  Poke(_SID_+6, 240); // Set Sustain/Release for voice 1 (S=15, R=0)

  // 510 POKE S+12, 85 : POKE S+13, 133 : REM Set Attack/Decay for voice 2 (A=5, D=5) : Set Sustain/Release for voice 2 (S=8,  R=5)
  Poke(_SID_+12, 85);   // Set Attack/Decay for voice 2 (A=5, D=5)
  Poke(_SID_+13, 133);  // Set Sustain/Release for voice 2 (S=8, R=5)

  // 520 POKE S+19, 10 : POKE S+20, 197 : REM Set Attack/Decay for voice 3 (A=0, D=10) : Set Sustain/Release for voice 3 (S=12, R=5)
  Poke(_SID_+19, 10);   // Set Attack/Decay for voice 3 (A=0, D=10)
  Poke(_SID_+20, 197);  // Set Sustain/Release for voice 3 (S=12, R=5)

  // 530 POKE S+24, 31 : REM Set volume 15, low-pass filtering.
  Poke(_SID_+24, 31);   // Set volume 15, low-pass filtering

  // 540 FOR I = 0 TO IM : REM Start loop for every 1/16th of a measure.
  rptcnt = repeat_to_marker_count;
  i = 0;
  while (i <= im)
  {
    //__asm__ ( "INC $D020" );

    if (i == repeat_to_marker_pos && i != 0)
    {
      if (rptcnt > 0)
      {
        rptcnt--;
        i = marker_pos;
        continue;
      }
    }

    if (i == repeat_to_beginning_pos && i != 0)
    {
      i = 0;
      rptcnt = repeat_to_marker_count;
      continue;
    }

    if ( ( i % 16 ) == 0)
    {
      // show the next lyric
      
      // home the cursor
      __asm__ ( "JSR $E566" );

      __asm__ ( "LDA #144" ); // black colour for text
      __asm__ ( "JSR $FFD2" );                  // call CHROUT

      for (k = 0; k<39; k++)
      {
        lo = lyrics[lyric_idx][k];
        __asm__ ( "LDA %v", lo);
        __asm__ ( "JSR $FFD2" );                  // call CHROUT
      }
      lyric_idx++;

      if (lyrics[lyric_idx] == 0)
        lyric_idx = 0;
    }

    //printf("fr=%u\n", l[0][i] + 256*h[0][i]);
    // 550 POKE S,   L(0, I) : POKE S+7,  L(1, I) : POKE S+14, L(2, I) : REM POKE low frequency from activity array for all voices.
    Poke(_SID_,    l[0][i]);
    Poke(_SID_+7,  l[1][i]);
    Poke(_SID_+14, l[2][i]);

    // 560 POKE S+1, H(0, I) : POKE S+8,  H(1, I) : POKE S+15, H(2, I) : REM POKE high frequency from activity array for all voices.
    Poke(_SID_+1,  h[0][i]);
    Poke(_SID_+8,  h[1][i]);
    Poke(_SID_+15, h[2][i]);

    // 570 POKE S+4, C(0, I) : POKE S+11, C(1, I) : POKE S+18, C(2, I) : REM POKE waveform control from activity array for all voices.
    Poke(_SID_+4,  c[0][i]);
    Poke(_SID_+11, c[1][i]);
    Poke(_SID_+18, c[2][i]);

    /*
    printf("l0=%d,l1=%d,l2=%d\n",l[0][i], l[1][i], l[2][i]);
    printf("h0=%d,h1=%d,h2=%d\n",h[0][i], h[1][i], h[2][i]);
    printf("c0=%d,c1=%d,c2=%d\n",c[0][i], c[1][i], c[2][i]);
    cgetc();
    */

    // 580 FOR T = 1 TO 80 : NEXT : NEXT : REM Timing loop for 1/16th of a measure and back for next 1/16th measure.
    for (t = 0; t < 1000; t++)
      ;

    i++;
  }

  // 590 FOR T = 1 TO 200 : NEXT : POKE S+24, 0 : REM Pause, then turns off volume.
  // Final pause before ending eh...
  for (t = 0; t < 5000; t++)
    ;

  // turn off volume
  Poke(_SID_+24, 0);

  // Encoding of durations
  // ---------------------
  // Units are in "number of 1/16ths of a measure"
  //                          |||| |  <-- 5-bits
  //  1 : 1/16       : 128 :  0000 1000 0000
  //  2 : 1/8        : 256 :  0001 0000 0000
  //  3 : d1/8       : 384 :  0001 1000 0000
  //  4 : 1/4        : 512 :  0010 0000 0000
  //  5 : 1/4 + 1/16 : 640 :  0010 1000 0000
  //  6 : d1/4       : 768 :  0011 0000 0000
  //  8 : 1/2        : 1024 : 0100 0000 0000
  //  9 : 1/2+1/16   : 1152 : 0100 1000 0000
  // 10 : 1/2+1/8    : 1280 : 0101 0000 0000
  // 12 : d1/2       : 1536 : 0110 0000 0000
  // 16 : whole      : 2048 : 1000 0000 0000

  // Octave encoding
  // ---------------
  // Valid values are 0 - 7 (3-bits)
  //                                ||| <-- 3-bits
  // 0 - 7 :                  0000 0xxx 0000

  // Note encoding
  // -------------
  // Valid values are 0 - 11 (4-bits)
  //                                    |||| <-- 4-bits
  // 0-11 :                   0000 0000 xxxx

  // Encoding Summary
  // ----------------
  //
  // dddd dooo nnnn
  //
  // d = duration, o = octave, n = note number (in scale)
  // A silence is obtained by using the negative of the duration number
  // (number of 1/16ths of a measure * 128)

  return 0;
}
