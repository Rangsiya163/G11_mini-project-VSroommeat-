#include "phase1.h"

// ฟังก์ชันสุ่มสร้างคำศัพท์ใหม่ในช่องว่างที่ inactive อยู่
void spawnWord(FallingWord words[])
{
    for (int i = 0; i < MAX_WORDS; i++)
    {
        if (!words[i].active)
        {
            const char *word = ROOM_ITEMS[rand() % ROOM_ITEM_COUNT];
            int len = (int)strlen(word);

            strcpy(words[i].text, word);
            words[i].x = rand() % (FIELD_W - len - 2) + 1;
            words[i].y = 0;
            words[i].typed = 0;
            words[i].active = 1;
            return;
        }
    }
}

// ฟังก์ชันค้นหาคำศัพท์ที่อยู่ต่ำที่สุดเพื่อใช้เป็นเป้าหมายหลักในการพิมพ์
int findTargetWord(FallingWord words[])
{
    int target = -1;
    int lowest = -1;

    for (int i = 0; i < MAX_WORDS; i++)
    {
        if (words[i].active && words[i].y > lowest)
        {
            lowest = words[i].y;
            target = i;
        }
    }
    return target;
}

// ฟังก์ชันแสดงผลและวาดหน้าจอ Phase 1
void drawPhase1(const GameState *game, FallingWord words[], int timeLeft)
{
    char field_ch[FIELD_H][FIELD_W + 1];
    int field_state[FIELD_H][FIELD_W + 1];

    for (int y = 0; y < FIELD_H; y++)
    {
        for (int x = 0; x < FIELD_W; x++)
        {
            field_ch[y][x] = ' ';
            field_state[y][x] = 0;
        }
        field_ch[y][FIELD_W] = '\0';
    }

    int targetIdx = findTargetWord(words);

    for (int i = 0; i < MAX_WORDS; i++)
    {
        if (!words[i].active || words[i].y >= FIELD_H)
            continue;

        if (i == targetIdx && words[i].x > 0)
        {
            field_ch[words[i].y][words[i].x - 1] = '>';
            field_state[words[i].y][words[i].x - 1] = 3;
        }

        for (int c = 0; words[i].text[c] != '\0'; c++)
        {
            if (words[i].x + c < FIELD_W)
            {
                field_ch[words[i].y][words[i].x + c] = words[i].text[c];
                field_state[words[i].y][words[i].x + c] = (c < words[i].typed) ? 2 : 1;
            }
        }
    }

    moveCursor(0, 0);
    printf("======================================================================\n");
    printf(" PHASE 1 : FALLING WORDS                             เวลาเหลือ : %3d sec\n", timeLeft);
    printf("----------------------------------------------------------------------\n");
    printf(" SCORE: %-5d | คำที่เคลียร์: %-3d | พิมพ์ผิด: %-3d\n", game->score, game->words_completed, game->errors);
    printf(" BOSS HP : ");
    printBossHPBar(game->boss_hp, game->max_boss_hp);
    printf("----------------------------------------------------------------------\n");

    const char **current_boss_art = (game->boss_hurt_timer > 0) ? BOSS_HURT : BOSS_NORMAL;

    for (int y = 0; y < FIELD_H; y++)
    {
        printf("|");
        for (int x = 0; x < FIELD_W; x++)
        {
            if (field_state[y][x] == 3)
            {
                printf("\033[93m%c\033[0m", field_ch[y][x]);
            }
            else if (field_state[y][x] == 2)
            {
                printf("\033[97m%c\033[0m", field_ch[y][x]);
            }
            else if (field_state[y][x] == 1)
            {
                if (game->freeze_time > 0)
                {
                    printf("\033[96m%c\033[0m", field_ch[y][x]);
                }
                else
                {
                    printf("\033[90m%c\033[0m", field_ch[y][x]);
                }
            }
            else
            {
                printf(" ");
            }
        }
        printf("|");

        switch (y)
        {
        case 1:
            printf("   [THE ROOMMATE]");
            break;
        case 3:
            printf("   %s", current_boss_art[0]);
            break;
        case 4:
            printf("   %s", current_boss_art[1]);
            break;
        case 5:
            printf("   %s", current_boss_art[2]);
            break;
        case 8:
            printf("   สกินผู้เล่น: %s", PLAYER_SKINS[game->selected_skin][3]);
            break;
        case 9:
            printf("   %s", PLAYER_SKINS[game->selected_skin][0]);
            break;
        case 10:
            printf("   %s", PLAYER_SKINS[game->selected_skin][1]);
            break;
        case 11:
            printf("   %s", PLAYER_SKINS[game->selected_skin][2]);
            break;
        default:
            printf("                 ");
            break;
        }
        printf("\n");
    }

    printf("----------------------------------------------------------------------\n");
    printf(" [คำแนะนำ] พิมพ์คำที่มีเครื่องหมาย > นำหน้าก่อน | [ \\ ] ร้านค้า | [ESC] ยอมแพ้\n");
    printf("======================================================================\n");
}

