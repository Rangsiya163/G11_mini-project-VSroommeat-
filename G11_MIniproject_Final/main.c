/* G11_VSroommate
  692110158 นางสาวแพวรา เรียบประดิษฐ์
  692110163 นางสาวรังสิญา แก้วประพาฬ
  692110174 นางสาวสุพิชญา สิทธิ */

#include "game_state.h"
#include "shop.h"
#include "phase1.h"
#include "phase2.h"

/* ===========
   ข้อมูลต่างๆ
   ========== */

const char *ROOM_ITEMS[] = {
    "a", "b", "c", "d", "e", "f", "g",
    "toothbrush", "towel", "shampoo", "skincare",
    "laptop", "blanket", "pillow", "charger",
    "noodle", "hanger", "snack", "money",
    "mirror", "comb", "notebook", "pencil",
    "desk", "chair", "shoes", "socks"};

const int ROOM_ITEM_COUNT = (int)(sizeof(ROOM_ITEMS) / sizeof(ROOM_ITEMS[0]));

const DebatePair DEBATE_LIST[] = {
    {"Why so mad? I just borrowed your skincare!", "Ask me before using my stuff!"},
    {"I am your senior, respect me!", "Respect goes both ways!"},
    {"Don't forget to pay the electric bill!", "You used the AC all day long!"},
    {"Why is the room so dirty again?!", "It was clean until you arrived!"}};

const int DEBATE_COUNT = (int)(sizeof(DEBATE_LIST) / sizeof(DEBATE_LIST[0]));

const char *BOSS_NORMAL[] = {
    "  (◣_◢)",
    "  /|█|\\",
    "   / \\"};

const char *BOSS_HURT[] = {
    " (>(益)<)",
    "  /(█)\\",
    "   / \\"};

const char *PLAYER_SKINS[SKIN_COUNT][4] = {
    {"/\\_/\\",
     "(≧ω≦)",
     "( > )>♡", "Meow Heart"},

    {" 𐔌՞ ܸ.ˬ.ܸ՞𐦯",
     "  ( > )>",
     "   vv", "Fluffy"},

    {"  (˵ •̀ ᴗ - ˵ ) ✧",
     "  ദ്ദി𐔌    𐦯",
     "    v v", "Winky"},

    {"  ٩(ˊᗜˋ*)و",
     "    (  )♡",
     "    vv", "Power"},

    {"  ^ (˵◝ ⩊ ◜˵マ",
     "    <( . ✧< )",
     "        V V", "Glitter"}};

/* =========================================================
   ฟังก์ชันช่วยจัดการส่วน Console ภายในเกม
   ========================================================= */

void clearScreen(void) { system("cls"); } // ไว้ล้างข้อความทั้งหมดบนหน้าจอเป็นหน้าเปล่าๆ

void moveCursor(int x, int y)
{
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void hideCursor(void) // ปิดการแสดงผลเคอร์เซอร์พิมพ์งาน ความสวยงาม
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(console, &info);
    info.bVisible = FALSE;
    SetConsoleCursorInfo(console, &info);
}

void initConsole(void) // ใช่นำเข้าภาษาไทย สัญลักษณ์พิเศษ และไอคอน ASCII Art ได้ถูกต้อง
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    hideCursor();
}

void waitEnter(void)
{
    printf("\nกด [ENTER] เพื่อดำเนินรายการต่อ...");
    getchar();
}

void printBossHPBar(int current, int max)
{
    int total_blocks = 20;
    int filled = (int)((double)current / max * total_blocks);
    if (filled < 0)
        filled = 0;
    if (filled > total_blocks)
        filled = total_blocks;

    printf("[");
    for (int i = 0; i < total_blocks; i++)
    {
        if (i < filled)
            printf("█");
        else
            printf("░");
    }
    printf("] %d / %d\n", current, max);
}

/* =========================================================
   GAME FLOW & MENUS
   ========================================================= */

