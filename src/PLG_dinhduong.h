#include <Arduino.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <Wire.h>
#include <TimeLib.h>
#include <RTClib.h>
#include <string.h>
#include <hardwareSerial.h>
#include <PLG_datastring.h>
#include <PLG_debug.hpp>

#define TFT_GREY 0x5AEB    // New colour
TFT_eSPI tft = TFT_eSPI(); // Invoke library

#define TX 17
#define RX 16
uint16_t temp[1], hud[1];
uint16_t mode[1], fr[1], state[1];
long TEMP = 0, HUD = 0;
int T = 0;
int TIMER1 = 0;
#define PH_SENSOR 32
#define EC_SENSOR 33
#define M1 15
#define M2 13
#define M3 12
#define M4 14
#define SET 36
#define UP 39
#define DOWN 34
#define read_set digitalRead(SET)
#define read_up digitalRead(UP)
#define read_dowm digitalRead(DOWN)
int mouse = 1;
int mouse_cai = 1;
float CURRENT_SETTING = 2.00, TEMP_SETTING = 30.00, HUD_SETTING = 50;
float PH = 0, EC = 0;
String STATE_M1 = "OFF", STATE_M2 = "OFF", STATE_M3 = "OFF", STATE_M4 = "OFF";
String ID_control = "0";
String ID_CB = "1";
String ID_DD = "2";
String ID_control_DD = "4";
String ID_master = "3";
String STATUS_M1 = "ENABLE";
String STATUS_M2 = "ENABLE";
String STATUS_M3 = "ENABLE";
String STATUS_M4 = "ENABLE";
int a = 0;
tmElements_t tm;
int value[6] = {0, 0, 0, 0, 0, 0};
unsigned long lastTime = 0;
bool run = false;
bool last_down = 0;
bool last_up = 0;
bool PLG_setup = false;
bool PLG_caibom = false;
bool clear = true;
bool cleared = false;

bool buttonPressed = false;
unsigned long pressStartTime = 0;

void PLG_delete()
{
    if (clear == true)
    {
        tft.fillScreen(TFT_BLACK);
        clear = false; // Đánh dấu đã xóa
    }
}
//-------------------mode cai dat---------------------------
void PLG_destop_set()
{
    yield();

    tft.setTextColor(TFT_BLUE); // set mau chu
    tft.drawCentreString("SET_UP ", 160, 5, 4);

    tft.drawRect(10, 40, 290, 50, TFT_WHITE); // hang 1
    tft.setTextColor(TFT_GREEN);
    tft.drawString("CAI DAT BOM ", 80, 50, 4);

    tft.drawRect(10, 100, 290, 50, TFT_WHITE); // hang 2
    tft.setTextColor(TFT_GREEN);
    tft.drawString("CAI LUU LUONG ", 80, 110, 4);

    tft.drawRect(10, 160, 290, 50, TFT_WHITE); // hang 3
    tft.setTextColor(TFT_GREEN);
    tft.drawString("THOAT ", 80, 170, 4);
}
void PLG_mouse_hang_1()
{
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("---->", 20, 50, 4);
    // tft.fillRect(20, 50, 60, 20, TFT_BLACK);// Xóa mouse hang 1
    tft.fillRect(20, 110, 60, 20, TFT_BLACK); // Xóa mouse hang 2
    tft.fillRect(20, 170, 60, 20, TFT_BLACK); // Xóa mouse hang 3
}
void PLG_mouse_hang_2()
{
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("---->", 20, 110, 4);
    tft.fillRect(20, 50, 60, 20, TFT_BLACK); // Xóa mouse hang 1
    // tft.fillRect(20, 110, 60, 20, TFT_BLACK);// Xóa mouse hang 2
    tft.fillRect(20, 170, 60, 20, TFT_BLACK); // Xóa mouse hang 3
}
void PLG_mouse_hang_3()
{
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("---->", 20, 170, 4);
    tft.fillRect(20, 50, 60, 20, TFT_BLACK);  // Xóa mouse hang 1
    tft.fillRect(20, 110, 60, 20, TFT_BLACK); // Xóa mouse hang 2
    // tft.fillRect(20, 170, 60, 20, TFT_BLACK); // Xóa mouse hang 3
}
void PLG_mode()
{

    if (read_dowm == 1 && last_down == 0)
    {
        mouse--;
        if (mouse <= 0)
            mouse = 3;
    }

    if (read_up == 1 && last_up == 0)
    {
        mouse++;
        if (mouse >= 4)
            mouse = 1;
    }
    last_down = read_dowm;
    last_up = read_up;
    switch (mouse)
    {
    case 1:
        PLG_mouse_hang_1();
        break;
    case 2:
        PLG_mouse_hang_2();
        break;
    case 3:
        PLG_mouse_hang_3();
        break;

    default:
        break;
    }
}
//----------------------------------------------------------

