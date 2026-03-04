#include <Arduino.h>

#define MAX_MENU_DEPTH 10

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