void showHowToPlay(void)
{
    clearScreen();
    printf("============================================================\n");
    printf("                    HOW TO PLAY                            \n");
    printf("============================================================\n\n");

    printf(" PHASE 1 : FALLING WORDS เก็บคะแนนและลด Boss HP\n");
    printf(" - พิมพ์คำศัพท์ที่กำลังตกมาถึงพื้นดินเพื่อทำดาเมจใส่บอสรูทเมท Boss HP\n");
    printf(" - มีสัญลักษณ์ [>] คอยบอกเป้าหมายคำที่อยู่ต่ำที่สุดเพื่อช่วยนำสายตาได้ง่าย\n");
    printf(" - เมื่อรูทเมล Boss HP ลงถึง 50%% บอสจะโมโหตัดเข้าสู่ Phase 2 ทันที\n");
    printf(" - กดปุ่ม [ \\ ] เพื่อเปิดร้านค้าซื้อของช่วยเหลือได้ตลอดเวลา\n\n");

    printf(" PHASE 2 : DEBATE BATTLE ประชันฝีปาก\n");
    printf(" - รูมเมทจะยั่วโมโหด้วยคำพูด\n");
    printf(" - พิมพ์ข้อความโต้ตอบที่กำหนดให้ถูกต้องแล้วกด ENTER\n");
    printf(" - หากโต้ตอบสำเร็จ บอสจะพ่ายแพ้ทันที!\n\n");

    printf(" HP SYSTEM\n");
    printf(" - เลือดบอสรูทเมท (Boss HP) ลดจากการพิมพ์คำศัพท์ใน Phase 1\n");
    printf(" - เลือดผู้เล่น (Player HP) จะลดเมื่อพิมพ์ผิดหรือหมดเวลาใน Phase 2\n\n");

    printf(" เป้าหมายสูงสุด\n");
    printf(" - ปราบรูมเมทด้วยการโต้ตอบชนะใน Phase 2 เพื่อยึดห้องคืน!\n");

    waitEnter();
}

void selectDifficulty(GameState *game)
{
    clearScreen();
    printf("=====================================================\n");
    printf("                 SELECT DIFFICULTY                   \n");
    printf("=====================================================\n\n");
    printf("  1. EASY   - Boss HP: 50  | Time: 60s  | Slow Fall\n");
    printf("  2. MEDIUM - Boss HP: 80  | Time: 90s  | Normal Fall\n");
    printf("  3. HARD   - Boss HP: 100 | Time: 120s | Fast Fall\n\n");
    printf("เลือกความยาก (1-3): ");

    int diff = 2;
    if (scanf("%d", &diff) != 1)
        diff = 2;
    while (getchar() != '\n')
        ;

    game->difficulty = diff;
    switch (diff)
    {
    case 1:
        game->max_boss_hp = 50;
        game->boss_hp = 50;
        game->time_limit = 60;
        game->fall_speed = 1200;
        break;
    case 3:
        game->max_boss_hp = 100;
        game->boss_hp = 100;
        game->time_limit = 120;
        game->fall_speed = 600;
        break;
    case 2:
    default:
        game->max_boss_hp = 80;
        game->boss_hp = 80;
        game->time_limit = 90;
        game->fall_speed = 900;
        break;
    }
}

void storyIntro(GameState *game)
{
    clearScreen();
    printf("============================================================\n");
    printf("                      VSroommate                           \n");
    printf("============================================================\n\n");

    printf("สวัสดี %s!\n\n", game->name);
    printf("ในที่สุดก็สอบติดมหาลัย! ชีวิตเด็กปี 1 จะต้องสดใสแน่นอน...\n\n");
    Sleep(1200);

    printf("แต่พอเดินเข้าห้องหอพักมา... นี่คือรูมเมทของฉัน!\n\n");
    printf("รูมเมท: \"เฮ้ย! เด็กใหม่! สกินแคร์นายขอยืมหน่อยนะ...\"\n");
    printf("รูมเมท: \"อ้อ ค่าไฟเดือนนี้จ่าย 70%% ด้วยละกัน!\"\n\n");
    printf("%s: \"ยืมของไม่เคยขอ! ตื่นสาย! กินไม่ทิ้ง! ...ฉันไม่ทนอีกต่อไป!\"\n\n", game->name);

    waitEnter();

    chooseSkin(game);
    selectDifficulty(game);
}

