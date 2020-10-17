#include <Bounce2.h>

const int rowCount = 5;
const int colCount = 5;

const int debounce = 5;
const int scan_delay = 5;
 
Bounce switches[rowCount * colCount];

boolean buttonStatus[rowCount * colCount]; //store button status so that inputs can be released
boolean keyStatus[rowCount * colCount];    //store keyboard status so that keys can be released

const int colPins[] = {8, 9, 10, 11, 12}; //Arduino pins
const int rowPins[] = {2, 3, 4, 5, 6};    //Arduino pins

// Keycodes for qwerty input
const char layer_rows[] = {
  'Q', 'W', 'E', 'R', 'T',
  'A', 'S', 'D', 'F', 'G',
  'Z', 'X', 'C', 'V', 'B'
};

void setup() {
  //start serial connection
  Serial.begin(115200);

  for (int i = 0; i < rowCount * colCount; i++) {
    // create debouncers for row pins
    Bounce debouncer = Bounce();
    debouncer.attach(rowPins[i % rowCount]);
    debouncer.interval(debounce);
    switches[i] = debouncer;
  }
  
  for(int x=0; x<colCount; x++) {
    Serial.print(colPins[x]); Serial.println(" as input");
    pinMode(colPins[x], INPUT);
  }

  for (int x=0; x<rowCount; x++) {
    Serial.print(rowPins[x]); Serial.println(" as input-pullup");
    pinMode(rowPins[x], INPUT_PULLUP);
  }
}

void loop() {
  scanMatrix();
  delay(scan_delay);
}

void scanMatrix() {
  int i;

  for (i = 0; i < rowCount * colCount; i++) {
    prepareMatrixRead(i);
    switches[i].update();
    if (switches[i].fell()) {
      matrixPress(i);
    } else if (switches[i].rose()) {
      matrixRelease(i);
    }
  }
}

/*Returns input pin to be read by keyScan method
  Param key is the keyboard matrix scan code (col * ROW_COUNT + row)*/
void prepareMatrixRead(int key) {
  static int currentCol = 0;
  int p = key / rowCount;
  if (p != currentCol) {
    pinMode(colPins[currentCol], INPUT);
    pinMode(colPins[p], OUTPUT);
    digitalWrite(colPins[p], LOW);
    currentCol = p;
  }
}

void matrixPress(int keyCode) {
  keyPress(keyCode);
  //keystrokePress(keyCode);
}

void matrixRelease(int keyCode) {
  keyRelease(keyCode);
  //keystrokeRelease(keyCode);
}

void keyPress(int keyCode) {
  Serial.print(layer_rows[keyCode]);
  keyStatus[keyCode]=true;
}

void keyRelease(int keyCode) {
  //Keyboard.release(layer[keyCode]);
  keyStatus[keyCode]=false;
}
