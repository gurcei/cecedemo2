/*******************************************
 * CECE'S SECOND DEMO
 * ==================
 *
 * 05/05/2018
 * ----------
 * Based on CECE'S FIRST DEMO
 *
 * Music Engine refined to use 1/32th notes, so that I could add cute trills into the melody.
 * I also added more comments into the code to figure out bit-fields of the SID-chip registers
 * that get set in this example. This helped me to figure out how to change voice3 to be my
 * drum channel.
 *
 * PETSCII Art:
 * -----------
 * I created the tini.seq petscii art file with the Kaleidoscope V4.0 tool
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

// Encoding = 0rxd dddd dooo nnnn  (where d=duration, o=octave, n=note)
//
// I've added an extra bit so that I can have 1/32 notes too
//
// My extra fields are:
// - x = special token, it will override the purpose of the bit-fields in order to accomodate more commands
// - r = repeat back to marker

//#define MARKER 4096U
//#define REPEAT_BACK_TO_MARKER 8192U
#define REPEAT_TO_BEGINNING 16384U

// durations
#define D1_32 128
#define D1_16 256
#define D1_8  512
#define D1_8D 768
#define D1_4  1024
#define D1_4_1_16 1280
#define D1_4D 1536
#define D1_2  2048
#define D1_2_1_16 2304
#define D1_2_1_8  2560
#define D1_2D     3072
#define D1        4096

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
  // 10 S = 54272 : FOR L = S TO S+24 : POKE L, 0 : NEXT
  for (addr = _SID_; addr <= (_SID_+24); addr++)
  {
    Poke(addr, 0);
  }
}

#define VERSE
#define CHORUS

#define CHORUS_V1 \
  //BAR1          : Tin Tin, Tini mini hanim \
  NE + O5 + D1_4,                            \
  NE + O5 + D1_4,                            \
  NE + O5 + D1_16,                           \
  NG + O5 + D1_16,                           \
  NF + O5 + D1_16,                           \
  NG + O5 + D1_16,                           \
  NF + O5 + D1_8,                            \
  NE + O5 + D1_8,                            \
                                             \
  //BAR2          : Tin Tin, Tini mini hanim \
  NE + O5 + D1_4,                            \
  NE + O5 + D1_4,                            \
  NE + O5 + D1_16,                           \
  NG + O5 + D1_16,                           \
  NF + O5 + D1_16,                           \
  NG + O5 + D1_16,                           \
  NF + O5 + D1_8,                            \
  NE + O5 + D1_8,                            \
                                             \
  //BAR3          : Seni seviyor canim       \
  NF + O5 + D1_8,                            \
  NF + O5 + D1_8,                            \
  NE + O5 + D1_4,                            \
  ND + O5 + D1_8,                            \
  ND + O5 + D1_8,                            \
  NC + O5 + D1_8,                            \
  NC + O5 + D1_8,                            \
                                             \
  //BAR4          : Seni seviyor canim       \
  NF + O5 + D1_8,                            \
  NF + O5 + D1_8,                            \
  NE + O5 + D1_4,                            \
  ND + O5 + D1_8,                            \
  ND + O5 + D1_8,                            \
  NC + O5 + D1_8,                            \
  NC + O5 + D1_8,                            

#define VERSE_V1 \
  // BAR1 \
  NE + O5 + D1_8, \
  NE + O5 + D1_8, \
  NE + O5 + D1_4, \
  NE + O5 + D1_8, \
  NG + O5 + D1_8, \
  NF + O5 + D1_32, \
  NG + O5 + D1_32, \
  NF + O5 + D1_32, \
  NG + O5 + D1_32, \
  NE + O5 + D1_8, \
   \
  // BAR2 \
  NE + O5 + D1_8, \
  NE + O5 + D1_8, \
  NE + O5 + D1_4, \
  NE + O5 + D1_8, \
  NG + O5 + D1_8, \
  NF + O5 + D1_32, \
  NG + O5 + D1_32, \
  NF + O5 + D1_32, \
  NG + O5 + D1_32, \
  NE + O5 + D1_8, \
 \
  // BAR3 \
  NF + O5 + D1_16, \
  NE + O5 + D1_16, \
  NF + O5 + D1_16, \
  NG + O5 + D1_16, \
  NE + O5 + D1_8, \
  NE + O5 + D1_8, \
  ND + O5 + D1_16, \
  NC + O5 + D1_16, \
  ND + O5 + D1_16, \
  NE + O5 + D1_16, \
  NC + O5 + D1_8, \
  NC + O5 + D1_8, \
   \
  // BAR4 \
  NF + O5 + D1_16, \
  NE + O5 + D1_16, \
  NF + O5 + D1_16, \
  NG + O5 + D1_16, \
  NE + O5 + D1_8, \
  NE + O5 + D1_8, \
  ND + O5 + D1_16, \
  NC + O5 + D1_16, \
  ND + O5 + D1_16, \
  NE + O5 + D1_16, \
  NC + O5 + D1_8, \
  NC + O5 + D1_8,


// voice 1
int v1[] =
{
  CHORUS_V1
  CHORUS_V1
  VERSE_V1
  VERSE_V1

  0
};

#define CHORUS_V2 \
  // BAR1 \
  NC + O3 + D1_8D, \
  NG + O3 + D1_16, \
  NE + O3 + D1_8, \
  NG + O3 + D1_8, \
  NC + O3 + D1_8D, \
  NG + O3 + D1_16, \
  NE + O3 + D1_8, \
  NG + O3 + D1_8, \
 \
  // BAR2 \
  NC + O3 + D1_8D, \
  NG + O3 + D1_16, \
  NE + O3 + D1_8, \
  NG + O3 + D1_8, \
  NC + O3 + D1_8D, \
  NG + O3 + D1_16, \
  NE + O3 + D1_8, \
  NG + O3 + D1_8, \
 \
  // BAR3 \
  NF + O3 + D1_8D, \
  NF + O3 + D1_16, \
  NE + O3 + D1_8, \
  NE + O3 + D1_8, \
  ND + O3 + D1_8D, \
  ND + O3 + D1_16, \
  NC + O3 + D1_8, \
  NC + O3 + D1_8, \
 \
  // BAR4 \
  NF + O3 + D1_8D, \
  NF + O3 + D1_16, \
  NE + O3 + D1_8, \
  NE + O3 + D1_8, \
  ND + O3 + D1_8D, \
  ND + O3 + D1_16, \
  NC + O3 + D1_8, \
  NC + O3 + D1_8, \


// voice 2
int v2[] =
{
  CHORUS_V2
  CHORUS_V2
  CHORUS_V2  // recycle for the verse too
  CHORUS_V2  // recycle for the verse too

  REPEAT_TO_BEGINNING,

  0
};

#define DRUM_PAT1 \
  NC + O2 + D1_8, \
  NC + O4 + D1_16, \
  NC + O4 + D1_16, \
  NC + O2 + D1_8, \
  NC + O4 + D1_8, 

#define DRUM_PAT2 \
  NC + O2 + D1_16, \
  NC + O4 + D1_8, \
  NC + O4 + D1_16, \
  NC + O2 + D1_8, \
  NC + O4 + D1_8, 

#define DRUM_PAT3 \
  NC + O4 + D1_16, \
  NC + O4 + D1_16, \
  NC + O4 + D1_16, \
  NC + O4 + D1_16, \
  NC + O3 + D1_16, \
  NC + O3 + D1_16, \
  NC + O3 + D1_16, \
  NC + O3 + D1_16, \

#define DRUM_PAT1x2 \
  DRUM_PAT1 \
  DRUM_PAT1

#define DRUM_PAT1n2 \
  DRUM_PAT1 \
  DRUM_PAT2

#define DRUM_PAT3n2 \
  DRUM_PAT3 \
  DRUM_PAT2

#define DRUM_PAT1x4 \
  DRUM_PAT1x2 \
  DRUM_PAT1n2

// voice 3
int v3[] =
{
  DRUM_PAT1x4
  DRUM_PAT1x4
  DRUM_PAT1x4
  DRUM_PAT3n2
  DRUM_PAT3n2

  DRUM_PAT1x4
  DRUM_PAT1x4
  DRUM_PAT1x4
  DRUM_PAT3n2
  DRUM_PAT3n2

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
unsigned int v[3] = { 17, 65, 129 }; // These are to be stored in: 
                              // (0xd404) v1 control register
                              // 0001 0001 (17)
                              // rpst omzg (r=random noise, p=pulse, s=sawtooth, t=triangle
                              //            o=disable oscillator, m=ring mod. v1 with v3,
                              //            z=synchronize v1 with v3, g=gate bit: 1=start attack+decay+sustain, 0=start release
                              // (0xd40b) v2 control register
                              // 0100 0001 (65)
                              // rpst omzg (r=random noise, p=pulse, s=sawtooth, t=triangle
                              //            o=disable oscillator, m=ring mod. v2 with v1,
                              //            z=synchronize v2 with v1, g=gate bit: 1=start attack+decay+sustain, 0=start release
                              // (0xd412) v3 control register
                              // 1000 0001 (129)
                              // rpst omzg (r=random noise, p=pulse, s=sawtooth, t=triangle
                              //            o=disable oscillator, m=ring mod. v3 with v2,
                              //            z=synchronize v3 with v2, g=gate bit: 1=start attack+decay+sustain, 0=start release


// vars to hold hfreq, lfreq and control register values for the current 1/32nd measure (per voice)
unsigned char h[3];   // current MSB frequency for each voice
unsigned char l[3];   // current LSB frequency for each voice
unsigned char c[3];   // current control register for each voice

char *lyrics[40] =
{
  "        tin tin, tini mini hanim        ",
  "          tin tin, tini mini hanim      ",
  "           seni seviyor canim           ",
  "             seni seviyor canim         ",
  "        tin tin, tini mini hanim        ",
  "          tin tin, tini mini hanim      ",
  "           seni seviyor canim           ",
  "             seni seviyor canim         ",
  "            seftali agaclari            ",
  "              seftali agaclari          ",
  "           turlu cicek baslari          ",
  "             turlu cicek baslari        ",
  "            seftali agaclari            ",
  "              seftali agaclari          ",
  "           turlu cicek baslari          ",
  "             turlu cicek baslari        ",
  "        tin tin, tini mini hanim        ",
  "          tin tin, tini mini hanim      ",
  "           seni seviyor canim           ",
  "             seni seviyor canim         ",
  "        tin tin, tini mini hanim        ",
  "          tin tin, tini mini hanim      ",
  "           seni seviyor canim           ",
  "             seni seviyor canim         ",
  "           yakti yandirdi beni          ",
  "             yakti yandirdi beni        ",
  "           yarin hilal kaslari          ",
  "             yarin hilal kaslari        ",
  "           yakti yandirdi beni          ",
  "             yakti yandirdi beni        ",
  "           yarin hilal kaslari          ",
  "             yarin hilal kaslari        ",
  NULL
};

// H = high-byte of frequency
// L = low-byte of frequency
// C = control byte of waveform

unsigned char st;   // the returned status value from JSR READST
unsigned char fname_len, lo, hi;
int lyric_idx = 0;

int marker_pos = 0;     // the positon of the marker to repeat back to
int repeat_to_marker_pos = 0;   // the position of the repeat-to-marker token
int repeat_to_marker_count = 0; // the number of times to repeat back to marker
int repeat_to_beginning_pos = 0; // the position of the repeat-to-beginning token
int rptcnt = 0;       // a counter for the number of times to repeat a marked section of music
int nm[3] = { 0 };   // the current encoded note-data value for each voice
int wa[3] = { 0 };   // each decoded voice's control register value with gating on
int wb[3] = { 0 };   // each decoded voice's control register value with gating off
  
int dr = 0;   // duration of currently decoded note-data
int oc = 0;   // octave of currently decoded note-data
int nt = 0;   // note-in-octave of currently decoded note-data
unsigned long fr = 0; // 16-bit frequency of currently decoded note-data
unsigned int hf;      // MSB frequency of currently decoded note-data
unsigned int lf;      // LSB frequency of currently decoded note-data

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
  printf("      cece's second demo (05/05/2018)\n");
  printf("      ==================\n\n\n");

  __asm__ ( "LDA #158" ); // yellow colour for text
  __asm__ ( "JSR $FFD2" );                  // call CHROUT

  printf("    aka. 'turkish karaoke machine'!!\n");
  printf("\n\n");

  printf("      tini mini hanim - by unknown\n\n\n");
  printf("         dedicated to my mum ;)\n\n\n");
  printf("         press any key to begin");
  cgetc();
}

void load_petscii(void)
{
  char* file = "tini,s,r";

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

int girlx = 100;
int girly = 195;
int frame = 1;
int dir=0;

void update_sprites(unsigned int i)
{
  Poke(2040, 192+frame*3+dir*6); // pick sprite0 index
  Poke(2041, 193+frame*3+dir*6); // pick sprite1 index
  Poke(2042, 194+frame*3+dir*6); // pick sprite2 index
  Poke(0xd015, (int)1+2+4);  // turn on sprite0+1+2
  Poke(0xd000, girlx & 0xff); // sprite0-x
  Poke(0xd001, girly); // sprite0-y
  Poke(0xd002, girlx & 0xff); // sprite1-x
  Poke(0xd003, girly); // sprite1-y
  Poke(0xd004, girlx & 0xff); // sprite2-x
  Poke(0xd005, girly); // sprite2-y
  if (girlx > 0xff)
    Poke(0xd010, 255);
  else
    Poke(0xd010, 0);   // spritex-msb

  Poke(0xd027, 0);  // Sprite0 color
  Poke(0xd028, 10);  // Sprite1 color
  Poke(0xd029, 3);  // Sprite2 color

  // select direction & frame to use for girly animation
  if (dir == 0)
  {
    girlx += 2;
    if (girlx > 310) dir = 1;
  }
  else // dir == 1
  {
    girlx -= 2;
    if (girlx < 25) dir = 0;
  }
  if (i % 2 == 0)
    frame = (frame+1) % 2;

  // SPRITES ARE 24 PIXELS BY 21 PIXELS
  // LOCATION = (BANK * 16384) + (SPRITE POINTER VALUE * 64)
  // ---------
  // 2040 (0x07F8) Sprite0 pointer
  // 2041 (0x07F9) Sprite1 pointer
  // 2042 (0x07FA) Sprite2 pointer
  // 2043 (0x07FB) Sprite3 pointer
  // 2044 (0x07FC) Sprite4 pointer
  // 2045 (0x07FD) Sprite5 pointer
  // 2046 (0x07FE) Sprite6 pointer
  // 2047 (0x07FF) Sprite7 pointer
  // ---------
  // 53269 (0xD015) SPRITE ENABLE register
  // ---------
  // 53287 (0xD027) Sprite0 color
  // 53288 (0xD028) Sprite1 color
  // 53289 (0xD029) Sprite2 color
  // 53290 (0xD02A) Sprite3 color
  // 53291 (0xD02B) Sprite4 color
  // 53292 (0xD02C) Sprite5 color
  // 53293 (0xD02D) Sprite6 color
  // 53294 (0xD02E) Sprite7 color
  // ---------
  // 53248 (0xD000) Sprite0 X Position
  // 53249 (0xD001) Sprite0 Y Position
  // 53250 (0xD002) Sprite1 X Position
  // 53251 (0xD003) Sprite1 Y Position
  // 53252 (0xD004) Sprite2 X Position
  // 53253 (0xD005) Sprite2 Y Position
  // 53254 (0xD006) Sprite3 X Position
  // 53255 (0xD007) Sprite3 Y Position
  // 53256 (0xD008) Sprite4 X Position
  // 53257 (0xD009) Sprite4 Y Position
  // 53258 (0xD00A) Sprite5 X Position
  // 53259 (0xD00B) Sprite5 Y Position
  // 53260 (0xD00C) Sprite6 X Position
  // 53261 (0xD00D) Sprite6 Y Position
  // 53262 (0xD00E) Sprite7 X Position
  // 53263 (0xD00F) Sprite7 Y Position
  // 53264 (0xD010) SPRITE X MSB Register
}

int inverted(int val)
{
  return (val & 1 ? 128 : 0)
    + (val & 2 ? 64 : 0)
    + (val & 4 ? 32 : 0)
    + (val & 8 ? 16 : 0)
    + (val & 16 ? 8 : 0)
    + (val & 32 ? 4 : 0)
    + (val & 64 ? 2 : 0)
    + (val & 128 ? 1 : 0);
}

void invert_sprites(void)
{
  int idx = 192;
  int src, dst;

  for (idx = 192; idx < 198; idx++)
  {
    int nidx = idx+6;
    int y;
    for (y = 0; y < 21; y++)
    {
      src = 64*idx+y*3;
      dst = 64*nidx+y*3;
      Poke(dst, inverted(Peek(src+2)));
      Poke(dst+1, inverted(Peek(src+1)));
      Poke(dst+2, inverted(Peek(src)));
    }
  }
}


void decode_note(int k)
{
  // Decode duration and octave.

  // Encoding = dddd dooo nnnn  (where d=duration, o=octave, n=note)
  dr = nm[k] / 128;
  oc = (nm[k] - 128*dr) / 16;  // ok, now I get what they're doing here, overly verbose masking :)

  // Decode note.
  nt = (nm[k] - 128*dr) - 16*oc;   // more overly verbose masking :)
}


void calculate_note_frequency(void)
{
  // Get base frequency for this note.
  fr = fq[nt];

  //if (k == 1) // voice 2 debugging
  //{
    //printf("nm=%d dr=%d oc=%d nt=%d fr=%lu\n", nm, dr, oc, nt, fr);
  //}

  fr <<= 16;

  // If highest octave, skip division loop.
  if (oc != 7)
  {
    int j;
    // Divide base frequency by 2 appropriate number of times.
    for (j = 6; j >= oc; j--)
      fr = fr / 2UL;
  }

  fr >>= 16;

  // Get high and low frequency bytes.
  hf = fr / 256U;
  lf = fr - 256U * hf;
}


void prepare_waveform_control_registers(int k)
{
  // Set waveform controls to proper voice. If silence, set waveform controls to 0.
  wa[k] = v[k];   // set the waveform control to proper voice
  wb[k] = wa[k] - 1; // turn the gate-bit (bit0) of the voice's control-register off (releases the note from the sustain)

  if (nm[k] < 0) // if encoded note value is negative, this equates to silence, so set waveform controls to 0.
  {
    nm[k] = -nm[k]; // invert it, and what remains equates to a duration value
    wa[k] = 0;
    wb[k] = 0;
  }
}


// each loop iteration, let's just decode the bare-minimum of musical
// note information needed (rather than decoding it all up-front!)
int update_decoded_music(void)
{
  static int idx[3] = { 0 };  // index to the current note+octave+duration value of each voice
  static int remain[3] = { 0 };  // the duration remaining for the current note on this voice

  int k;

  // REM Begin decoding loop for each voice.
  // 100 FOR K = 0 TO 2
  for (k = 0; k <= 2; k++)
  {
    int *pvoice;
    if (k==0) pvoice = v1;
    if (k==1) pvoice = v2;
    if (k==2) pvoice = v3;

    if (remain[k] == 0)
    {
      // read the next encoded note
      nm[k] = pvoice[idx[k]];

      // move index in preparation of next note (once this note finishes)
      idx[k]++;

      // If coded note is zero, then end of the song, quit program?
      if (nm[k] == 0)
        return 0;

      // preparing wa and wb vars
      prepare_waveform_control_registers(k);

      // figure out dr, oc & nt (duration, octave and note-within-octave)
      decode_note(k);

      // figure out fr, lf an hf (16-bit frequency and 8-bit LSB and MSB frequency values)
      calculate_note_frequency();

      // prepare start of note details
      l[k] = lf;
      h[k] = hf;
      c[k] = wa[k];
      remain[k] = dr;
    }
    else if (remain[k] == 1) // are we on the last 1/32nd of the present note? Time to gate off?
    {
      c[k] = c[k] & 0xFE; // assure gate-bit is turned off
    }

    remain[k]--;

    //printf("v=%d,l=%d,h=%d,c=%d\n", k, l[k], h[k], c[k]);

    /*
    while (pvoice[idx] != 0)
    {
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

      //if (k == 0)
      {
        //printf("v=%d,i=%d,hf=%d,lf=%d,wa=%d,wb=%d\n",k,i,hf,lf,wa,wb);
        //cgetc();
      }
    }*/
  }

  return 1;
}

