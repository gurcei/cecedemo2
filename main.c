#include <stdio.h>
#include <conio.h>

#define Poke(A,X)  (*(unsigned char *)(A)) = (X)
#define Peek(A)    (*(unsigned char *)(A))
#define PokeW(A,X) (*(unsigned int *)(A)) = (X)
#define PeekW(A)   (*(unsigned int *)(A))

#define _SID_ 0xD400

unsigned int addr;

void init_sid(void)
{
  for (addr = _SID_; addr <= (_SID_+24); addr++)
  {
    Poke(addr, 0);
  }
}


// voice 1
int v1[] =
{
  594, 594, 594, 596, 596,
  1618, 587, 592, 587, 585, 331, 336,
  1097, 583, 585, 585, 585, 587, 587,
  1609, 585, 331, 337, 594, 594, 593,
  1618, 594, 596, 594, 592, 587,
  1616, 587, 585, 331, 336, 841, 327,
  1607, 0
};

// voice 2
int v2[] =
{
  583, 585, 583, 583, 327, 329,
  1611, 583, 585, 578, 578, 578,
  196, 198, 583, 326, 578,
  326, 327, 329, 327, 329, 326, 578, 583,
  1606, 582, 322, 324, 582, 587,
  329, 327, 1606, 583,
  327, 329, 587, 331, 329,
  329, 328, 1609, 578, 834,
  324, 322, 327, 585, 1602,
  0
};

// voice 3
int v3[] =
{
  567, 566, 567, 304, 306, 308, 310,
  1591, 567, 311, 310, 567,
  306, 304, 299, 308,
  304, 171, 176, 306, 291, 551, 306, 308,
  310, 308, 310, 306, 295, 297, 299, 304,
  1586, 562, 567, 310, 315, 311,
  308, 313, 297,
  1586, 567, 560, 311, 309,
  308, 309, 306, 308,
  1577, 299, 295, 306, 310, 310, 304,
  562, 546, 1575,
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


// 40 V(0) = 17: V(1) = 65: V(2) = 33 : REM Store waveform control byte for each voice
unsigned int v[3] = { 17, 65, 33 };


// 20 DIM H(2,200), L(2,200), C(2,200)  : REM Dimension array to contain activity of song, 1/16th of a measure per location
unsigned char h[2][200];
unsigned char l[2][200];
unsigned char c[2][200];

// H = high-byte of frequency
// L = low-byte of frequency
// C = control byte of waveform

int main(void)
{
  int t;  // loop variable used for timers
  int i;  // generic loop variable
  int k;  // generic loop variable
  int im = 0;   // the total length of the songs (in units of 1/16th measures)

  // switch back to upper-case
  // https://www.cc65.org/mailarchive/2004-09/4446.html
  Poke(0xd018, 0x15);

  // 10 S = 54272 : FOR L = S TO S+24 : POKE L, 0 : NEXT
  init_sid();

#if 1
  // 50 POKE S+10, 8: POKE S+22, 128: POKE S+23, 244 : REM Set high pulse width for voice 2 : Set high frequency for filter cutoff : Set resonance for filter and filter voice 3
  Poke(_SID_+10U, 8);   // Set high pulse width for voice 2
  Poke(_SID_+22U, 128); // Set high frequency for filter cutoff
  Poke(_SID_+23U, 244); // Set resonance for filter and filter voice 3

  
  // 100 FOR K = 0 TO 2 : REM Begin decoding loop for each voice.
  for (k = 0; k <= 2; k++)
  {
    // 110 I=0 : REM Initialise pointer to activity array.
    int i = 0;
    int idx = 0;

    int *pvoice;
    if (k==0) pvoice = v1;
    if (k==1) pvoice = v2;
    if (k==2) pvoice = v3;

    printf("\n\n");

    while (pvoice[idx] != 0)
    {
      int j = 0;
      int wa = 0;
      int wb = 0;
      int dr = 0;
      int oc = 0;
      int nt = 0;
      long fr = 0;
      unsigned int hf;
      unsigned int lf;

      // 120 READ NM : REM Read coded note
      int nm = pvoice[idx];

      // 130 IF NM = 0 THEN 250 : REM If coded note is zero, then next voice.
      if (nm == 0)
        break;

      // 140 WA = V(K) : WB = WA - 1 : IF NM < 0 THEN NM = -NM : WA = 0 : WB = 0 : REM Set waveform controls to proper voice. If silence, set waveform controls to 0.
      wa = v[k]; // set the waveform control to proper voice
      wb = wa - 1;

      if (nm < 0) // if encoded note value is negative, this equates to silence, so set waveform controls to 0.
      {
        printf("less\n");
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

      if (k == 0) // voice 1 debugging
      {
        printf("nm=%d dr=%d oc=%d nt=%d fr=%d ", nm, dr, oc, nt, fr);
      }

      // 180 IF OC% = 7 THEN 200 : REM If highest octave, skip division loop.
      if (oc != 7)
      {
        int j;
        // 190 FOR J=6 TO OC% STEP -1: FR = FR / 2: NEXT : REM Divide base frequency by 2 appropriate number of times.
        for (j = 6; j != oc; j--)
          fr = fr / 2;
      }

      //fr = fr / 65536;

      // 200 HF% = FR / 256 : LF% = FR - 256 * HF% : REM Get high and low frequency bytes.
      hf = fr / 256;
      lf = fr - 256 * hf;

      // 210 IF DR% = 1 THEN H(K,I) = HF% : L(K, I) = LF% : C(K, I) = WA : I=I+1 : GOTO 120: REM If sixteenth note, set activity array: high frequency, low frequency and waveform control (voice on)
      if (dr == 1)
      {
        h[k][i] = hf;
        l[k][i] = lf;
        c[k][i] = wa;
        i++;
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
  for (i = 0; i <= im; i++)
  {
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

    // 580 FOR T = 1 TO 80 : NEXT : NEXT : REM Timing loop for 1/16th of a measure and back for next 1/16th measure.
    for (t = 0; t < 1000; t++)
      ;
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

  while (1)
  {
    //printf("hello world!\n");
    //unsigned char key = cgetc();
    //printf("key = %u\n", key);
  }

#endif

  return 0;
}
