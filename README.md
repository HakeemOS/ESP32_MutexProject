# _ESP32 Series_

## Mutex Project 

Project Designed to use mutex to ensure setup function argument (stack variable) is still in scope while it is passed to Task One. 

**Setup();**
- Creates Mutex and takes before and after task one is created
- Creates Task One and pins to to single core, passing argument to task

**Task One**
- Stores argument in local variable
- Returns Mutex and completes its function using passed value as key parameter (blink repitions) 

