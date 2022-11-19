import processing.serial.*;

PFont font;

Serial serial;

int[] dist = new int[12];
float[] distFiltered = new float[12];

float scale;

boolean isFirst = true;

void setup() {
  //size(1200, 700);
  fullScreen();
  font = createFont("data/SF-Mono-Heavy.otf", 250);
  textFont(font);

  scale = (float)width / 6400.0 * 2;
  //scale = 2;
  serial = new Serial(this, "/dev/cu.usbmodem14101", 2000000);

  background(#101010);
  //stroke(#00BCD4, 150);
}

void draw() {
  if (isFirst) {
    translate(width / 2, height / 2);

    colorMode(HSB, 255);
    for (int i = 0; i <= 255; i+=1) {
      stroke(i, 140, 255, 100);
      strokeWeight(30);
      noFill();
      ellipse(0, 0, i * 12, i * 12);
    }
    isFirst = false;
  } else {
    noStroke();
    //tint( 255.0, 20 );

    fill(#151515, 40);
    rect(0, 0, width, height);
    //image( img, (width - img.width)/2, 0 );

    if (serial.available() >= 26) {
      if (serial.read() == 'F') {
        if (serial.read() == 'l') {
          for (int i = 0; i < 12; i++) {
            dist[i] = serial.read() * 256 + serial.read();
          }
        }
      }

      serial.clear();
      while (serial.available() > 0) {
        int trash = serial.read();
      }
    }

    for (int i = 0; i < 12; i++) {
      if (dist[i] > 1000) {
        dist[i] = 1000;
      }
      distFiltered[i] = 0.7 * distFiltered[i] + 0.3 * dist[i];
    }


    translate(width / 2, height / 2);



    colorMode(RGB, 255);

    fill(#FFFFFF, 80);
    textSize(250);
    text("VL53L0X", -width/2 + 30, height/2 - 40);
    
    textSize(80);
    text("NUMBER OF SENSORS:", 120, height/2 - 40);
    textSize(170);
    text("12", 1020, height/2 - 40);

    noFill();

    stroke(#FFFFFF, 10);
    strokeWeight(2);

    for (int i = 1; i <= 4; i++) {
      ellipse(0, 0, 1000 * i * scale, 1000 * i * scale);
    }
    line(0, height, 0, -height);
    line(width, 0, -width, 0);



    colorMode(HSB, 255);

    strokeWeight(50);
    for (int i = 0; i < 12; i++) {
      //dist[i] = 500;

      stroke((distFiltered[i] + distFiltered[(i+1)%12]) / 25 * 1.3, 140, 255, 40);
      line(cos(radians(i * 30 - 90)) * distFiltered[i] * scale, sin(radians(i * 30 - 90)) * distFiltered[i] * scale, cos(radians((i+ 1) * 30 - 90)) * distFiltered[(i + 1)%12] * scale, sin(radians((i + 1) * 30 - 90)) * distFiltered[(i + 1)%12] * scale);
    }
  }
}
