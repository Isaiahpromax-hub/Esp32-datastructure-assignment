#include <Arduino.h>

/*
 * SensorData structure
 * Stores simulated sensor readings
 * Size: ~12-16 bytes (float:4, float:4, int:4 + padding)
 */
struct SensorData {
  float temperature;
  float humidity;
  int light;
};

// ======================================================
// STEP 1 ORIGINAL FUNCTION ((with memory leak))
// ======================================================
/*
 * ORIGINAL BUGGY FUNCTION - HAS MEMORY LEAK!
 * 
 * PROBLEM: Allocates memory with 'new' but never frees it
 * Each call loses ~16 bytes of heap memory
 * 
 * @return Pointer to newly allocated SensorData
 */
SensorData* readSensorsWithLeak() {
  SensorData* data = new SensorData;  // Allocates heap memory
  data->temperature = random(200, 300) / 10.0;
  data->humidity = random(300, 600) / 10.0;
  data->light = random(0, 1024);
  return data;
}

// ======================================================
// STEP 3 - IMPROVED FUNCTION (pass by reference)
// ======================================================
/*
 * IMPROVED FUNCTION - No dynamic allocation!
 * 
 * Instead of allocating new memory, this function
 * accepts an existing struct by reference and fills it.
 * 
 * @param data Reference to existing SensorData struct to fill
 */
void readSensors(SensorData &data) {
  data.temperature = random(200, 300) / 10.0;
  data.humidity = random(300, 600) / 10.0;
  data.light = random(0, 1024);
  // NO 'new' here! Using memory that already exists
}

// Global struct for STEP 3 solution
SensorData globalSensorData;

// Statistics tracking
int step = 1;
int loopCounter = 0;
long initialHeap = 0;
int leakRate = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  randomSeed(analogRead(0));
  
  // Record initial free heap
  initialHeap = ESP.getFreeHeap();
  
  Serial.println("==================================================");
  Serial.println("QUESTION 2: MEMORY LEAK DETECTION AND PREVENTION");
  Serial.println("==================================================\n");
  
  Serial.println("This demonstration shows 3 steps:\n");
  Serial.println("STEP 1: Memory Leak (no delete) - 5 seconds");
  Serial.println("        Watch Free Heap DECREASE each second!\n");
  Serial.println("STEP 2: Fixed with delete - 5 seconds");
  Serial.println("        Watch Free Heap STABILIZE\n");
  Serial.println("STEP 3: Professional stack-based solution - runs forever");
  Serial.println("        NO new/delete - perfectly stable!\n");
  
  Serial.println("Starting demonstration in 3 seconds...\n");
  delay(3000);
}

