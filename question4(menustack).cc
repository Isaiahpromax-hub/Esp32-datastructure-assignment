#include <Arduino.h>

#define MAX_MENU_DEPTH 10

// Menu IDs
#define MENU_MAIN 0
#define MENU_SETTINGS 1#include <Arduino.h>

#define MAX_MENU_DEPTH 10

/**
 * ANALYSIS: Menu Navigation System
 * ================================
 * This implements a stack-based navigation system for menu-driven interfaces.
 * The stack tracks navigation history to enable "Back" functionality.
 * 
 * KEY CONCEPTS:
 * - Stack: LIFO (Last In, First Out) data structure
 * - Maximum depth: 10 levels (prevents excessive nesting)
 * - Error handling: Overflow/Underflow detection
 */

// Menu IDs
#define MENU_MAIN 0
#define MENU_SETTINGS 1
#define MENU_DISPLAY_SETTINGS 2
#define MENU_AUDIO_SETTINGS 3
#define MENU_BRIGHTNESS 4
#define MENU_CONTRAST 5

// TODO: Implement an array-based stack for integers (menu IDs)
class MenuStack {
private:
  int stack[MAX_MENU_DEPTH];
  int top; // Index of the top element, -1 for empty

public:
  MenuStack() : top(-1) {}

  // TODO: Push a menu ID onto the stack. Check for stack overflow.
  void push(int menuId) {
    /**
     * ANALYSIS: Push Operation
     * ========================
     * Time Complexity: O(1)
     * 
     * ERROR HANDLING:
     * - Checks for stack overflow (top >= MAX_MENU_DEPTH - 1)
     * - Prevents navigation beyond maximum depth
     * - Prints error message for debugging
     * 
     * When overflow occurs:
     * - Function returns without pushing
     * - User cannot navigate deeper
     * - Previous menu history preserved
     */
    if (top >= MAX_MENU_DEPTH - 1) {
      Serial.println("Stack Overflow! Cannot go deeper.");
      return;
    }
    top++;
    stack[top] = menuId;
  }

  // TODO: Pop and return the top menu ID. Check for underflow. Return -1 if empty.
  int pop() {
    /**
     * ANALYSIS: Pop Operation
     * =======================
     * Time Complexity: O(1)
     * 
     * ERROR HANDLING:
     * - Checks for stack underflow (isEmpty)
     * - Returns -1 as error indicator when empty
     * - Prints error message for debugging
     * 
     * Return values:
     * - Valid menu ID (0-5): Success
     * - -1: Error (stack empty)
     */
    if (isEmpty()) {
      Serial.println("Stack Underflow!");
      return -1;
    }
    int value = stack[top];
    top--;
    return value;
  }

  // TODO: Return the top menu ID without removing it. Return -1 if empty.
  int peek() {
    /**
     * ANALYSIS: Peek Operation
     * ========================
     * Time Complexity: O(1)
     * 
     * Unlike pop(), this doesn't modify the stack
     * Useful for previewing previous menu without navigation
     * Returns -1 if stack is empty
     */
    if (isEmpty()) {
      return -1;
    }
    return stack[top];
  }

  bool isEmpty() {
    return top == -1;  // Stack empty when top is -1
  }
};

MenuStack navStack;
int currentMenu = MENU_MAIN;

