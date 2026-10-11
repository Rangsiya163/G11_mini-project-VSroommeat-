#include "phase2.h"

// วาด UI และประโยคโต้ตอบใน Phase 2
void renderDebateUI(const GameState *game, int round, const DebatePair *debate, const char *input, int remaining)
{
    clearScreen();

    printf("============================================================\n");
    printf(" PHASE 2 : DEBATE BATTLE                        รอบที่ %d / 3\n", round);
    printf("============================================================\n\n");

    const char **boss_art = (game->boss_hurt_timer > 0) ? BOSS_HURT : BOSS_NORMAL;

    printf("                         BOSS\n");
    printf("                         %s\n", boss_art[0]);
    printf("                         %s\n", boss_art[1]);
    printf("                         %s\n\n", boss_art[2]);

    printf(" BOSS HP   : ");
    printBossHPBar(game->boss_hp, game->max_boss_hp);

    printf(" PLAYER HP : ");
    for (int i = 0; i < game->max_player_hp; i++)
        printf(i < game->player_hp ? "♥ " : "♡ ");
    printf(" %d/%d                                เวลา: %2d sec\n\n", game->player_hp, game->max_player_hp, remaining);

    printf("------------------------------------------------------------\n");
    printf(" BOSS:\n");
    printf(" \"%s\"\n\n", debate->boss_text);

    printf(" คำโต้ตอบของคุณ:\n");
    printf(" >> %s\n", debate->answer);

    printf("------------------------------------------------------------\n");
    printf(" พิมพ์คำตอบที่นี่ [กด \\ เพื่อเปิดร้านค้า]:\n");
    printf(" > %s", input);

    fflush(stdout);
}

// การเล่นการดวลโต้ตอบ 1 รอบ
int playOneDebate(GameState *game, int round)
{
    const DebatePair *debate = &DEBATE_LIST[rand() % DEBATE_COUNT];

    char input[128] = "";
    int length = 0;

    int targetLength = (int)strlen(debate->answer);
    int timeLimit = 15;

    DWORD start = GetTickCount();
    DWORD lastSecond = start;

    game->boss_hurt_timer = 0;
    renderDebateUI(game, round, debate, input, timeLimit);

    while (1)
    {
        DWORD now = GetTickCount();

        int elapsed = (int)((now - start) / 1000);
        int remaining = timeLimit - elapsed;

        if (now - lastSecond >= 1000)
        {
            lastSecond = now;
            if (game->boss_hurt_timer > 0)
                game->boss_hurt_timer--;
        }

        if (remaining <= 0)
        {
            printf("\n\n [TIME OUT] หมดเวลา! โดนสวนกลับ! (Player HP -1)\n");
            game->player_hp--;
            Sleep(1500);
            return 0;
        }

        moveCursor(3, 21);
        printf("%-65s", input);

        moveCursor(48, 11);
        printf("เวลา: %2d sec", remaining);

        if (_kbhit())
        {
            int key = _getch();

            if (key == 27)
                return -1;

            if (key == '\\')
            {
                openShop(game);
                start = GetTickCount() - ((DWORD)elapsed * 1000);
                renderDebateUI(game, round, debate, input, remaining);
                continue;
            }

            if (key == '\r')
            {
                if (strcmp(input, debate->answer) == 0)
                {
                    game->boss_hp = 0;
                    game->boss_hurt_timer = 2;
                    renderDebateUI(game, round, debate, input, remaining);

                    printf("\n\n \033[93mCRITICAL COMEBACK! -%d HP\033[0m\n", game->max_boss_hp / 2);
                    printf(" [SUCCESS] โต้ตอบสำเร็จ! บอสเงิบจนพูดไม่ออก!\n\n");

                    Sleep(1800);
                    return 1;
                }
                else
                {
                    printf("\n\n [FAILED] พิมพ์ผิด! การโต้ตอบล้มเหลว! (Player HP -1)\n");
                    game->player_hp--;
                    Sleep(1500);
                    return 0;
                }
            }

            if (key == '\b')
            {
                if (length > 0)
                {
                    length--;
                    input[length] = '\0';
                }
            }
            else if (isprint(key) && length < targetLength)
            {
                input[length] = (char)key;
                length++;
                input[length] = '\0';
            }
        }
        Sleep(20);
    }
}

// ลูปหลักคุมการเล่นใน Phase 2 จนกว่าจะครบ 3 รอบ
int runDebatePhase(GameState *game)
{
    for (int round = 1; round <= 3; round++)
    {
        int result = playOneDebate(game, round);

        if (result == -1)
            return 0;
        if (game->boss_hp <= 0)
            return 1;
        if (game->player_hp <= 0)
            return 0;
    }
    return game->boss_hp <= 0;
}