void loop() {
  loopCounter++;
  
  // ==================================================
  // STEP 1: Memory Leak Demonstration (First 5 loops)
  // ==================================================
  if (step == 1) {
    // Call function with memory leak
    SensorData* sensor = readSensorsWithLeak();
    
    // Calculate heap change
    long currentHeap = ESP.getFreeHeap();
    long heapLost = initialHeap - currentHeap;
    
    Serial.print(" STEP 1 [LEAK] - Loop ");
    Serial.print(loopCounter);
    Serial.print("/5 | Free Heap: ");
    Serial.print(currentHeap);
    Serial.print(" bytes | Lost: ");
    Serial.print(heapLost);
    Serial.print(" bytes | Temp: ");
    Serial.print(sensor->temperature);
    Serial.print("°C, Hum: ");
    Serial.print(sensor->humidity);
    Serial.print("%, Light: ");
    Serial.println(sensor->light);
    
    // INTENTIONAL LEAK - NO DELETE HERE!
    // Memory is NOT freed - heap keeps decreasing
    
    if (loopCounter >= 5) {
      step = 2;
      loopCounter = 0;
      Serial.println("\n STEP 1 COMPLETE: Memory leak demonstrated!");
      Serial.println("   Notice how Free Heap decreased each time.");
      Serial.println("   Each loop lost ~16 bytes of memory.\n");
      Serial.println("STEP 2: Fixing the leak with 'delete' (Next 5 seconds)");
      Serial.println("-----------------------------------------------------");
      delay(3000);
    }
  }
  
  // ==================================================
  // STEP 2: Fixed with delete (Next 5 loops)
  // ==================================================
  else if (step == 2) {
    // Still using original function but NOW WITH DELETE
    SensorData* sensor = readSensorsWithLeak();
    
    Serial.print(" STEP 2 [FIXED] - Loop ");
    Serial.print(loopCounter);
    Serial.print("/5 | Free Heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.print(" bytes | Temp: ");
    Serial.print(sensor->temperature);
    Serial.print("°C, Hum: ");
    Serial.print(sensor->humidity);
    Serial.print("%, Light: ");
    Serial.println(sensor->light);
    
    // TODO: Step 1 - Add the missing code here to prevent the memory leak.
    delete sensor;  // ← THE FIX! Memory is now freed
    
    if (loopCounter >= 5) {
      step = 3;
      loopCounter = 0;
      Serial.println("\n STEP 2 COMPLETE: Memory leak fixed with 'delete'!");
      Serial.println("   Notice how Free Heap remained stable this time.\n");
      Serial.println("STEP 3: Professional stack-based solution (Runs forever)");
      Serial.println("------------------------------------------------------");
      delay(3000);
    }
  }
  
  // ==================================================
  // STEP 3: Professional solution (Runs forever)
  // ==================================================
  else {
    // TODO: Step 3 - Using modified function that accepts reference
    readSensors(globalSensorData);  // No allocation!
    
    Serial.print(" STEP 3 [PROFESSIONAL] - Loop ");
    Serial.print(loopCounter);
    Serial.print(" | Free Heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.print(" bytes | Temp: ");
    Serial.print(globalSensorData.temperature);
    Serial.print("°C, Hum: ");
    Serial.print(globalSensorData.humidity);
    Serial.print("%, Light: ");
    Serial.println(globalSensorData.light);
    
    // Show analysis every 10 loops
    if (loopCounter % 10 == 0) {
      Serial.println("\n==================== ANALYSIS ====================");
      Serial.println("QUESTION 2: Memory Leak Detection and Prevention");
      Serial.println("==================================================\n");
      
      Serial.println("1. MEMORY LEAK IDENTIFICATION:");
      Serial.println("   ----------------------------------------");
      Serial.println("   • Location: Line 12 - 'new SensorData' in readSensorsWithLeak()");
      Serial.println("   • Problem: Memory allocated every loop() but never freed");
      Serial.println("   • Each allocation: ~16 bytes");
      Serial.println("   • After 1 hour (3600 loops): ~57KB lost!");
      Serial.println("   • ESP32 has limited heap (~320KB) - WILL CRASH!\n");
      
      Serial.println("2. FIX WITH DELETE (Step 2):");
      Serial.println("   ----------------------------------------");
      Serial.println("   • Added 'delete sensor' at line 36");
      Serial.println("   • Memory freed after each use ✓");
      Serial.println("   • Free heap remains stable ✓");
      Serial.println("   • BUT: Still not ideal for long-running systems!");
      Serial.println("   • Why? Constant new/delete causes HEAP FRAGMENTATION");
      Serial.println("   • Memory becomes scattered with small holes");
      Serial.println("   • Eventually can't allocate large blocks\n");
      
      Serial.println("3. PROFESSIONAL SOLUTION (Step 3):");
      Serial.println("   ----------------------------------------");
      Serial.println("   • Created global SensorData struct");
      Serial.println("   • Modified readSensors to accept reference parameter");
      Serial.println("   • NO dynamic allocation at all!");
      Serial.println("   • Memory allocated ONCE at startup");
      Serial.println("   • Zero fragmentation risk ✓");
      Serial.println("   • Faster execution (no new/delete overhead) ✓");
      Serial.println("   • Deterministic - always works ✓");
      Serial.println("   • PREFERRED for embedded systems! ✓\n");
      
      
      Serial.println("==================================================\n");
    }
  }
  
  delay(1000);  // Wait 1 second between readings

}