void prepare_ADSRs(void)
{
  // 500 POKE S+5, 0 : POKE S+6, 240 : REM Set Attack/Decay for voice 1 (A=0, D=0) : Set Sustain/Release for voice 1 (S=15, R=0)
  Poke(_SID_+5, 0);   // (0xd405) Set Attack/Decay for voice 1 (A=0, D=0)
                      // aaaa dddd (a=attack, d=decay)
  Poke(_SID_+6, (15<<4) + 0); // Set Sustain/Release for voice 1 (S=15, R=0)
                      // 1111 0000
                      // ssss rrrr (s=sustain, r=release)

  // 510 POKE S+12, 85 : POKE S+13, 133 : REM Set Attack/Decay for voice 2 (A=5, D=5) : Set Sustain/Release for voice 2 (S=8,  R=5)
  Poke(_SID_+12, 85);   // (0xd40c) Set Attack/Decay for voice 2 (A=5, D=5)
                        // 0101 0101
                        // aaaa dddd (a=attack, d=decay)
  Poke(_SID_+13, 133);  // (0xd40d) Set Sustain/Release for voice 2 (S=8, R=5)
                        // 1000 0101
                        // ssss rrrr (s=sustain, r=release)

  // 520 POKE S+19, 10 : POKE S+20, 197 : REM Set Attack/Decay for voice 3 (A=0, D=10) : Set Sustain/Release for voice 3 (S=12, R=5)
  Poke(_SID_+19, (0<<4) + 3);   // Set Attack/Decay for voice 3 (A=0, D=10)
                        // 0000 1010
                        // aaaa dddd (a=attack, d=decay)
  Poke(_SID_+20, (0<<4) + 5);  // Set Sustain/Release for voice 3 (S=0, R=5)
                        // 0000 0101
                        // ssss rrrr (s=sustain, r=release)
}