// ลูปหลักในการประมวลผล Phase 1
int runTypingPhase(GameState *game)
{
    FallingWord words[MAX_WORDS] = {0};

    DWORD start = GetTickCount();
    DWORD lastDrop = start;
    DWORD lastSecond = start;

    spawnWord(words);

    while (1)
    {
        DWORD now = GetTickCount();

        int elapsed = (int)((now - start) / 1000);
        int timeLeft = game->time_limit - elapsed;

        if (timeLeft <= 0)
            return 0;

        if (game->boss_hp <= (game->max_boss_hp / 2))
        {
            clearScreen();
            printf("============================================================\n");
            printf("         \033[91m!!! WARNING: ROOMMATE IS ENRAGED !!!\033[0m              \n");
            printf("============================================================\n\n");
            printf(" รูมเมท: \"หน็อยแน่! พิมพ์เร็วเรอะ?! เข้ามาเคลียร์กันตรงๆ ดีกว่า!\"\n\n");
            printf(" บอสโมโหแล้ว! ตัดเข้าสู่ช่วง DEBATE BATTLE (การดวลฝีปาก)\n");
            waitEnter();
            return 1;
        }

        if (now - lastSecond >= 1000)
        {
            lastSecond = now;
            if (game->freeze_time > 0)
                game->freeze_time--;
            if (game->double_score_time > 0)
                game->double_score_time--;
            if (game->boss_hurt_timer > 0)
                game->boss_hurt_timer--;
        }

        while (_kbhit())
        {
            int key = _getch();

            if (key == 27)
                return -1;

            if (key == '\\')
            {
                openShop(game);
                clearScreen();
                lastDrop = GetTickCount();
                continue;
            }

            if (isalpha(key))
            {
                int target = findTargetWord(words);

                if (target != -1)
                {
                    FallingWord *word = &words[target];

                    char expected = (char)tolower((unsigned char)word->text[word->typed]);
                    char typed = (char)tolower((unsigned char)key);

                    if (expected == typed)
                    {
                        word->typed++;

                        if (word->text[word->typed] == '\0')
                        {
                            int points = (int)strlen(word->text);

                            game->boss_hp -= points;
                            if (game->boss_hp < 0)
                                game->boss_hp = 0;

                            if (game->double_score_time > 0)
                                points *= 2;

                            game->score += points;
                            game->words_completed++;
                            game->boss_hurt_timer = 2;
                            word->active = 0;
                        }
                    }
                    else
                    {
                        game->errors++;
                    }
                }
            }
        }

        int speed = game->fall_speed;
        if (game->freeze_time > 0)
            speed += 1000;

        if (now - lastDrop >= (DWORD)speed)
        {
            lastDrop = now;
            for (int i = 0; i < MAX_WORDS; i++)
            {
                if (words[i].active)
                {
                    words[i].y++;
                    if (words[i].y >= FIELD_H)
                        words[i].active = 0;
                }
            }
            spawnWord(words);
        }

        if (findTargetWord(words) == -1)
            spawnWord(words);

        drawPhase1(game, words, timeLeft);
        Sleep(20);
    }
}