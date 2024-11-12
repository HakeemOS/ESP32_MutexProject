#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else 
static const BaseType_t app_cpu = 1; 
#endif

//defs
#define led_pin 13

//Global vars
static SemaphoreHandle_t mutex0; 
static uint8_t tFlag = 0; 

// NOTE: the intended way mutex is to work in this project doesnt occur (hypothetically) due to main and taskOne running on different cores and therefore different threads, will try again at some point with 2 tasks running on same core
// (cont.) seems as though main task is auto run/binded onto core 0 and unicore functions only occurs on core 1; 
void taskOne(uint8_t* input){
    char* taskName = pcTaskGetName(NULL);
    if(pdTRUE == xSemaphoreTake(mutex0, 5)){
        ESP_LOGI(taskName, "Take -- PASS!"); 
    } 
    uint8_t lvl = 0;
    uint8_t numIn = *input;
    //xSemaphoreGive(mutex0); 
    if(pdTRUE == xSemaphoreGive(mutex0)){
            ESP_LOGW(taskName, "Give -- PASS!"); 
    }
    char* inPtr = NULL; 
    uint8_t count = 0; 
    ESP_LOGI(taskName, "%d", *input); 
    //uint8_t* numInPtr = &numIn; 
    numIn *= 2; 
    ESP_LOGI(taskName, "running ... \n Input : %d", numIn); 
    for (size_t i = 0; i < numIn; i++){
        lvl = !lvl; 
        gpio_set_level(led_pin, lvl); 
        vTaskDelay(75/portTICK_PERIOD_MS); 
    }
    if(pdTRUE == xSemaphoreGive(mutex0)){
            ESP_LOGW(taskName, "Give -- PASS! (2)"); 
    }
    tFlag = 1;
    while (1){
        ESP_LOGI(taskName, "Doing nothing!"); 
        vTaskDelay(5000/portTICK_PERIOD_MS); 
    }
        
}

void setup(){
    vTaskDelay(1000/portTICK_PERIOD_MS); 
    char* taskName = pcTaskGetName(NULL); 
    ESP_LOGI(taskName, "Mutex Demo Project ..."); 
    printf("\n"); 

    uint8_t inChar;                                                                                      
    //uint8_t count = 0; 
    //char* TxPtr = NULL;
    uint8_t* argPtr = NULL; 
    mutex0 = xSemaphoreCreateMutex(); 
    inChar = 3; 
    ESP_LOGI(taskName, "Blink Rate");                                                                 
    argPtr = &inChar; 
    //argPtr = (uint8_t*)TxPtr; 
    gpio_reset_pin(led_pin);
    gpio_set_direction(led_pin, GPIO_MODE_OUTPUT); 
    ESP_LOGI(taskName, "Number received - %d", *argPtr);
    //xSemaphoreTake(mutex0, portMAX_DELAY);
    xTaskCreatePinnedToCore(taskOne, "Task One", 10000, argPtr, 1, NULL, app_cpu);  
    //xSemaphoreTake(mutex0, portMAX_DELAY); 
    vTaskDelay(1000/portTICK_PERIOD_MS);  
    
}

void app_main(void){
    setup(); 
}