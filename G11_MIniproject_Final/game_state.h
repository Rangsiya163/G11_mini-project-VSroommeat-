#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

/* --- ค่าคงที่กำหนดขนาดฉากและข้อจำกัดของเกม --- */
#define FIELD_W 42   // ความกว้างของพื้นที่พิมพ์ตก (กว้าง 42 ตัวอักษร)
#define FIELD_H 14   // ความสูงของพื้นที่พิมพ์ตก (สูง 14 แถว)
#define MAX_WORDS 5  // จำนวนคำศัพท์สูงสุดที่สามารถตกพร้อมกันในหน้าจอ
#define SKIN_COUNT 5 // จำนวนสกินตัวละครทั้งหมดในเกม

/* -------------------------------------------------------------------------
 * STRUCT : FallingWord
 * ใช้เก็บข้อมูลของ "คำศัพท์ที่กำลังตกลงมา" ใน Phase 1
 * ------------------------------------------------------------------------- */
typedef struct
{
    char text[30];
    int x;
    int y;
    int typed;
    int active;
} FallingWord;

/* -------------------------------------------------------------------------
 * STRUCT : DebatePair
 * ใช้เก็บคู่คำพูดสำหรับการดวลฝีปากใน Phase 2 (Debate)
 * ------------------------------------------------------------------------- */
typedef struct
{
    const char *boss_text;
    const char *answer;
} DebatePair;

/* -------------------------------------------------------------------------
 * STRUCT : GameState
 * โครงสร้างข้อมูลหลักสำหรับจัดการ "สถานะทั้งหมดของเกม"
 * ------------------------------------------------------------------------- */
typedef struct
{
    char name[32]; // ชื่อของผู้เล่น
    int score;     // คะแนนสะสม (ใช้เป็นเงินซื้อของในร้านค้า)

    /* HP Control */
    int boss_hp;       // เลือดปัจจุบันของบอส
    int max_boss_hp;   // เลือดสูงสุดของบอส
    int player_hp;     // เลือดปัจจุบันของผู้เล่น
    int max_player_hp; // เลือดสูงสุดของผู้เล่น

    int difficulty; // ระดับความยาก (1=Easy, 2=Medium, 3=Hard)
    int time_limit; // เวลาทั้งหมดใน Phase 1 (วินาที)
    int fall_speed; // ความเร็วในการตกของตัวอักษร (มิลลิวินาที)

    int selected_skin;             // สกินที่เลือกใช้งานปัจจุบัน (index 0-4)
    int unlocked_skin[SKIN_COUNT]; // อาร์เรย์เช็กสถานะปลดล็อกสกิน (1=ปลดล็อกแล้ว, 0=ยัง)

    int words_completed; // จำนวนคำที่พิมพ์สำเร็จ
    int errors;          // จำนวนครั้งที่พิมพ์ผิด

    int freeze_time;       // เวลาคงเหลือของบัฟแช่แข็งคำตก (วินาที)
    int double_score_time; // เวลาคงเหลือของบัฟคะแนน x2 (วินาที)
    int boss_hurt_timer;   // นับเวลาแสดงท่าเจ็บของบอส (วินาที)
} GameState;

/* --- ฟังก์ชัน Helper สำหรับ Console & UI --- */
void clearScreen(void);
void moveCursor(int x, int y);
void hideCursor(void);
void initConsole(void);
void waitEnter(void);
void printBossHPBar(int current, int max);

/* --- ประกาศตัวแปรคำศัพท์ อาต และข้อความบอส (Global External Arrays) --- */
extern const char *ROOM_ITEMS[];
extern const int ROOM_ITEM_COUNT;

extern const DebatePair DEBATE_LIST[];
extern const int DEBATE_COUNT;

extern const char *BOSS_NORMAL[];
extern const char *BOSS_HURT[];
extern const char *PLAYER_SKINS[SKIN_COUNT][4];

#endif