void displayMenu(int menuId) {
  Serial.println("\n--- Menu ---");

  switch (menuId) {
    case MENU_MAIN:
      Serial.println("1. Main Menu");
      Serial.println(" Enter 's' for Settings");
      break;

    case MENU_SETTINGS:
      Serial.println("2. Settings Menu");
      Serial.println(" Enter 'd' for Display Settings");
      Serial.println(" Enter 'a' for Audio Settings");
      Serial.println(" Enter 'b' to go Back");
      break;

    case MENU_DISPLAY_SETTINGS:
      Serial.println("3. Display Settings Menu");
      Serial.println(" Enter 'r' for Brightness");
      Serial.println(" Enter 'c' for Contrast");
      Serial.println(" Enter 'b' to go Back");
      break;

    case MENU_AUDIO_SETTINGS:
      Serial.println("4. Audio Settings Menu");
      Serial.println(" (No sub-menus)");
      Serial.println(" Enter 'b' to go Back");
      break;

    case MENU_BRIGHTNESS:
      Serial.println("5. Brightness Menu");
      Serial.println(" Enter 'b' to go Back");
      break;

    case MENU_CONTRAST:
      Serial.println("6. Contrast Menu");
      Serial.println(" Enter 'b' to go Back");
      break;

    default:
      Serial.println("Unknown Menu");
      break;
  }

  Serial.print("> ");
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  displayMenu(currentMenu);
  
  /**
   * ANALYSIS: Initial State
   * ======================
   * - Stack is empty (top = -1)
   * - Current menu: MAIN (ID: 0)
   * - User starts at root of menu tree
   */
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'b') { // BACK command

      // TODO: Implement back navigation using the stack.
      // If the stack is not empty, pop the previous menu and set it as currentMenu.
      // If the stack is empty, you are at the main menu; print "Already at main menu."

      /**
       * ANALYSIS: Back Navigation
       * =========================
       * This implements the "Back" button functionality:
       * 
       * 1. Check if stack has previous menu (not empty)
       * 2. If yes: Pop previous menu and navigate to it
       * 3. If no: Already at main menu, can't go back further
       * 
       * Edge Cases Handled:
       * - Stack underflow prevention
       * - User feedback for invalid back operation
       */
      if (!navStack.isEmpty()) {
        currentMenu = navStack.pop();
      } else {
        Serial.println("Already at main menu.");
      }

    } else { // Assume any other key is a navigation command

      // TODO: Before changing to a new menu, push the *current* menu onto the stack.
      // Then, change currentMenu based on the command.

      int newMenu = -1;

      /**
       * ANALYSIS: Menu Navigation Logic
       * ===============================
       * This creates a tree structure of menus:
       * 
       * MAIN (0)
       *   └── SETTINGS (1)
       *        ├── DISPLAY (2)
       *        │    ├── BRIGHTNESS (4)
       *        │    └── CONTRAST (5)
       *        └── AUDIO (3)
       * 
       * Each valid command:
       * 1. Pushes current menu to stack (for back navigation)
       * 2. Updates currentMenu to new destination
       * 3. Stack now contains the path taken
       */
      if (currentMenu == MENU_MAIN && command == 's') {
        newMenu = MENU_SETTINGS;
      }
      else if (currentMenu == MENU_SETTINGS && command == 'd') {
        newMenu = MENU_DISPLAY_SETTINGS;
      }
      else if (currentMenu == MENU_SETTINGS && command == 'a') {
        newMenu = MENU_AUDIO_SETTINGS;
      }
      else if (currentMenu == MENU_DISPLAY_SETTINGS && command == 'r') {
        newMenu = MENU_BRIGHTNESS;
      }
      else if (currentMenu == MENU_DISPLAY_SETTINGS && command == 'c') {
        newMenu = MENU_CONTRAST;
      }

      if (newMenu != -1) {
        /**
         * ANALYSIS: Successful Navigation
         * ===============================
         * 1. Push current menu to stack (save history)
         * 2. Update currentMenu to new destination
         * 3. Stack now contains: [MAIN, SETTINGS, ...]
         * 
         * Example:
         * MAIN → 's' → SETTINGS
         * Stack: [MAIN]
         * 
         * SETTINGS → 'd' → DISPLAY
         * Stack: [MAIN, SETTINGS]
         */
        navStack.push(currentMenu);
        currentMenu = newMenu;
      }
      
      /**
       * ANALYSIS: Invalid Commands
       * ==========================
       * If newMenu remains -1:
       * - Command not recognized in current menu
       * - No navigation occurs
       * - Stack unchanged
       * - Current menu stays same
       */
    }

    displayMenu(currentMenu);
  }
}

/**
 * OVERALL SYSTEM ANALYSIS
 * =======================
 * 
 * 1. DATA STRUCTURE: Stack
 *    - LIFO perfect for "Back" functionality
 *    - Fixed size array (no dynamic allocation)
 *    - O(1) operations for all methods
 * 
 * 2. ERROR HANDLING:
 *    - Stack Overflow: Prevents excessive nesting
 *    - Stack Underflow: Handles back from main menu
 *    - Invalid commands: Ignored silently
 * 
 * 3. MEMORY USAGE:
 *    - Stack array: 10 integers × 4 bytes = 40 bytes
 *    - Very efficient for embedded systems
 * 
 * 4. USER EXPERIENCE:
 *    - Clear menu display
 *    - Visual feedback for navigation
 *    - Error messages for invalid operations
 * 
 * 5. LIMITATIONS:
 *    - Maximum 10 levels deep
 *    - No circular navigation
 *    - Single-character commands only
 */
