#include <mcp_can.h>
#include <SPI.h>

/* 
 * MKS ODrive Mini - Dual Axis CAN Controller (TORQUE HOMING EDITION)
 * Arduino Mega 2560 + MCP2515 (CS Pin 53)
 * 
 * STRATEGY: BLIND TORQUE HOMING (Zero AFTER Relax)
 * 1. Push to wall with torque (3s)
 * 2. Release + IDLE (2s relax)
 * 3. Zero HERE (relaxed = 0.0)
 * 4. Engage position mode
 * 5. Command target
 * 
 * Ratio: 5.0 (Physical)
 * Pole Pairs: 20
 */

// =============== USER CONFIG ===============
const int   SPI_CS_PIN  = 53;
const int   NODE_M0     = 0;
const int   NODE_M1     = 1;

// Individual Gear Ratios
const float GEAR_RATIO_0 = 5.0; 
const float GEAR_RATIO_1 = 5.0; 

// Individual Directions
const int   MOVE_DIR_0  = -1;   
const int   MOVE_DIR_1  = -1;   

// Homing Parameters
const float HOME_TORQUE = 0.3;  // Nm
const int   HOME_DIR    = 1;    // +1 = Push to Wall
const unsigned long LOOP_MS = 3000;
// ===========================================

MCP_CAN CAN0(SPI_CS_PIN);
int loopStep = 0;
unsigned long lastMoveTime = 0;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) 
    Serial.println(F("CAN OK"));
  else { Serial.println(F("CAN FAIL")); while(1); }
  CAN0.setMode(MCP_NORMAL);
  delay(200);

  // --- BOOT DELAY ---
  Serial.println(F("Waiting for motors (8s boot delay)..."));
  unsigned long t = millis();
  while (millis() - t < 8000) { 
    drainCAN();
    delay(10);
  }

  // =============================================
  // === HOMING (BLIND TORQUE) ===
  // =============================================
  Serial.println(F("=== HOMING (BLIND TORQUE) ==="));
  
  // 1. Idle + Clear
  sendAxisState(NODE_M0, 1);
  sendAxisState(NODE_M1, 1);
  delay(50);
  sendClearErrors(NODE_M0);
  sendClearErrors(NODE_M1);
  delay(200);
  
  // 2. Torque Mode (Control=1, Input=1)
  Serial.println(F("Engaging TORQUE MODE..."));
  sendControlMode(NODE_M0, 1, 1); 
  sendControlMode(NODE_M1, 1, 1);
  delay(50);
  
  // 3. Closed Loop
  sendAxisState(NODE_M0, 8);
  sendAxisState(NODE_M1, 8);
  delay(500);
  
  // 4. Push to Wall
  float torque = HOME_TORQUE * HOME_DIR;
  Serial.print(F("Pushing Wall (Nm): ")); Serial.println(torque);
  sendTorque(NODE_M0, torque);
  sendTorque(NODE_M1, torque);
  
  // 5. Wait for Stall (Blind)
  Serial.println(F("Waiting 3s for stall..."));
  unsigned long startTime = millis();
  while (millis() - startTime < 3000) {
    drainCAN();
    delay(10);
  }
  
  // 6. Release Torque
  Serial.println(F("Releasing torque..."));
  sendTorque(NODE_M0, 0.0f);
  sendTorque(NODE_M1, 0.0f);
  delay(500);

  // 7. IDLE (Let strings relax)
  Serial.println(F("Relaxing (IDLE for 2s)..."));
  sendAxisState(NODE_M0, 1); 
  sendAxisState(NODE_M1, 1);
  delay(2000);
  
  // 8. Clear Errors
  sendClearErrors(NODE_M0);
  sendClearErrors(NODE_M1);
  delay(200);

  // =============================================
  // === ZERO HERE (AT RELAXED POSITION) ===
  // This is the KEY fix: 0.0 = relaxed, NOT wall.
  // =============================================
  Serial.println(F(">>> ZEROING AT RELAXED POSITION <<<"));
  for(int k=0; k<5; k++) {
    sendSetLinearCount(NODE_M0, 0);
    sendSetLinearCount(NODE_M1, 0);
    delay(20);
  }
  delay(200);
  Serial.println(F("Encoder Zeroed (Relaxed = 0.0)"));

  // =============================================
  // === ENGAGE POSITION MODE ===
  // =============================================
  
  // Set Position Mode (Control=3) + Trap Traj (Input=5)
  sendControlMode(NODE_M0, 3, 5); 
  sendControlMode(NODE_M1, 3, 5);
  delay(50);
  
  // Set input position to 0.0 (we ARE at 0.0, no jump)
  sendPosition(NODE_M0, 0.0f);
  sendPosition(NODE_M1, 0.0f);
  delay(50);
  
  // Engage Closed Loop
  Serial.println(F("Engaging Closed Loop..."));
  sendAxisState(NODE_M0, 8);
  sendAxisState(NODE_M1, 8);
  delay(1000);

  // =============================================
  // === FIRST MOVE ===
  // =============================================
  Serial.println(F("Moving to 90 deg..."));
  moveToAngle(NODE_M0, 90.0);
  moveToAngle(NODE_M1, 90.0);
  delay(4000);
  
  Serial.println(F("=== READY ==="));
  lastMoveTime = millis();
}