//------------------mode cai dat bom------------------------
void PLG_destop_set_bom()
{
    yield();
    tft.setTextColor(TFT_BLUE); // set mau chu
    tft.drawCentreString("SET_UP ", 160, 5, 4);

    tft.drawRect(10, 40, 145, 50, TFT_WHITE); // hang 1 cot 1
    tft.setTextColor(TFT_GREEN);
    tft.drawString("MOTOR 1", 55, 50, 2);
    tft.setTextColor(TFT_YELLOW);
    tft.drawString(STATUS_M1, 55, 65, 2);

    tft.drawRect(165, 40, 145, 50, TFT_WHITE); // hang 1 cot 2
    tft.setTextColor(TFT_GREEN);
    tft.drawString("MOTOR 2", 210, 50, 2);
    tft.setTextColor(TFT_YELLOW);
    tft.drawString(STATUS_M2, 210, 65, 2);

    tft.drawRect(10, 110, 145, 50, TFT_WHITE); // hang 2 cot 1
    tft.setTextColor(TFT_GREEN);
    tft.drawString("MOTOR 3", 55, 120, 2);
    tft.setTextColor(TFT_YELLOW);
    tft.drawString(STATUS_M3, 55, 135, 2);

    tft.drawRect(165, 110, 145, 50, TFT_WHITE); // hang 2 cot 2
    tft.setTextColor(TFT_GREEN);
    tft.drawString("MOTOR 4", 210, 120, 2);
    tft.setTextColor(TFT_YELLOW);
    tft.drawString(STATUS_M4, 210, 135, 2);

    tft.drawRect(10, 170, 300, 50, TFT_WHITE);
    tft.setTextColor(TFT_GREEN);
    tft.drawCentreString("THOAT ", 160, 180, 4);
}

