/*********
 * PLG Project LoRa DD
 *
 * This code is for sending LoRa packets using an ESP32 and a LoRa transceiver module.
 * It initializes the LoRa module, sets the frequency, and sends packets with a counter value.
 * The packets alternate between "even" and "odd" based on the counter value.
 * * The code also includes a commented-out section for receiving packets, which can be enabled if needed.
 *
 * This code is part of the PLG Project for the PLG Contest 2025.
 */
#define DEBUG 
#define SEN
// #define PLG_LORA_DD

#include <Arduino.h>

/**************************************************************
 *                
 ***************************************************************/
#if defined(PLG_LORA_DD)
#include <PLG_lora_DD.h>
#else
#include <PLG_dinhduong.h>
#endif

