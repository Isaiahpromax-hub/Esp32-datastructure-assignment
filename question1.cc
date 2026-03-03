#include <Arduino.h>
#define MAX_PLAYLIST_SIZE 10

// TYPE DEFINITIONS
struct Song {
    char title[50];
    char artist[50];
    int duration; // in seconds
};

// Array-based Playlist Implementation
struct ArrayList {
    Song songs[MAX_PLAYLIST_SIZE];
    int size = 0;
};

// Define the SongNode struct for linked list
struct SongNode {
    Song song;
    SongNode* next;
};

struct LinkedList {
    SongNode* head = nullptr;
};

// ====== FUNCTION PROTOTYPES ======
// Array list functions
void addSong(ArrayList* list, const Song& newSong);
void removeSong(ArrayList* list, int index);
void displayPlaylist(const ArrayList* list);

// Linked list functions
void addSong(LinkedList* list, const Song& newSong);
void removeSong(LinkedList* list, int index);
void displayPlaylist(const LinkedList* list);

// TODO: Add a song to the end of the array list. Check for overflow.
void addSong(ArrayList* list, const Song& newSong) {
    // Check if there's space in the array
    if (list->size < MAX_PLAYLIST_SIZE) {
        list->songs[list->size] = newSong;
        list->size++;
        Serial.println("Song added to array playlist");
    } else {
        Serial.println("Error: Playlist is full! Cannot add more songs.");
    }
}

// TODO: Remove a song at a given index (0-based). Shift elements left.
void removeSong(ArrayList* list, int index) {
    // Check if index is valid
    if (index < 0 || index >= list->size) {
        Serial.println("Error: Invalid index!");
        return;
    }
    
    // Shift all elements after index to the left
    for (int i = index; i < list->size - 1; i++) {
        list->songs[i] = list->songs[i + 1];
    }
    list->size--;
    Serial.println("Song removed from array playlist");
}

// TODO: Print all songs in the list.
void displayPlaylist(const ArrayList* list) {
    Serial.println("\n--- Array Playlist ---");
    
    if (list->size == 0) {
        Serial.println("Playlist is empty");
        return;
    }
    
    for (int i = 0; i < list->size; i++) {
        Serial.print(i);
        Serial.print(": ");
        Serial.print(list->songs[i].title);
        Serial.print(" - ");
        Serial.print(list->songs[i].artist);
        Serial.print(" [");
        Serial.print(list->songs[i].duration / 60);
        Serial.print(":");
        
        // Format seconds with leading zero
        int seconds = list->songs[i].duration % 60;
        if (seconds < 10) Serial.print("0");
        Serial.print(seconds);
        Serial.println("]");
    }
}

// --- Linked List-based Playlist Implementation ---

// TODO: Add a song to the end of the linked list.
void addSong(LinkedList* list, const Song& newSong) {
    // Create a new node dynamically
    SongNode* newNode = new SongNode;
    newNode->song = newSong;
    newNode->next = nullptr;
    
    if (list->head == nullptr) {
        // List is empty - new node becomes the head
        list->head = newNode;
    } else {
        // Find the last node
        SongNode* current = list->head;
        while (current->next != nullptr) {
            current = current->next;
        }
        // Add new node at the end
        current->next = newNode;
    }
    Serial.println("Song added to linked list playlist");
}

// TODO: Remove a song at a given index (0-based).
void removeSong(LinkedList* list, int index) {
    // Check if list is empty
    if (list->head == nullptr) {
        Serial.println("Error: Playlist is empty!");
        return;
    }
    
    // Case 1: Remove the first node (index 0)
    if (index == 0) {
        SongNode* temp = list->head;
        list->head = list->head->next;
        delete temp; // Free memory
        Serial.println("Song removed from linked list playlist");
        return;
    }
    
    // Case 2: Remove node at position > 0
    // Find the node just BEFORE the one to remove
    SongNode* current = list->head;
    int currentIndex = 0;
    
    while (current != nullptr && currentIndex < index - 1) {
        current = current->next;
        currentIndex++;
    }
    
    // Check if index is valid
    if (current == nullptr || current->next == nullptr) {
        Serial.println("Error: Invalid index!");
        return;
    }
    
    // Remove the node
    SongNode* temp = current->next;
    current->next = temp->next;
    delete temp; // Free memory
    Serial.println("Song removed from linked list playlist");
}