void PLG_mouse_set_bom_1()
{
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("-->", 15, 60, 2);
    // tft.fillRect(15, 60, 40, 20, TFT_BLACK);// Xóa mouse hang 1 c1
    tft.fillRect(170, 60, 40, 20, TFT_BLACK);  // Xóa mouse hang 1 c2
    tft.fillRect(15, 130, 40, 20, TFT_BLACK);  // Xóa mouse hang 2 c1
    tft.fillRect(170, 130, 40, 20, TFT_BLACK); // Xóa mouse hang 2 c2
    tft.fillRect(15, 190, 40, 20, TFT_BLACK);
}
void PLG_mouse_set_bom_2()
{
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("-->", 170, 60, 2);
    tft.fillRect(15, 60, 40, 20, TFT_BLACK); // Xóa mouse hang 1 c1
    // tft.fillRect(170, 60, 40, 20, TFT_BLACK);  // Xóa mouse hang 1 c2
    tft.fillRect(15, 130, 40, 20, TFT_BLACK);  // Xóa mouse hang 2 c1
    tft.fillRect(170, 130, 40, 20, TFT_BLACK); // Xóa mouse hang 2 c2
    tft.fillRect(15, 190, 40, 20, TFT_BLACK);
}
void PLG_mouse_set_bom_3()
{
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("-->", 15, 130, 2);
    tft.fillRect(15, 60, 40, 20, TFT_BLACK);  // Xóa mouse hang 1 c1
    tft.fillRect(170, 60, 40, 20, TFT_BLACK); // Xóa mouse hang 1 c2
    // tft.fillRect(15, 130, 40, 20, TFT_BLACK);  // Xóa mouse hang 2 c1
    tft.fillRect(170, 130, 40, 20, TFT_BLACK); // Xóa mouse hang 2 c2
    tft.fillRect(15, 190, 40, 20, TFT_BLACK);
}
void PLG_mouse_set_bom_4()
{
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("-->", 170, 130, 2);
    tft.fillRect(15, 60, 40, 20, TFT_BLACK);  // Xóa mouse hang 1 c1
    tft.fillRect(170, 60, 40, 20, TFT_BLACK); // Xóa mouse hang 1 c2
    tft.fillRect(15, 130, 40, 20, TFT_BLACK); // Xóa mouse hang 2 c1
    // tft.fillRect(170, 130, 40, 20, TFT_BLACK); // Xóa mouse hang 2 c2
    tft.fillRect(15, 190, 40, 20, TFT_BLACK);
}
void PLG_mouse_set_bom_thoat()
{
    tft.setTextColor(TFT_YELLOW);
    tft.drawString("-->", 15, 190, 2);
    tft.fillRect(15, 60, 40, 20, TFT_BLACK);   // Xóa mouse hang 1 c1
    tft.fillRect(170, 60, 40, 20, TFT_BLACK);  // Xóa mouse hang 1 c2
    tft.fillRect(15, 130, 40, 20, TFT_BLACK);  // Xóa mouse hang 2 c1
    tft.fillRect(170, 130, 40, 20, TFT_BLACK); // Xóa mouse hang 2 c2
    // tft.fillRect(15, 190, 40, 20, TFT_BLACK);
}
void PLG_cai_bom()
{

    if (read_dowm == 1 && last_down == 0)
    {
        mouse_cai--;
        if (mouse_cai <= 0)
            mouse_cai = 5;
    }

    if (read_up == 1 && last_up == 0)
    {
        mouse_cai++;
        if (mouse_cai >= 6)
            mouse_cai = 1;
    }
    last_down = read_dowm;
    last_up = read_up;
    switch (mouse_cai)
    {
    case 1:
        PLG_mouse_set_bom_1();
        break;
    case 2:
        PLG_mouse_set_bom_2();
        break;
    case 3:
        PLG_mouse_set_bom_3();
        break;
    case 4:
        PLG_mouse_set_bom_4();
        break;
    case 5:
        PLG_mouse_set_bom_thoat();
        break;

    default:
        break;
    }
}
//----------------------------------------------------------