void update_lyrics(int i)
{
  int k;

  if ( ( i % 32 ) == 0)
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
}

void music_loop(void)
{
  unsigned int i = 0;   // the number of 1/32nd measures that have transpired since start of the song
  unsigned int t;  // loop variable used for timers

  //REM Start loop for every 1/32nd of a measure.
  rptcnt = repeat_to_marker_count;

  while (1)
  {
    //__asm__ ( "INC $D020" );

    /*
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
    }*/

    update_lyrics(i);

    update_sprites(i);
  
    update_decoded_music();

    //printf("fr=%u\n", l[0][i] + 256*h[0][i]);
    // 550 POKE S,   L(0, I) : POKE S+7,  L(1, I) : POKE S+14, L(2, I) : REM POKE low frequency from activity array for all voices.
    Poke(_SID_,    l[0]);  // (0xd400) v1 freq lo-byte
    Poke(_SID_+7,  l[1]);  // (0xd407) v2 freq lo-byte
    Poke(_SID_+14, l[2]);  // (0xd40e) v3 freq lo-byte

    // 560 POKE S+1, H(0, I) : POKE S+8,  H(1, I) : POKE S+15, H(2, I) : REM POKE high frequency from activity array for all voices.
    Poke(_SID_+1,  h[0]);  // (0xd401) v1 freq hi-byte
    Poke(_SID_+8,  h[1]);  // (0xd408) v2 freq hi-byte
    Poke(_SID_+15, h[2]);  // (0xd40f) v3 freq hi-byte

    // 570 POKE S+4, C(0, I) : POKE S+11, C(1, I) : POKE S+18, C(2, I) : REM POKE waveform control from activity array for all voices.
    Poke(_SID_+4,  c[0]);  // (0xd404) v1 control register
                              // rpst omzg (r=random noise, p=pulse, s=sawtooth, t=triangle
                              //            o=disable oscillator, m=ring mod. v1 with v3,
                              //            z=synchronize v1 with v3, g=gate bit: 1=start attack+decay+sustain, 0=start release
    Poke(_SID_+11, c[1]);  // (0xd40b) v2 control register
                              // rpst omzg (r=random noise, p=pulse, s=sawtooth, t=triangle
                              //            o=disable oscillator, m=ring mod. v2 with v1,
                              //            z=synchronize v2 with v1, g=gate bit: 1=start attack+decay+sustain, 0=start release
    Poke(_SID_+18, c[2]);  // (0xd412) v3 control register
                              // rpst omzg (r=random noise, p=pulse, s=sawtooth, t=triangle
                              //            o=disable oscillator, m=ring mod. v3 with v2,
                              //            z=synchronize v3 with v2, g=gate bit: 1=start attack+decay+sustain, 0=start release

    /*
    printf("l0=%d,l1=%d,l2=%d\n",l[0][i], l[1][i], l[2][i]);
    printf("h0=%d,h1=%d,h2=%d\n",h[0][i], h[1][i], h[2][i]);
    printf("c0=%d,c1=%d,c2=%d\n",c[0][i], c[1][i], c[2][i]);
    cgetc();
    */

    // 580 FOR T = 1 TO 80 : NEXT : NEXT : REM Timing loop for 1/32nd of a measure and back for next 1/32nd measure.
    for (t = 0; t < 500; t++)
      ;

    i++; // increment to next 1/32nd measure
  }
}