#define MENU_DISPLAY_SETTINGS 2
#define MENU_AUDIO_SETTINGS 3
#define MENU_BRIGHTNESS 4
#define MENU_CONTRAST 5

// TODO: Implement an array-based stack for integers (menu IDs)
class MenuStack {
private:
  int stack[MAX_MENU_DEPTH];
  int top; // Index of the top element, -1 for empty

public:
  MenuStack() : top(-1) {}

  // TODO: Push a menu ID onto the stack. Check for stack overflow.
  void push(int menuId) {
    if (top >= MAX_MENU_DEPTH - 1) {
      Serial.println("Stack Overflow! Cannot go deeper.");
      return;
    }
    top++;
    stack[top] = menuId;
  }

  // TODO: Pop and return the top menu ID. Check for underflow. Return -1 if empty.
  int pop() {
    if (isEmpty()) {
      Serial.println("Stack Underflow!");
      return -1;
    }
    int value = stack[top];
    top--;
    return value;
  }

  // TODO: Return the top menu ID without removing it. Return -1 if empty.
  int peek() {
    if (isEmpty()) {
      return -1;
    }
    return stack[top];
  }

  bool isEmpty() {
    return top == -1;
  }
};

MenuStack navStack;
int currentMenu = MENU_MAIN;

void displayMenu(int menuId) {
  Serial.println("\n--- Menu ---");

  switch (menuId) {
    case MENU_MAIN:
      Serial.println("1. Main Menu");
      Serial.println(" Enter 's' for Settings");
      break;

    case MENU_SETTINGS:
      Serial.println("2. Settings Menu");
      Serial.println(" Enter 'd' for Display Settings");
      Serial.println(" Enter 'a' for Audio Settings");
      Serial.println(" Enter 'b' to go Back");
      break;

    case MENU_DISPLAY_SETTINGS:
      Serial.println("3. Display Settings Menu");
      Serial.println(" Enter 'r' for Brightness");
      Serial.println(" Enter 'c' for Contrast");
      Serial.println(" Enter 'b' to go Back");
      break;

    case MENU_AUDIO_SETTINGS:
      Serial.println("4. Audio Settings Menu");
      Serial.println(" (No sub-menus)");
      Serial.println(" Enter 'b' to go Back");
      break;

    case MENU_BRIGHTNESS:
      Serial.println("5. Brightness Menu");
      Serial.println(" Enter 'b' to go Back");
      break;

    case MENU_CONTRAST:
      Serial.println("6. Contrast Menu");
      Serial.println(" Enter 'b' to go Back");
      break;

    default:
      Serial.println("Unknown Menu");
      break;
  }

  Serial.print("> ");
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  displayMenu(currentMenu);
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'b') { // BACK command

      // TODO: Implement back navigation using the stack.
      // If the stack is not empty, pop the previous menu and set it as currentMenu.
      // If the stack is empty, you are at the main menu; print "Already at main menu."

      if (!navStack.isEmpty()) {
        currentMenu = navStack.pop();
      } else {
        Serial.println("Already at main menu.");
      }

    } else { // Assume any other key is a navigation command

      // TODO: Before changing to a new menu, push the *current* menu onto the stack.
      // Then, change currentMenu based on the command.

      int newMenu = -1;

      if (currentMenu == MENU_MAIN && command == 's') {
        newMenu = MENU_SETTINGS;
      }
      else if (currentMenu == MENU_SETTINGS && command == 'd') {
        newMenu = MENU_DISPLAY_SETTINGS;
      }
      else if (currentMenu == MENU_SETTINGS && command == 'a') {
        newMenu = MENU_AUDIO_SETTINGS;
      }
      else if (currentMenu == MENU_DISPLAY_SETTINGS && command == 'r') {
        newMenu = MENU_BRIGHTNESS;
      }
      else if (currentMenu == MENU_DISPLAY_SETTINGS && command == 'c') {
        newMenu = MENU_CONTRAST;
      }

      if (newMenu != -1) {
        navStack.push(currentMenu);
        currentMenu = newMenu;
      }
    }

    displayMenu(currentMenu);
  }
}