void loop() {
  drainCAN();
  
  if (millis() - lastMoveTime > LOOP_MS) {
    lastMoveTime = millis();
    if (loopStep == 0) {
      Serial.println(F("-> Moving to 45 deg"));
      moveToAngle(NODE_M0, 45.0);
      moveToAngle(NODE_M1, 45.0);
      loopStep = 1;
    } else {
      Serial.println(F("-> Moving to 90 deg"));
      moveToAngle(NODE_M0, 90.0);
      moveToAngle(NODE_M1, 90.0);
      loopStep = 0;
    }
  }
}

// ============== HELPERS ==============

void moveToAngle(int node, float degrees) {
  float ratio = (node == 0) ? GEAR_RATIO_0 : GEAR_RATIO_1;
  int dir = (node == 0) ? MOVE_DIR_0 : MOVE_DIR_1;
  
  float motorTurns = (degrees / 360.0) * ratio * dir;
  sendPosition(node, motorTurns);
  
  Serial.print(F("Node")); Serial.print(node);
  Serial.print(F(" -> ")); Serial.print(motorTurns, 3);
  Serial.println(F("t"));
}

void drainCAN() {
  while(CAN0.checkReceive() == CAN_MSGAVAIL) {
    long unsigned int rxId; unsigned char len, buf[8];
    CAN0.readMsgBuf(&rxId, &len, buf);
    int node = rxId >> 5;
    int cmd = rxId & 0x1F;
    
    if (cmd == 0x01 && len >= 4) {
      uint32_t err; memcpy(&err, buf, 4);
      if (err != 0) {
        Serial.print(F("ERR N")); Serial.print(node);
        Serial.print(F(":0x")); Serial.println(err, HEX);
      }
    }
  }
}

// ============== CAN COMMANDS ==============

void sendAxisState(int node, int32_t state) {
  unsigned long id = ((unsigned long)node << 5) | 0x007;
  byte d[8] = {0}; memcpy(d, &state, 4);
  CAN0.sendMsgBuf(id, 0, 8, d);
}

void sendControlMode(int node, int32_t ctrl, int32_t input) {
  unsigned long id = ((unsigned long)node << 5) | 0x00B;
  byte d[8] = {0}; memcpy(d, &ctrl, 4); memcpy(d+4, &input, 4);
  CAN0.sendMsgBuf(id, 0, 8, d);
}

void sendTorque(int node, float torque) {
  unsigned long id = ((unsigned long)node << 5) | 0x00E;
  byte d[8] = {0}; memcpy(d, &torque, 4);
  CAN0.sendMsgBuf(id, 0, 8, d);
}

void sendPosition(int node, float pos) {
  unsigned long id = ((unsigned long)node << 5) | 0x00C;
  byte d[8] = {0}; memcpy(d, &pos, 4);
  CAN0.sendMsgBuf(id, 0, 8, d);
}

void sendSetLinearCount(int node, int32_t count) {
  unsigned long id = ((unsigned long)node << 5) | 0x013;
  byte d[8] = {0}; memcpy(d, &count, 4);
  CAN0.sendMsgBuf(id, 0, 8, d);
}

void sendClearErrors(int node) {
  unsigned long id = ((unsigned long)node << 5) | 0x018;
  byte d[8] = {0};
  CAN0.sendMsgBuf(id, 0, 8, d);
}