int main(void)
{
  int k;  // generic loop variable
  unsigned int t;  // loop variable used for timers
  int im = 0;   // the total length of the songs (in units of 1/16th measures)

  int j = 0;

  // switch back to upper-case
  // https://www.cc65.org/mailarchive/2004-09/4446.html
  Poke(0xd018, 0x15);

  init_sid();

  // REM Set high pulse width for voice 2 : Set high frequency for filter cutoff : Set resonance for filter and filter voice 3
  // 50 POKE S+10, 8: POKE S+22, 128: POKE S+23, 244 
  Poke(_SID_+10, 8);   // (0xd40a) Set high pulse width for voice 2
  Poke(_SID_+22, 128); // (0xd416) Set high frequency for filter cutoff
  Poke(_SID_+23, 244); // (0xd417) 1111 0100 Set resonance for filter and filter voice 3
                                // rrrr e321 (r=filter resonance, e=filter external input, 1/2/3 = filter voice 1/2/3)

  intro_screen();
  load_petscii();
  invert_sprites();

  //decode_music();

  // Aha! Now we play what has been rendered by all 3 voices in those activity buffers...
  // Sheesh... This seems like such a wasteful way of doing this, but oh well, let's see how it goes in c then...

  prepare_ADSRs();

  // 530 POKE S+24, 31 : REM Set volume 15, low-pass filtering.
  Poke(_SID_+24, 31);   // (0xd418) Set volume 15, low-pass filtering
                        // 0001 1111
                        // chbl vvvv (c=cutoff voice3 output 1=off/0=on, h=select high-pass filter, b=select band-pass filter,
                        // l=select low-pass filter, v = volume)

  music_loop();

  // 590 FOR T = 1 TO 200 : NEXT : POKE S+24, 0 : REM Pause, then turns off volume.
  // Final pause before ending eh...
  for (t = 0; t < 5000; t++)
    ;

  // turn off volume
  Poke(_SID_+24, 0);    // (0xd418) Set volume 15, low-pass filtering
                        // 0000 0000
                        // chbl vvvv (c=cutoff voice3 output 1=off/0=on, h=select high-pass filter, b=select band-pass filter,
                        // l=select low-pass filter, v = volume)


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
