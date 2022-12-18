#include "./unitV.h"
#include "./VL53L0X.h"

extern HardwareSerial uart1;
extern VL53L0X distanceSensor;

UNITV::UNITV(HardwareSerial *ptr) {
    serialPtr = ptr;
    serialPtr->begin(115200);
}

void UNITV::read(void) {
    if (millis() > firstReadTime + 5000) {
        if (isVictimDetected) {
            lastUpdatingTime = millis();
        }

        isVictimDetected = false;
    }

    if (serialPtr->available()) {
        char data = serialPtr->read();

        //生死判定用
        if (data == 'H' || data == 'U' || data == 'S' || data == 'R' || data == 'G' || data == 'Y') {
            uart1.println(data);
            if (!isVictimDetected && millis() > lastUpdatingTime + 5000) {
                firstReadTime = millis();
                isVictimDetected = true;
            }
        }

        return;

        // if (millis() < lastUpdatingTime + 10000) {
        //     return;  // 10秒以内に更新されたら無視
        // }

        // switch (data) {
        //     case VICTIM_H:
        //         victimTypeList[victimCount] = VICTIM_H;
        //         break;
        //     case VICTIM_U:
        //         victimTypeList[victimCount] = VICTIM_U;
        //         break;
        //     case VICTIM_S:
        //         victimTypeList[victimCount] = VICTIM_S;
        //         break;
        //     case VICTIM_RED:
        //         victimTypeList[victimCount] = VICTIM_RED;
        //         break;
        //     case VICTIM_GREEN:
        //         victimTypeList[victimCount] = VICTIM_GREEN;
        //         break;
        //     case VICTIM_YELLOW:
        //         victimTypeList[victimCount] = VICTIM_YELLOW;
        //         break;
        // }

        // victimCount++;
        // if (victimCount == 9) {
        //     isVictimDetected = true;
        //     victimCount = 0;
        //     lastUpdatingTime = millis();

        //     // victimTypeListの中で最も多く出現したものをvictimTypeに代入
        //     int victimTypeCount[6] = {0};
        //     for (int i = 0; i < 10; i++) {
        //         switch (victimTypeList[i]) {
        //             case VICTIM_H:
        //                 victimTypeCount[0]++;
        //                 break;
        //             case VICTIM_U:
        //                 victimTypeCount[1]++;
        //                 break;
        //             case VICTIM_S:
        //                 victimTypeCount[2]++;
        //                 break;
        //             case VICTIM_RED:
        //                 victimTypeCount[3]++;
        //                 break;
        //             case VICTIM_GREEN:
        //                 victimTypeCount[4]++;
        //                 break;
        //             case VICTIM_YELLOW:
        //                 victimTypeCount[5]++;
        //                 break;
        //         }
        //     }

        //     int max = 0;
        //     for (int i = 0; i < 6; i++) {
        //         if (victimTypeCount[i] > max) {
        //             max = i;
        //         }
        //     }

        //     switch (max) {
        //         case 0:
        //             victimType = VICTIM_H;
        //             break;
        //         case 1:
        //             victimType = VICTIM_U;
        //             break;
        //         case 2:
        //             victimType = VICTIM_S;
        //             break;
        //         case 3:
        //             victimType = VICTIM_RED;
        //             break;
        //         case 4:
        //             victimType = VICTIM_GREEN;
        //             break;
        //         case 5:
        //             victimType = VICTIM_YELLOW;
        //             break;
        //     }
        // }
    }
}