// TODO: Print all songs in the list.
void displayPlaylist(const LinkedList* list) {
    Serial.println("\n--- Linked List Playlist ---");
    
    if (list->head == nullptr) {
        Serial.println("Playlist is empty");
        return;
    }
    
    SongNode* current = list->head;
    int index = 0;
    
    while (current != nullptr) {
        Serial.print(index);
        Serial.print(": ");
        Serial.print(current->song.title);
        Serial.print(" - ");
        Serial.print(current->song.artist);
        Serial.print(" [");
        Serial.print(current->song.duration / 60);
        Serial.print(":");
        
        // Format seconds with leading zero
        int seconds = current->song.duration % 60;
        if (seconds < 10) Serial.print("0");
        Serial.print(seconds);
        Serial.println("]");
        
        current = current->next;
        index++;
    }
}

// ====== SETUP ======
void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for serial connection
    
    Serial.println("==========================================");
    Serial.println("QUESTION 1: SONG PLAYLIST IMPLEMENTATION");
    Serial.println("==========================================\n");
    
    // Test Array-based Playlist
    Serial.println("== Testing Array Playlist ==");
    ArrayList myArrayPlaylist;
    
    // Add songs to array playlist
    addSong(&myArrayPlaylist, {"Song A1", "Artist 1", 180});
    addSong(&myArrayPlaylist, {"Song B2", "Artist 2", 240});
    addSong(&myArrayPlaylist, {"Song C3", "Artist 1", 200});
    
    // Display array playlist
    displayPlaylist(&myArrayPlaylist);
    
    // Remove a song from array playlist
    removeSong(&myArrayPlaylist, 1);
    Serial.println("After removing index 1:");
    displayPlaylist(&myArrayPlaylist);
    
    // Test Linked List-based Playlist
    Serial.println("\n== Testing Linked List Playlist ==");
    LinkedList myLinkedListPlaylist;
    
    // Add songs to linked list playlist
    addSong(&myLinkedListPlaylist, {"Song X1", "Artist X", 195});
    addSong(&myLinkedListPlaylist, {"Song Y2", "Artist Y", 225});
    addSong(&myLinkedListPlaylist, {"Song Z3", "Artist Z", 215});
    
    // Display linked list playlist
    displayPlaylist(&myLinkedListPlaylist);
    
    // Remove a song from linked list playlist
    removeSong(&myLinkedListPlaylist, 1);
    Serial.println("After removing index 1:");
    displayPlaylist(&myLinkedListPlaylist);
    
    // Clean up linked list nodes to avoid memory leak
    SongNode* current = myLinkedListPlaylist.head;
    while (current != nullptr) {
        SongNode* temp = current;
        current = current->next;
        delete temp;
    }
    
    // Print analysis
    Serial.println("\n==================== ANALYSIS ====================");
    Serial.println("Memory Trade-offs:");
    Serial.println("- Array: Fixed size (allocates all memory upfront), no per-element overhead");
    Serial.println("- Linked List: Dynamic size, but each node requires extra memory for pointer");
    Serial.println();
    Serial.println("For a playlist that rarely changes, array is more memory-efficient because:");
    Serial.println("1. No pointer overhead (8 bytes per node on 32-bit ESP32)");
    Serial.println("2. Contiguous memory layout with no fragmentation");
    Serial.println();
    Serial.println("For frequent adds/removes in the middle, linked list is more time-efficient because:");
    Serial.println("1. O(1) insertion/deletion after finding position (just change pointers)");
    Serial.println("2. Array requires O(n) shifting of all subsequent elements");
    Serial.println();
    Serial.println("Risks of linked list on ESP32 over long periods:");
    Serial.println("1. Memory fragmentation - frequent new/delete creates non-contiguous free memory");
    Serial.println("2. Heap exhaustion - small heap (typically 320KB) can become too fragmented");
    Serial.println("3. Slower access times due to non-cache-friendly scattered memory locations");
    Serial.println("==================================================\n");
}

void loop() {
    // Nothing to do here - all work is done in setup()
}