void showEndScreen(GameState *game, int win)
{
    clearScreen();
    printf("============================================================\n");

    if (win)
    {
        printf("                  ★ VICTORY! ชนะแล้ว! ★                    \n");
        printf("============================================================\n\n");
        printf("                         BOSS\n");
        printf("                         %s\n", BOSS_HURT[0]);
        printf("                         %s\n", BOSS_HURT[1]);
        printf("                         %s\n\n", BOSS_HURT[2]);
        printf(" %s: \"สั่งสอนเรียบร้อย! จากนี้ไปห้องนี้ต้องมีระเบียบ!\"\n\n", game->name);
    }
    else
    {
        printf("                  GAME OVER... ยอมแพ้...                   \n");
        printf("============================================================\n\n");
        printf("                         BOSS\n");
        printf("                         %s\n", BOSS_NORMAL[0]);
        printf("                         %s\n", BOSS_NORMAL[1]);
        printf("                         %s\n\n", BOSS_NORMAL[2]);
        printf(" %s: \"ยอมแพ้แล้ว... โดนรูมเมทยึดห้องและสกินแคร์ต่อไป...\"\n\n", game->name);
    }

    printf("------------------------------------------------------------\n");
    printf(" ผู้เล่น         : %s\n", game->name);
    printf(" คะแนนสะสม      : %d pts\n", game->score);
    printf(" คำที่พิมพ์สำเร็จ  : %d\n", game->words_completed);
    printf(" พิมพ์ผิด        : %d\n", game->errors);
    printf(" BOSS HP       : %d / %d\n", game->boss_hp, game->max_boss_hp);
    printf(" PLAYER HP     : %d / %d\n", game->player_hp, game->max_player_hp);
    printf("------------------------------------------------------------\n");

    waitEnter();
}

void playGame(GameState *game)
{
    storyIntro(game);

    game->max_player_hp = 3;
    game->player_hp = game->max_player_hp;
    game->errors = 0;
    game->words_completed = 0;

    clearScreen();
    printf("============================================================\n");
    printf("                    BATTLE START! เริ่มเกม!                  \n");
    printf("============================================================\n\n");

    printf(" เฟส 1: พิมพ์คำศัพท์ตกจากฟ้าเพื่อลด Boss HP ให้เหลือ 50%%\n");
    printf(" เฟส 2: ดวลโต้ตอบคำพูดฉากไคลแม็กซ์เพื่อปราบรูมเมท!\n\n");

    printf(" BOSS HP   : %d\n", game->boss_hp);
    printf(" PLAYER HP : %d\n\n", game->player_hp);

    waitEnter();

    int transitionToPhase2 = runTypingPhase(game);

    if (transitionToPhase2 == 1 && game->player_hp > 0)
    {
        int win = runDebatePhase(game);

        if (win)
            showEndScreen(game, 1);
        else
            showEndScreen(game, 0);
    }
    else
    {
        showEndScreen(game, 0);
    }
}

int main(void)
{
    GameState game = {0};

    srand((unsigned int)time(NULL));
    initConsole();

    // game.score = 50000; //สำหรับให้เงินตัวเองเทสเกม
    game.unlocked_skin[0] = 1;
    game.unlocked_skin[1] = 1;
    game.unlocked_skin[2] = 1;

    clearScreen();
    printf("============================================================\n");
    printf("                    VSroommate                              \n");
    printf("============================================================\n\n");

    printf("กรุณาระบุชื่อของคุณ: ");
    if (scanf("%31s", game.name) != 1)
        strcpy(game.name, "Freshy");

    while (getchar() != '\n')
        ;

    while (1)
    {
        int choice;
        clearScreen();

        printf("============================================================\n");
        printf("                    VSroommate                        \n");
        printf("============================================================\n");
        printf(" ผู้เล่น : %-20s\n", game.name);
        printf(" คะแนน  : %d pts\n", game.score);
        printf("------------------------------------------------------------\n");
        printf(" [1] เริ่มการต่อสู้ (Start Battle)\n");
        printf(" [2] ร้านค้าไอเทม / สกิน (Shop)\n");
        printf(" [3] วิธีการเล่น (How to Play)\n");
        printf(" [4] ออกจากเกม (Exit)\n");
        printf("------------------------------------------------------------\n");
        printf(" เลือกรายการ: ");

        if (scanf("%d", &choice) != 1)
            choice = 0;
        while (getchar() != '\n')
            ;

        if (choice == 1)
        {
            playGame(&game);
        }
        else if (choice == 2)
        {
            openShop(&game);
        }
        else if (choice == 3)
        {
            showHowToPlay();
        }
        else if (choice == 4)
        {
            break;
        }
    }

    clearScreen();
    printf("\nขอบคุณที่เล่นเกม!\n\n");
    return 0;
}