void DRAW_FORM()
{
    yield();
    tft.setTextColor(TFT_GREEN);
    tft.drawCentreString("AUTO-FERTIGATION ", 160, 15, 2);

    tft.drawWideLine(10, 40, 150, 40, 1, TFT_WHITE, TFT_WHITE); // ROW1
    tft.drawWideLine(10, 80, 150, 80, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(10, 40, 10, 80, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(150, 40, 150, 80, 1, TFT_WHITE, TFT_WHITE);
    tft.setTextColor(TFT_GREEN);
    tft.drawCentreString("SENSOR", 80, 50, 2);

    tft.drawWideLine(160, 40, 300, 40, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(160, 80, 300, 80, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(160, 40, 160, 80, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(300, 40, 300, 80, 1, TFT_WHITE, TFT_WHITE);
    tft.setTextColor(TFT_GREEN);
    tft.drawCentreString("MOTOR", 230, 50, 2);
    // delay(20);

    yield();
    tft.drawWideLine(10, 90, 150, 90, 1, TFT_WHITE, TFT_WHITE); // ROW2
    tft.drawWideLine(10, 150, 150, 150, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(10, 90, 10, 150, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(150, 90, 150, 150, 1, TFT_WHITE, TFT_WHITE);
    tft.setTextColor(TFT_GREEN);
    tft.drawRightString("EC", 40, 115, 2);
    tft.fillRect(50, 110, 80, 40, TFT_BLACK);
    // delay(1);
    tft.setTextColor(TFT_YELLOW);
    tft.setTextFont(4);
    tft.drawFloat(EC, 1, 60, 115);
    tft.setTextColor(TFT_WHITE);
    tft.drawRightString("ppm", 150, 115, 2);
    // delay(1);

    tft.drawWideLine(160, 90, 300, 90, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(160, 130, 300, 130, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(160, 90, 160, 130, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(230, 90, 230, 130, 1, TFT_WHITE, TFT_WHITE); // LINE GIUA
    tft.drawWideLine(300, 90, 300, 130, 1, TFT_WHITE, TFT_WHITE);
    tft.setTextColor(TFT_WHITE);
    tft.drawCentreString("M1=", 180, 100, 2);
    tft.fillRect(190, 100, 35, 20, TFT_BLACK);
    // delay(1);
    tft.drawCentreString(STATE_M1, 210, 100, 2);
    tft.setTextColor(TFT_WHITE);
    tft.drawCentreString("M2=", 250, 100, 2);
    tft.fillRect(260, 100, 35, 20, TFT_BLACK);
    // delay(1);
    tft.drawCentreString(STATE_M2, 280, 100, 2);
    tft.setTextColor(TFT_WHITE);
    // delay(1);

    yield();
    tft.drawWideLine(10, 160, 150, 160, 1, TFT_WHITE, TFT_WHITE); // ROW3
    tft.drawWideLine(10, 220, 150, 220, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(10, 160, 10, 220, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(150, 160, 150, 220, 1, TFT_WHITE, TFT_WHITE);
    tft.setTextColor(TFT_GREEN);
    tft.drawRightString("PH", 40, 180, 2);
    tft.fillRect(60, 170, 70, 40, TFT_BLACK);
    // delay(1);
    tft.setTextColor(TFT_YELLOW);
    tft.setTextFont(4);
    tft.drawFloat(PH, 1, 60, 175);
    tft.setTextColor(TFT_WHITE);
    tft.drawRightString("%", 140, 180, 2);
    // delay(1);

    tft.drawWideLine(160, 140, 300, 140, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(160, 180, 300, 180, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(230, 140, 230, 180, 1, TFT_WHITE, TFT_WHITE); // LINE GIUA
    tft.drawWideLine(160, 140, 160, 180, 1, TFT_WHITE, TFT_WHITE);
    tft.drawWideLine(300, 140, 300, 180, 1, TFT_WHITE, TFT_WHITE);
    tft.setTextColor(TFT_WHITE);
    tft.drawCentreString("M3=", 180, 150, 2);
    tft.fillRect(190, 150, 35, 20, TFT_BLACK);
    // delay(1);
    tft.drawCentreString(STATE_M3, 210, 150, 2);
    tft.drawCentreString("M4=", 250, 150, 2);
    tft.fillRect(260, 150, 35, 20, TFT_BLACK);
    // delay(1);
    tft.drawCentreString(STATE_M4, 280, 150, 2);

    // tft.drawWideLine(10, 230, 150, 230, 2, TFT_WHITE, TFT_WHITE);  // ROW4
    // tft.drawWideLine(10, 270, 150, 270, 2, TFT_WHITE, TFT_WHITE);
    // tft.drawWideLine(10, 230, 10, 270, 2, TFT_WHITE, TFT_WHITE);
    // tft.drawWideLine(150, 230, 150, 270, 2, TFT_WHITE, TFT_WHITE);
    // tft.setTextColor(TFT_YELLOW);
    // tft.drawCentreString("HUDMIDUTY:", 80, 240, 2);

    // tft.drawWideLine(160, 230, 230, 230, 2, TFT_WHITE, TFT_WHITE);
    // tft.drawWideLine(160, 270, 230, 270, 2, TFT_WHITE, TFT_WHITE);
    // tft.drawWideLine(160, 230, 160, 270, 2, TFT_WHITE, TFT_WHITE);
    // tft.drawWideLine(230, 230, 230, 270, 2, TFT_WHITE, TFT_WHITE);

    // //tft.drawCentreString("50", 195, 240, 2);

    // tft.drawWideLine(10, 280, 150, 280, 2, TFT_WHITE, TFT_WHITE);  // ROW5
    // tft.drawWideLine(10, 315, 150, 315, 2, TFT_WHITE, TFT_WHITE);
    // tft.drawWideLine(10, 280, 10, 315, 2, TFT_WHITE, TFT_WHITE);
    // tft.drawWideLine(150, 280, 150, 315, 2, TFT_WHITE, TFT_WHITE);

    // tft.drawWideLine(160, 280, 230, 280, 2, TFT_WHITE, TFT_WHITE);
    // tft.drawWideLine(160, 315, 230, 315, 2, TFT_WHITE, TFT_WHITE);
    // tft.drawWideLine(160, 280, 160, 315, 2, TFT_WHITE, TFT_WHITE);
    // tft.drawWideLine(230, 280, 230, 315, 2, TFT_WHITE, TFT_WHITE);
}

void setup()
{
    pinMode(25, INPUT_PULLUP);
    pinMode(26, INPUT);
    pinMode(27, INPUT);
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    pinMode(M3, OUTPUT);
    pinMode(M4, OUTPUT);
    pinMode(SET, INPUT);
    pinMode(UP, INPUT);
    pinMode(DOWN, INPUT);
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RX, TX);
    Serial.println(" Config device");

    mode[0] = 4;
    fr[0] = 2500;

    Wire.begin(21, 22);

    bool parse = false;
    bool config = false;

    // get the date and time the compiler was run
    //     if (getDate(__DATE__) && getTime(__TIME__)) {
    //     parse = true;
    //     // and configure the RTC with this info
    //     if (RTC.write(tm)) {
    //       config = true;
    //     }
    //  }

    // delay(200);
    if (parse && config)
    {
        Serial.print("DS1307 configured Time=");
        Serial.print(__TIME__);
        Serial.print(", Date=");
        Serial.println(__DATE__);
    }
    else if (parse)
    {
        Serial.println("DS1307 Communication Error :-{");
        Serial.println("Please check your circuitry");
    }
    else
    {
        Serial.print("Could not parse info from the compiler, Time=\"");
        Serial.print(__TIME__);
        Serial.print("\", Date=\"");
        Serial.print(__DATE__);
        Serial.println("\"");
    }
    Serial.println("DS1307RTC Read Test");
    Serial.println("-------------------");

    // TFT INIT
    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    // DRAW_FORM();
    // delay(300);
}
void RS485()
{
    // modbus.readInputRegisters(20, 1, temp, 1);
    // modbus.readInputRegisters(20, 0, hud, 1);
    // modbus.readInputRegisters(10, 0, state, 1);
    TEMP = temp[0] / 10;
    HUD = hud[0] / 10;
    Serial.print("    Temp:  ");
    Serial.print(TEMP);
    Serial.print(" Độ C");
    Serial.print("    Hud:  ");
    Serial.print(HUD);
    Serial.print(" %");
    Serial.print("    State:  ");
    Serial.println(state[0]);
}

void READ_SENSOR()
{
    // PH = analogRead(PH_SENSOR);
    // EC = analogRead(EC_SENSOR);
    PH = 50.2;
    EC = 3.3;
    PLG_write_4(ID_control_DD, ID_DD, "EC", String(EC, 2));
    SEN_PRINTLN(messages4);
    DEBUG_PRINTLN(messages4);
    delay(20);
    PLG_write_4(ID_control_DD, ID_DD, "PH", String(PH, 2));
    SEN_PRINTLN(messages4);
    DEBUG_PRINTLN(messages4);
}
void RUN_M1()
{
    digitalWrite(M1, HIGH);
    STATE_M1 = "ON";
}
void STOP_M1()
{
    digitalWrite(M1, LOW);
    STATE_M1 = "OFF";
}
void RUN_M2()
{
    digitalWrite(M2, HIGH);
    STATE_M2 = "ON";
}
void STOP_M2()
{
    digitalWrite(M2, LOW);
    STATE_M2 = "OFF";
}
void RUN_M3()
{
    digitalWrite(M3, HIGH);
    STATE_M3 = "ON";
}
void STOP_M3()
{
    digitalWrite(M3, LOW);
    STATE_M3 = "OFF";
}
void STOP_M4()
{
    digitalWrite(M4, LOW);
    STATE_M4 = "OFF";
}
void RUN_M4()
{
    digitalWrite(M4, HIGH);
    STATE_M4 = "ON";
}
void loop()
{

    bool buttonState = digitalRead(SET) == HIGH;
    if (buttonState && !buttonPressed)
    {
        // Bắt đầu nhấn
        buttonPressed = true;
        pressStartTime = millis();
    }

    if (!buttonState && buttonPressed)
    {
        // Nhả nút
        buttonPressed = false;
        run =! run;
    }

    // Nếu nút vẫn đang nhấn và đã giữ đủ 5s
    if (buttonPressed && (millis() - pressStartTime >= 5000))
    {
        PLG_setup = true;
        tft.fillScreen(TFT_BLACK);
        // Reset thời gian bắt đầu để tiếp tục đếm nếu giữ tiếp
        pressStartTime = millis();
    }
    if (PLG_setup == true)
    {
        while (read_set == 1)
            ;
        if (PLG_caibom == false)
        {
            PLG_delete();
            PLG_destop_set();
            PLG_mode();
        }
        if (PLG_caibom == true)
        {
            PLG_delete();
            PLG_destop_set_bom();
            PLG_cai_bom();
        }
        if (read_set == 1)
        {
            while (read_set == 1)
                ;

            if (mouse == 1 && PLG_setup == true && PLG_caibom == false)
            {
                PLG_caibom = true;
                clear = true;
            }
            else if (mouse == 2 && PLG_setup == true && PLG_caibom == false)
            {
                /* code */
                clear = true;
            }
            else if (mouse == 3 && PLG_setup == true && PLG_caibom == false)
            {
                PLG_setup = false;
                clear = true;
                run =! run;
            }
            else if (mouse_cai == 1 && PLG_caibom == true)
            {
                clear = true;
                if (STATUS_M1.startsWith("ENABLE"))
                {
                    STATUS_M1 = "DISABLE";
                }
                else
                {
                    STATUS_M1 = "ENABLE";
                }
            }
            else if (mouse_cai == 2 && PLG_caibom == true)
            {
                clear = true;
                if (STATUS_M2.startsWith("ENABLE"))
                {
                    STATUS_M2 = "DISABLE";
                }
                else
                {
                    STATUS_M2 = "ENABLE";
                }
            }
            else if (mouse_cai == 3 && PLG_caibom == true)
            {
                clear = true;
                if (STATUS_M3.startsWith("ENABLE"))
                {
                    STATUS_M3 = "DISABLE";
                }
                else
                {
                    STATUS_M3 = "ENABLE";
                }
            }
            else if (mouse_cai == 4 && PLG_caibom == true)
            {
                clear = true;
                if (STATUS_M4.startsWith("ENABLE"))
                {
                    STATUS_M4 = "DISABLE";
                }
                else
                {
                    STATUS_M4 = "ENABLE";
                }
            }
            else if (mouse_cai == 5 && PLG_caibom == true)
            {
                clear = true;
                PLG_caibom = false;
            }
        }
    }
    else if (PLG_setup == false)
    {
        PLG_delete();
        DRAW_FORM();
        if (run == 1)
        {
            if (STATUS_M1.startsWith("ENABLE"))
            {
                RUN_M1();
            }
             if (STATUS_M2.startsWith("ENABLE"))
            {
                RUN_M2();
            }
             if (STATUS_M3.startsWith("ENABLE"))
            {
                RUN_M3();
            }
             if (STATUS_M4.startsWith("ENABLE"))
            {
                RUN_M4();
            }
            if (STATUS_M1.startsWith("DISABLE"))
            {
                STOP_M1();
            }
             if (STATUS_M2.startsWith("DISABLE"))
            {
                STOP_M2();
            }
             if (STATUS_M3.startsWith("DISABLE"))
            {
                STOP_M3();
            }
             if (STATUS_M4.startsWith("DISABLE"))
            {
                STOP_M4();
            }

        }
        else
        {
            if (STATUS_M1.startsWith("ENABLE"))
            {
                STOP_M1();
            }
             if (STATUS_M2.startsWith("ENABLE"))
            {
                STOP_M2();
            }
             if (STATUS_M3.startsWith("ENABLE"))
            {
                STOP_M3();
            }
             if (STATUS_M4.startsWith("ENABLE"))
            {
                STOP_M4();
            }
            if (STATUS_M1.startsWith("DISABLE"))
            {
                STOP_M1();
            }
             if (STATUS_M2.startsWith("DISABLE"))
            {
                STOP_M2();
            }
             if (STATUS_M3.startsWith("DISABLE"))
            {
                STOP_M3();
            }
             if (STATUS_M4.startsWith("DISABLE"))
            {
                STOP_M4();
            }
        }
    }
}
