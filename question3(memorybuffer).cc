#include <Arduino.h>

#define BUFFER_SIZE 64

// TODO : Implement this templated CircularBuffer class
template <typename T>
class CircularBuffer {
private:
    T buffer[BUFFER_SIZE];
    int head; // Points to the next location to write to
    int tail; // Points to the next location to read from
    int count; // Number of elements currently in the buffer
    unsigned long overflowCount; // Track overflow statistics
    int peakUsage; // Track peak buffer usage

public:
    CircularBuffer() : head(0), tail(0), count(0), overflowCount(0), peakUsage(0) {}

    // TODO : Add an item to the buffer . Return true if successful .
    bool enqueue(T item) {
        // Check for buffer full
        if (isFull()) {
            overflowCount++; // Track overflow for statistics
            return false;
        }
        
        // Add item to buffer
        buffer[head] = item;
        head = (head + 1) % BUFFER_SIZE;
        count++;
        
        // Track peak usage
        if (count > peakUsage) {
            peakUsage = count;
        }
        
        return true;
    }

    // TODO : Remove an item and store it in 'item'. Return true if successful .
    bool dequeue(T& item) {
        // Check for buffer empty
        if (isEmpty()) {
            return false;
        }
        
        // Remove item from buffer
        item = buffer[tail];
        tail = (tail + 1) % BUFFER_SIZE;
        count--;
        
        return true;
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == BUFFER_SIZE;
    }
    
    int size() {
        return count;
    }
    
    // Statistics methods
    unsigned long getOverflowCount() {
        return overflowCount;
    }
    
    int getPeakUsage() {
        return peakUsage;
    }
    
    void resetStatistics() {
        overflowCount = 0;
        peakUsage = 0;
    }
};

CircularBuffer<int> sensorDataBuffer;
unsigned long lastProducerTime = 0;
unsigned long lastConsumerTime = 0;
unsigned long lastStatsTime = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial);
    Serial.println("Starting Circular Buffer Demo...");
    Serial.println("Producer: 1000 Hz (every 1 ms)");
    Serial.println("Consumer: 10 Hz (every 100 ms)");
    Serial.println("Buffer Size: 64 items\n");
}

void loop() {
    unsigned long currentMillis = millis();

    // --- Producer : Simulate a high - speed sensor (e.g., 1000 Hz -> every 1 ms) ---
    if (currentMillis - lastProducerTime >= 1) {
        lastProducerTime = currentMillis;
        int simulatedSensorValue = random(0, 1000);

        // TODO : Try to add the sensor value to the buffer .
        // If the buffer is full , print a warning message " Buffer Overflow !".
        if (!sensorDataBuffer.enqueue(simulatedSensorValue)) {
            Serial.println("Buffer Overflow!");
        }
    }

    // --- Consumer : Simulate a slow processing loop (e.g., 10 Hz -> every 100 ms) ---
    if (currentMillis - lastConsumerTime >= 100) {
        lastConsumerTime = currentMillis;
        int processedValue = 0;
        int itemsProcessedThisCycle = 0;

        // TODO : Process all available data in the buffer
        // While the buffer is not empty , dequeue an item and print it.
        // Keep track of how many items were processed in this cycle .
        while (!sensorDataBuffer.isEmpty()) {
            if (sensorDataBuffer.dequeue(processedValue)) {
                Serial.print("Processed: ");
                Serial.println(processedValue);
                itemsProcessedThisCycle++;
            }
        }

        Serial.print("--- Processed ");
        Serial.print(itemsProcessedThisCycle);
        Serial.println(" items this cycle ---");
    }
    
    // --- Statistics Display (every 1 second) ---
    if (currentMillis - lastStatsTime >= 1000) {
        lastStatsTime = currentMillis;
        
        Serial.println("\n--- Buffer Statistics ---");
        Serial.print("Current buffer size: ");
        Serial.println(sensorDataBuffer.size());
        Serial.print("Peak buffer usage: ");
        Serial.print(sensorDataBuffer.getPeakUsage());
        Serial.print("/");
        Serial.println(BUFFER_SIZE);
        Serial.print("Total overflows: ");
        Serial.println(sensorDataBuffer.getOverflowCount());
        
        // Calculate and display data loss percentage
        int totalItems = sensorDataBuffer.getOverflowCount() + 
                        (sensorDataBuffer.getPeakUsage() > 0 ? sensorDataBuffer.getPeakUsage() : 0);
        if (totalItems > 0) {
            float lossPercentage = (float)sensorDataBuffer.getOverflowCount() / totalItems * 100;
            Serial.print("Data loss: ");
            Serial.print(lossPercentage, 2);
            Serial.println("%");
        }
        
        Serial.println("-------------------------\n");
        
        // Optional: Reset statistics periodically
        // sensorDataBuffer.resetStatistics();
    }
}
