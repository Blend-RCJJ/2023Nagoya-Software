#include "./SLAM-Kit.h"

SLAM_Kit::SLAM_Kit(/* args */) {
}

void SLAM_Kit::updatePosition(int *dataPtr, int gyro) {
    // データの更新と差分の計算
    for (int i = 0; i < 12; i++) {
        difference[i] = dataPtr[i] - distance[i];
        distance[i] = dataPtr[i];
    }
    this->angle = gyro;

    reliablenessEvaluation();
    integration();
}

void SLAM_Kit::reliablenessEvaluation(void) {
    // 信頼性評価
    for (int i = 0; i < 6; i++) {
        if (abs(difference[i] + difference[i + 6]) < allowanceValue &&
            difference[i] * difference[i + 6] < 0) {  // 誤差判定と符号判定
            reliableness[i] = true;
        } else {
            reliableness[i] = false;
        }
    }
}

void SLAM_Kit::integration(void) {
    // dがつくものは差分を表す
    dxRaw = 0;
    dyRaw = 0;

    for (int i = 0; i < 6; i++) {
        if (reliableness[i]) {
            int dAverage_i = (difference[i] - difference[i + 6]) / 2;
            int dx_i = round(sin(radians(30 * i)) * dAverage_i);
            int dy_i = round(cos(radians(30 * i)) * dAverage_i);

            dxRaw = abs(dxRaw) < abs(dx_i) ? dx_i : dxRaw;
            dyRaw = abs(dyRaw) < abs(dy_i) ? dy_i : dyRaw;
        }
    }

    if (abs(dxRaw) < 10 || abs(dxRaw) > 200) {
        dxRaw = 0;
    }

    if (abs(dyRaw) < 10 || abs(dyRaw) > 200) {
        dyRaw = 0;
    }

    double gain = 0.2;

    dx = gain * dx + (1.0 - gain) * dxRaw;
    dy = gain * dy + (1.0 - gain) * dyRaw;

    if (abs(dx) > 2) {
        x += dx;
    }

    if (abs(dy) > 2) {
        y += dy;
    }
}