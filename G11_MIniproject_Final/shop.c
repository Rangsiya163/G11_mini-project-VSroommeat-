#include "shop.h"

//วาดรูป ASCII Art ของสกินตัวละครที่เลือก
void showPlayerSkin(int skin)
{
    printf("%s\n", PLAYER_SKINS[skin][0]);
    printf("%s\n", PLAYER_SKINS[skin][1]);
    printf("%s\n", PLAYER_SKINS[skin][2]);
}

//เมนูร้านค้าสกิน (Skin Shop)
void showSkinShop(GameState *game)
{
    int choice;
    while (1)
    {
        clearScreen();
        printf("============================================================\n");
        printf("                        SKIN SHOP                           \n");
        printf("============================================================\n");
        printf(" คะแนนของคุณ: %d pts\n\n", game->score);

        for (int i = 0; i < SKIN_COUNT; i++)
        {
            if (i == 3)
                printf("                >>> SPECIAL SKINS <<<\n");

            printf(" [%d] %s", i + 1, PLAYER_SKINS[i][3]);
            if (game->unlocked_skin[i])
                printf("  [มีแล้ว]\n");
            else
                printf("  [ปล็อคอยู่]\n");

            showPlayerSkin(i);
            printf("\n");

            if (i == 2)
                printf("------------------------------------------------------------\n");
        }

        printf("------------------------------------------------------------\n");
        printf(" [4] ซื้อ Cyber Freshy       100 pts\n");
        printf(" [5] ซื้อ Golden Student     200 pts\n");
        printf(" [6] ย้อนกลับ\n");
        printf("------------------------------------------------------------\n");
        printf(" เลือกรายการ: ");

        if (scanf("%d", &choice) != 1)
            choice = 0;
        while (getchar() != '\n')
            ;

        if (choice == 6)
            return;

        if (choice == 4 || choice == 5)
        {
            int index = choice - 1;
            int price = (choice == 4) ? 100 : 200;

            if (game->unlocked_skin[index])
            {
                printf("\nคุณมีสกินนี้อยู่แล้ว!\n");
            }
            else if (game->score < price)
            {
                printf("\nคะแนนไม่พอซื้อ!\n");
            }
            else
            {
                game->score -= price;
                game->unlocked_skin[index] = 1;
                printf("\nปลดล็อกสำเร็จ: %s!\n", PLAYER_SKINS[index][3]);
            }
            Sleep(1200);
        }
    }
}

//หน้าจอเลือกสกินตัวละครที่จะนำไปใช้ในเกม
void chooseSkin(GameState *game)
{
    int choice;
    clearScreen();
    printf("============================================================\n");
    printf("                      เลือกสกินตัวละคร                      \n");
    printf("============================================================\n\n");

    for (int i = 0; i < SKIN_COUNT; i++)
    {
        printf(" [%d] %s", i + 1, PLAYER_SKINS[i][3]);
        if (!game->unlocked_skin[i])
        {
            printf(" [ยังไม่ปลดล็อก]\n\n");
            continue;
        }
        printf(" [ใช้งานได้]\n");
        showPlayerSkin(i);
        printf("\n");
    }

    printf("เลือกสกิน (1-5): ");
    if (scanf("%d", &choice) != 1)
        choice = 1;
    while (getchar() != '\n')
        ;

    if (choice < 1 || choice > SKIN_COUNT)
        choice = 1;

    if (!game->unlocked_skin[choice - 1])
    {
        printf("สกินนี้ยังไม่ปลดล็อก กำหนดให้ใช้สกินที่ 1\n");
        choice = 1;
        Sleep(1000);
    }
    game->selected_skin = choice - 1;
}

//หน้าร้านค้าซื้อไอเทมช่วยเหลือ (Item Shop)
void openShop(GameState *game)
{
    int choice;
    while (1)
    {
        clearScreen();
        printf("============================================================\n");
        printf("                        ITEM SHOP                           \n");
        printf("============================================================\n");
        printf(" คะแนนสะสม: %d pts\n\n", game->score);

        printf(" [1] Ice Potion     แช่แข็งคำตกชั่วคราว 5 วิ         - 30 pts\n");
        printf(" [2] Double Score   คะแนนคูณสองเป็นเวลา 10 วิ    - 50 pts\n");
        printf(" [3] Energy Drink   เพิ่มหัวใจผู้เล่น +1 HP         - 40 pts\n");
        printf(" [4] ร้านค้าสกิน (Skin Shop)\n");
        printf(" [5] ออกจากร้านค้า\n\n");

        printf("------------------------------------------------------------\n");
        printf(" เลือกรายการ: ");

        if (scanf("%d", &choice) != 1)
            choice = 0;
        while (getchar() != '\n')
            ;

        if (choice == 5)
            return;

        if (choice == 4)
        {
            showSkinShop(game);
            continue;
        }

        if (choice == 1 && game->score >= 30)
        {
            game->score -= 30;
            game->freeze_time += 5;
            printf("เปิดใช้งาน Ice Potion เรียบร้อย!\n");
        }
        else if (choice == 2 && game->score >= 50)
        {
            game->score -= 50;
            game->double_score_time += 10;
            printf("เปิดใช้งาน Double Score เรียบร้อย!\n");
        }
        else if (choice == 3 && game->score >= 40)
        {
            game->score -= 40;
            if (game->player_hp < game->max_player_hp)
                game->player_hp++;
            printf("เพิ่มพลังชีวิต Player HP +1!\n");
        }
        else
        {
            printf("คะแนนไม่เพียงพอ หรือเลือกรายการไม่ถูกต้อง\n");
        }
        Sleep(1000);
    }
}