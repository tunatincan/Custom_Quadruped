#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 53;
const int NODE_M0 = 0;
MCP_CAN CAN0(SPI_CS_PIN);

void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) Serial.println("CAN OK");
  else { Serial.println("CAN FAIL"); while(1); }
  CAN0.setMode(MCP_NORMAL);
  delay(100);

  Serial.println("=== RATIO CALIBRATION (3 TURNS) ===");
  Serial.println("1. Starting (Clear Errors)...");
  sendClearErrors(NODE_M0);
  delay(200);

  Serial.println("2. Setting Position Mode...");
  sendControlMode(NODE_M0, 3, 1); // Pos + Passthrough
  sendAxisState(NODE_M0, 8); // Closed Loop
  delay(1000);

  Serial.println("3. Zeroing Position...");
  sendSetLinearCount(NODE_M0, 0);
  sendPosition(NODE_M0, 0.0);
  delay(1000);

  Serial.println("4. MOVING MOTOR EXACTLY 3.0 TURNS...");
  // Move 3.0 Motor Turns slowly (over 3 seconds)
  for(int i=0; i<=30; i++) {
    float pos = i * 0.1; // 0.0 -> 3.0
    sendPosition(NODE_M0, pos);
    delay(100); // Slower updates
  }
  
  Serial.println("DONE! Moved 3.0 Motor Turns.");
  Serial.println("===============================");
  Serial.println("Please Look at the OUTPUT SHAFT:");
  Serial.println("How many full rotations did it make?");
  Serial.println("Calculate Ratio = 3.0 / Output_Rotations");
  Serial.println("Example: If Output spun 1.0 time  -> Ratio = 3.0 / 1.0 = 3.0");
  Serial.println("Example: If Output spun 0.4 times -> Ratio = 3.0 / 0.4 = 7.5");
  Serial.println("===============================");
}

void loop() {}

// Helpers
void sendClearErrors(int node) {
  unsigned long id = ((unsigned long)node << 5) | 0x018;
  byte d[8] = {0}; CAN0.sendMsgBuf(id, 0, 8, d);
}
void sendControlMode(int node, int32_t c, int32_t i) {
  unsigned long id = ((unsigned long)node << 5) | 0x00B;
  byte d[8] = {0}; memcpy(d, &c, 4); memcpy(d+4, &i, 4); CAN0.sendMsgBuf(id, 0, 8, d);
}
void sendAxisState(int node, int32_t s) {
  unsigned long id = ((unsigned long)node << 5) | 0x007;
  byte d[8] = {0}; memcpy(d, &s, 4); CAN0.sendMsgBuf(id, 0, 8, d);
}
void sendSetLinearCount(int node, int32_t c) {
  unsigned long id = ((unsigned long)node << 5) | 0x013;
  byte d[8] = {0}; memcpy(d, &c, 4); CAN0.sendMsgBuf(id, 0, 8, d);
}
void sendPosition(int node, float p) {
  unsigned long id = ((unsigned long)node << 5) | 0x00C;
  byte d[8] = {0}; memcpy(d, &p, 4); CAN0.sendMsgBuf(id, 0, 8, d);
}

