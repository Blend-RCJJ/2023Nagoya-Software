#ifndef _MLT8530_H_
#define _MLT8530_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "../kit/IO-Kit.h"

#define _DO 262
#define _DOS 277
#define _RE 294
#define _REF 311
#define _MI 330
#define _FA 349
#define _FAS 370
#define _SO 392
#define _SOS 415
#define _RA 440
#define _RAS 466
#define _SHI 494
#define DO 523
#define DOS 554
#define RE 587
#define RES 622
#define MI 659
#define FA 698
#define FAS 740
#define SO 784
#define SOS 831
#define RA 880
#define RAS 932
#define SHI 988
#define DO_ 1047
#define DOS_ 1109
#define RE_ 1175
#define RES_ 1245
#define MI_ 1319
#define FA_ 1397
#define FAS_ 1480
#define SO_ 1568
#define SOS_ 1661
#define RA_ 1760
#define RAS_ 1865
#define SHI_ 1976
class MLT8530 {
   public:
    Output pin = PB6;

    int bpm = 170;

    void setFrequency(int freq);
    void mute(void);

    void bootSound(void);
    void checkSound(void);
    void beat(int freq, double beat);

    void matsukenShogun(void);
    void matsukenSamba(void);
};

#endif