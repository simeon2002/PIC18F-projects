#include <SoftwareSerial.h>

// Create software serial on pins 2(RX) and 3(TX)
SoftwareSerial mySerial(2, 3);
String receivedMessage;
unsigned long lastSendTime = 0;
const unsigned long SEND_INTERVAL = 5000; // Send every 1 second

// Buffer for receiving
const int RX_BUFFER_SIZE = 64;
char rxBuffer[RX_BUFFER_SIZE];
int rxIndex = 0;

void setup() {
  Serial.begin(9600);     // Debug monitor
  mySerial.begin(9600);   // Software serial for UART
}

void loop() {
  // Check for new data but limit how many bytes we process per loop
  int bytesToRead = min(mySerial.available(), 10); // Process max 10 bytes per loop
  
  for(int i = 0; i < bytesToRead; i++) {
    char receivedChar = mySerial.read();
    if (receivedChar == '\n') {
      // Complete message received
      rxBuffer[rxIndex] = '\0'; // Null terminate
      Serial.println("Received: " + String(rxBuffer));
      rxIndex = 0; // Reset for next message
    } else if (rxIndex < RX_BUFFER_SIZE - 1) {
      rxBuffer[rxIndex++] = receivedChar;
    }
  }

  // Check if it's time to send data
  if (millis() - lastSendTime >= SEND_INTERVAL) {
    String message = "Hello from Arduino\n";
    mySerial.print(message);
    Serial.println("Sent: " + message);
    lastSendTime = millis();
  }

  // Check Serial Monitor input
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input += '\n';
    mySerial.print(input);
    Serial.println("Sent: " + input);
  }
}