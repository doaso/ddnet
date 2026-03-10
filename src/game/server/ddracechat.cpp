/* (c) Shereef Marzouk. See "licence DDRace.txt" and the readme.txt in the root of the distribution for more information. */
#include "gamecontext.h"
#include "player.h"
#include "score.h"

#include <base/log.h>

#include <engine/shared/config.h>
#include <engine/shared/protocol.h>

#include <game/mapitems.h>
#include <game/server/entities/character.h>
#include <game/server/gamemodes/DDRace.h>
#include <game/server/teams.h>
#include <game/team_state.h>
#include <game/teamscore.h>
#include <game/version.h>

void CGameContext::ConCmdList(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	pSelf->SendMotd("Команды:\n"
                "/cmdlist - Команды\n"
                "/acmdlist - Команды администрации\n"
                "/rules - Правила\n"
                "/login - Авторизация\n"
                "/register - Регистрация\n"
                "/passwd - Поменять пароль\n"
                "/pay - Передать пойнты\n"
                "/drop - Выбросить оружие\n"
                "/rank - Посмотреть статистику\n"
                "/shop - Магазин\n"
                "/duel - Предложить дуэль\n"
                "/orel - Предложить сыграть в Орел и Решка, игра за сторону Орел\n"
                "/reshka - Предложить сыграть в Орел и Решка, игра за сторону Решка\n"
                "/donate - Донат\n"
                "/clb - тп на клб\n"
                "/mlt - тп на мульту"
                , pResult->m_ClientId);
}

void CGameContext::ConACmdList(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        int AdminLevel = pResult->GetInteger(0);
        if (pPlayer->m_AdminLevel < AdminLevel)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "acmdlist");
                return;
        }

        if (AdminLevel == 1)
        {
                pSelf->SendMotd("Команды для 1-го уровня админки:\n"
                        "/effect - Установить эффект себе/игроку\n"
                        "/givegun - Выдать игроку/себе оружие\n"
                        "/removegun - Забрать у игрока/себя оружие\n"
                        "/infjump - Включить/Выключить себе/игроку бесконечные прыжки\n"
                        "/infhook - Включить/Выключить себе/игроку бесконечный хук\n"
                        "/jetpack - Включить/Выключить себе/игроку джетпак"
                        , pResult->m_ClientId);
                return;
        }

        if (AdminLevel == 2)
        {
                pSelf->SendMotd("Команды для 2-го уровня админки:\n"
                        "/solo - Включить/Выключить себе/игроку соло мод\n"
                        "/colision - Включить/Выключить себе/игроку колизию\n"
                        "/hookothers - Включить/Выключить себе/игроку хук по игрокам\n"
                        "/hitothers - Включить/Выключить себе/игроку хиты по игрокам\n"
                        "/goto - Телепорт к игроку\n"
                        "/gethere - Телепортировать игрока\n"
                        "/tpspec - Включить/Выключить телепорт в спеке"
                        , pResult->m_ClientId);
                return;
        }

        if (AdminLevel == 3)
        {
                pSelf->SendMotd("Команды для 3-го уровня админки:\n"
                        "/stats - Посмотреть статистику игрока\n"
                        "/broadcast - Вывести сообщение на весь сервер\n"
                        "/motd - Вывести motd на весь сервер\n"
                        "/fakemsg - Отправить сообщение от лица игрока\n"
                        "/invincible - Включить/Выключить себе/игроку бессмертие\n"
                        "/super - Включить/Выключить себе/игроку суперку\n"
                        "/freeze - Заморозить/Разморозить себя/игрока\n"
                        "/kill - Убить игрока"
                        , pResult->m_ClientId);
                return;
        }

        if (AdminLevel == 4)
        {
                pSelf->SendMotd("Команды для 4-го уровня админки:\n"
                        "/kick - Кикнуть игрока\n"
                        "/mute - Замутить игрока\n"
                        "/unmute - Размутить игрока\n"
                        "/ban - Заблокировать игрока"
                        , pResult->m_ClientId);
                return;
        }

        if (AdminLevel == 5)
        {
                pSelf->SendMotd("Команды для 5-го уровня админки:\n"
                        "/settempadminlevel - Установить временную админку\n"
                        "/setadminlevel - Установить постоянную админку\n"
                        "/setlevel - Установить уровень\n"
                        "/setpoints - Установить пойнты\n"
                        "/setdonatrubles - Установить донат рубли"
                        , pResult->m_ClientId);
                return;
        }
}

void CGameContext::ConRules(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	pSelf->SendMotd(R"(Правила:
1.0 ДЛЯ ИГРОКОВ:
• Запрещены любого рода читы, баги, боты, макросы
• Не сливать личные данные игроков

2.0 ДЛЯ АДМИНОВ:
• Не злоупотреблять командами
• Быть вежливым и нейтральным
• Наказывать только с доказательствами
• Не давать преимуществ игрокам

3.0 ОБЩИЕ ПОЛОЖЕНИЯ
• Игнорирование правил не освобождает от ответственности.
• «Я не знал» — не оправдание.
• Администрация оставляет за собой право менять правила без предварительного оповещения.
                )"
                , pResult->m_ClientId);
}

void CGameContext::ConLogin(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

        if (str_length(pResult->GetString(0)) == 0)
        {
                pSelf->Help(pResult, pUserData, "login");
                return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_IsLogined) {
                return;
        }

        if (!pPlayer->m_IsRegistered)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Ваш аккаунт не зарегистрирован! Используйте /register что-бы создать аккаунт");
                return;
        }

        if (str_comp(pPlayer->m_aPassword, pResult->GetString(0)) != 0)
        {
                pPlayer->m_TryEnterPasswordCount--;
                if (pPlayer->m_TryEnterPasswordCount == 0) {
                        pSelf->Server()->Kick(pResult->m_ClientId, "Cлишком много попыток входа");
                        return;
                }

                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Неправильный пароль. Попыток входа осталось: %d",
                        pPlayer->m_TryEnterPasswordCount);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
                return;
        }

        pPlayer->m_IsLogined = true;
        pSelf->SendChatTarget(pResult->m_ClientId, "Что-бы посмотреть вашу статистику введите /rank в чате");
		pSelf->SendChatTarget(pResult->m_ClientId, "Используйте /info что-бы посмотерть инофрмацию об сервере");
}

void CGameContext::ConRegister(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

        if (pResult->NumArguments() != 2)
        {
                pSelf->Help(pResult, pUserData, "register");
                return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_IsLogined) {
                return;
        }

        if (pPlayer->m_IsRegistered)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Ваш аккаунт был зарегистрирован! Используйте /login что-бы войти");
                return;
        }

        if (str_comp(pResult->GetString(0), pResult->GetString(1)) != 0)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Пароли не совпадают");
                return;
        }

        pSelf->Score()->Register(pSelf->Server()->ClientName(pResult->m_ClientId), pResult->GetString(0));
        pPlayer->m_IsLogined = true;
        pPlayer->m_IsRegistered = true;
        pSelf->SendChatTarget(pResult->m_ClientId, "Что-бы посмотреть вашу статистику введите /rank в чате");
		pSelf->SendChatTarget(pResult->m_ClientId, "Используйте /info что-бы посмотерть инофрмацию об сервере");
		
}

void CGameContext::ConPasswd(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

        if (pResult->NumArguments() != 2) {
                pSelf->Help(pResult, pUserData, "passwd");
                return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        const char* CurrentPassword = pResult->GetString(0);
        if (str_comp(pPlayer->m_aPassword, CurrentPassword) != 0)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Неправильный пароль");
                return;
        }

        const char* NewPassword = pResult->GetString(1);
        if (str_comp(CurrentPassword, NewPassword) == 0)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Новый пароль не должен совпадать с текущим");
                return;
        }

	str_copy(pPlayer->m_aPassword, NewPassword, sizeof(pPlayer->m_aPassword));
        pSelf->Score()->ChangePassword(pSelf->Server()->ClientName(pResult->m_ClientId), NewPassword);
        pSelf->SendChatTarget(pResult->m_ClientId, "Вы сменили пароль");
}

void CGameContext::ConPay(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pResult->NumArguments() != 2)
        {
                pSelf->Help(pResult, pUserData, "pay");
                return;
        }

        int Points = pResult->GetInteger(1);
        if (Points <= 0)
                return;

        if (std::uint32_t(Points) > pPlayer->m_Points)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно пойнтов");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	if(TargetClientId == pResult->m_ClientId)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Вы не можете применить это действие на себя");
		return;
        }

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        if (!pPlayerTarget->m_IsRegistered)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Игрок не зарегистрирован");
                return;
        }

        pPlayer->m_Points -= Points;
        pSelf->Score()->ChangePoints(pSelf->Server()->ClientName(pResult->m_ClientId), pPlayer->m_Points);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Вы передали %s %i пойнтов",
                TargetPlayerName, Points);
        pSelf->SendChatTarget(pResult->m_ClientId, aBuf);

        pPlayerTarget->m_Points += Points;
        pSelf->Score()->ChangePoints(TargetPlayerName, pPlayerTarget->m_Points);
        str_format(aBuf, sizeof(aBuf),
                "%s передал вам %i пойнтов",
                pSelf->Server()->ClientName(pResult->m_ClientId), Points);
        pSelf->SendChatTarget(TargetClientId, aBuf);
}

void CGameContext::ConDrop(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
                return;

        int ActiveWeapon = pChr->GetActiveWeapon();
        if (ActiveWeapon == WEAPON_HAMMER || ActiveWeapon == WEAPON_GUN)
                return;

        pSelf->CreatePickup(pChr->m_Pos, POWERUP_WEAPON, ActiveWeapon);
        pChr->GiveWeapon(ActiveWeapon, true);
}

void CGameContext::ConRankC(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        char aBuf[256];
        str_format(aBuf, sizeof(aBuf), 
                "%i LVL | %i/%i XP | %i ПОЙНТОВ",
                pPlayer->m_Level, pPlayer->m_XP, pPlayer->m_TargetXP, pPlayer->m_Points);
        pSelf->SendBroadcast(aBuf, pResult->m_ClientId);
}

void CGameContext::ConShop(IConsole::IResult *pResult, void *pUserData)
{
    CGameContext *pSelf = (CGameContext *)pUserData;
    if(!CheckClientId(pResult->m_ClientId))
        return;

    CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
    if (!pPlayer)
        return;

    struct Items { const char *Name; std::uint32_t Price; std::uint32_t Level; };
    static constexpr const Items ITEMS[] = {
        {"Дробовик", 100, 1},
        {"Гранатомет", 150, 5},
        {"Лазер", 150, 5},
        {"Ниндзя", 250, 10},
        {"Эффект Смерти", 5000, 25},
        {"Эффект Огненой ауры", 2500, 10},
        {"Эффект Хеппи-Бердей", 1000, 5},
        {"Эффект Хамер-Дробовик", 2500, 20},
        {"Эффект Хамер-Лазер", 5000, 25},
    };
    constexpr size_t ITEMS_COUNT = std::size(ITEMS);

    if (pResult->NumArguments() == 0)
    {
        char aBuf[1024] = "Магазин:\n";
        for (size_t index = 0; index < ITEMS_COUNT; index++)
        {
            char aLine[256];
            str_format(aLine, sizeof(aLine), "%d) %s за %d пойнтов, доступен от %d уровня\n", 
                index + 1, ITEMS[index].Name, ITEMS[index].Price, ITEMS[index].Level);
            str_append(aBuf, aLine, sizeof(aBuf));
        }

        pSelf->SendMotd(aBuf, pResult->m_ClientId);
        return;
    }

    CCharacter *pChr = pPlayer->GetCharacter();
    if(!pChr)
        return;

    int ID = pResult->GetInteger(0);
    if (ID < 1 || static_cast<size_t>(ID) > ITEMS_COUNT) {
        return;
    }

    if (pPlayer->m_Points < ITEMS[ID - 1].Price) {
        pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно пойнтов");
        return;
    }

    if (pPlayer->m_Level < ITEMS[ID - 1].Level) {
        pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно уровня");
        return;
    }

    switch (ID)
    {
        case 1: pChr->GiveWeapon(2, false); break;
        case 2: pChr->GiveWeapon(3, false); break;
        case 3: pChr->GiveWeapon(4, false); break;
        case 4: pChr->GiveWeapon(5, false); break;
        case 5: pPlayer->m_EffectID = 1; break;
        case 6: pPlayer->m_EffectID = 2; break;
        case 7: pPlayer->m_EffectID = 3; break;
        case 8: pPlayer->m_EffectID = 4; break;
        case 9: pPlayer->m_EffectID = 5; break;
        default: pSelf->SendChatTarget(pPlayer->GetCid(), "Неправильный ID товара"); return;
    }

    pPlayer->m_Points -= ITEMS[ID - 1].Price;
    pSelf->Score()->ChangePoints(pSelf->Server()->ClientName(pPlayer->GetCid()), pPlayer->m_Points);

    char aBuf[128];
    str_format(aBuf, sizeof(aBuf), "Вы купили %s за %d пойнтов", ITEMS[ID - 1].Name, ITEMS[ID - 1].Price);
    pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
}


void CGameContext::ConDonate(IConsole::IResult *pResult, void *pUserData)
{
    CGameContext *pSelf = (CGameContext *)pUserData;
    if(!CheckClientId(pResult->m_ClientId))
        return;

    CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
    if (!pPlayer)
        return;

    struct Items { const char *Name; std::uint32_t Price; };
    static constexpr const Items ITEMS[] = {
        {"Админка 1-го уровня", 100},
        {"Админка 2-го уровня", 250},
    };
    constexpr size_t ITEMS_COUNT = std::size(ITEMS);

    if (pResult->NumArguments() == 0)
    {
        char aBuf[1024] = "Магазин за донат рубли:\n";
        for (size_t index = 0; index < ITEMS_COUNT; index++)
        {
            char aLine[256];
            str_format(aLine, sizeof(aLine), "%d) %s за %d донат рублей\n", 
                index + 1, ITEMS[index].Name, ITEMS[index].Price);
            str_append(aBuf, aLine, sizeof(aBuf));
        }

        str_append(aBuf, "\n\n>>> ЗАДОНАТИТЬ <<<\nhttps://www.tbank.ru/cf/2llcosB2Eje\n\nВ коментариях указываете ник на сервере", sizeof(aBuf));
        pSelf->SendMotd(aBuf, pResult->m_ClientId);
        return;
    }

    CCharacter *pChr = pPlayer->GetCharacter();
    if(!pChr)
        return;

    int ID = pResult->GetInteger(0);
    if (ID < 1 || static_cast<size_t>(ID) > ITEMS_COUNT) {
        return;
    }

    if (pPlayer->m_DonateRubles < ITEMS[ID - 1].Price) {
        pSelf->SendChatTarget(pPlayer->GetCid(), "У вас недостаточно донат рублей");
        return;
    }

    switch (ID)
    {
        case 1:
            pPlayer->m_AdminLevel = 1;
            pSelf->Score()->ChangeAdminLevel(pSelf->Server()->ClientName(pPlayer->GetCid()), pPlayer->m_AdminLevel);
            pSelf->SendChatTarget(pPlayer->GetCid(), "Используйте /acmdlist что-бы посмотреть ваши команды");
            break;
        case 2:
            pPlayer->m_AdminLevel = 2;
            pSelf->Score()->ChangeAdminLevel(pSelf->Server()->ClientName(pPlayer->GetCid()), pPlayer->m_AdminLevel);
            pSelf->SendChatTarget(pPlayer->GetCid(), "Используйте /acmdlist что-бы посмотреть ваши команды");
            break;
        default: pSelf->SendChatTarget(pPlayer->GetCid(), "Неправильный ID товара"); return;
    }

    pPlayer->m_DonateRubles -= ITEMS[ID - 1].Price;
    pSelf->Score()->ChangeDonateRubles(pSelf->Server()->ClientName(pPlayer->GetCid()), pPlayer->m_DonateRubles);

    char aBuf[128];
    str_format(aBuf, sizeof(aBuf), "Вы купили %s за %d донат рублей", ITEMS[ID - 1].Name, ITEMS[ID - 1].Price);
    pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
}

void CGameContext::ConDuel(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

        if (pResult->NumArguments() != 3)
        {
                pSelf->Help(pResult, pUserData, "duel");
                return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        std::uint32_t Bid = pResult->GetInteger(1);
        if (Bid <= 0)
                return;

        if (pPlayer->m_Points < Bid)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно пойнтов");
                return;
        }

        std::uint8_t Round = pResult->GetInteger(2);
        if (Round < 1 || Round > 5)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Количество раундов должно быть 1-5");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	if(TargetClientId == pResult->m_ClientId)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Вы не можете применить это действие к себе");
		return;
        }

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        if (pPlayerTarget->m_Points < Bid)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У соперника недостаточно пойнтов");
                return;
        }

        pPlayerTarget->m_DuelBid = Bid;
        pPlayerTarget->m_DuelFromClientId = pResult->m_ClientId;
        pPlayerTarget->m_DuelRound = Round;

        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Вы предложили дуэль %s на %i пойнтов, продолжительностью в %i раунд(ов)",
                TargetPlayerName, Bid, Round);
        pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
        str_format(aBuf, sizeof(aBuf),
                "%s предложил дуэль  на %i пойнтов, продолжительностью в %i раунд(ов)",
                pSelf->Server()->ClientName(pResult->m_ClientId), Bid, Round);
        pSelf->SendChatTarget(TargetClientId, aBuf);
        pSelf->SendChatTarget(TargetClientId, "Что-бы согласиться введите /yes, что-бы отказаться введите /no");
}

void CGameContext::ConOrel(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

        if (pResult->NumArguments() != 2)
        {
                pSelf->Help(pResult, pUserData, "orel");
                return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        std::uint32_t Bid = pResult->GetInteger(1);
        if (Bid <= 0)
                return;

        if (pPlayer->m_Points < Bid)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно пойнтов");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	if(TargetClientId == pResult->m_ClientId)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Вы не можете применить это действие к себе");
		return;
        }

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        if (pPlayerTarget->m_Points < Bid)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У соперника недостаточно пойнтов");
                return;
        }

        pPlayerTarget->m_OrelAndReshkaBid = Bid;
        pPlayerTarget->m_OrelAndReshkaIsOrel = false;
        pPlayerTarget->m_OrelAndReshkaFromClientId = pResult->m_ClientId;

        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Вы предложили сыграть в Орел и Решка %s на %i пойнтов, ваша сторона: Орел",
                TargetPlayerName, Bid);
        pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
        str_format(aBuf, sizeof(aBuf),
                "%s предложил сыграть вам в Орел и Решка на %i пойнтов, ваша сторона: Решка",
                pSelf->Server()->ClientName(pResult->m_ClientId), Bid);
        pSelf->SendChatTarget(TargetClientId, aBuf);
        pSelf->SendChatTarget(TargetClientId, "Что-бы согласиться введите /yes, что-бы отказаться введите /no");
}

void CGameContext::ConReshka(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

        if (pResult->NumArguments() != 2)
        {
                pSelf->Help(pResult, pUserData, "reshka");
                return;
        }

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        std::uint32_t Bid = pResult->GetInteger(1);
        if (Bid <= 0)
                return;

        if (pPlayer->m_Points < Bid)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно пойнтов");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	if(TargetClientId == pResult->m_ClientId)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Вы не можете применить это действие к себе");
		return;
        }

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        if (pPlayerTarget->m_Points < Bid)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У соперника недостаточно пойнтов");
                return;
        }

        pPlayerTarget->m_OrelAndReshkaBid = Bid;
        pPlayerTarget->m_OrelAndReshkaIsOrel = true;
        pPlayerTarget->m_OrelAndReshkaFromClientId = pResult->m_ClientId;

        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Вы предложили сыграть в Орел и Решка %s на %i пойнтов, ваша сторона: Решка",
                TargetPlayerName, Bid);
        pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
        str_format(aBuf, sizeof(aBuf),
                "%s предложил сыграть вам в Орел и Решка на %i пойнтов, ваша сторона: Орел",
                pSelf->Server()->ClientName(pResult->m_ClientId), Bid);
        pSelf->SendChatTarget(TargetClientId, aBuf);
        pSelf->SendChatTarget(TargetClientId, "Что-бы согласиться введите /yes, что-бы отказаться введите /no");
}

void CGameContext::ConYes(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_IsDuelStart)
                return;

        if (pPlayer->m_DuelFromClientId != -1 || pPlayer->m_OrelAndReshkaFromClientId != -1)
        {
                if (pPlayer->m_DuelFromClientId != -1)
                {
                        std::uint32_t Bid = pPlayer->m_DuelBid;
                        if (pPlayer->m_Points < Bid)
                        {
                                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно пойнтов");
                                return;
                        }    

                        CPlayer *pPlayerTarget = pSelf->m_apPlayers[pPlayer->m_DuelFromClientId];
                        if(!pPlayerTarget)
                        {
                                pSelf->SendChatTarget(pResult->m_ClientId, "Игрок отключился");
                                pPlayer->m_DuelFromClientId = -1;
                                return;
                        }


                        if (pPlayerTarget->m_Points < Bid)
                        {
                                pSelf->SendChatTarget(pResult->m_ClientId, "У соперника недостаточно пойнтов");
                                return;
                        }

                        char aBuf[255];
                        str_format(aBuf, sizeof(aBuf),
                                "%s принял дуэль",
                                pSelf->Server()->ClientName(pResult->m_ClientId));
                        pSelf->SendChatTarget(pPlayer->m_DuelFromClientId, aBuf);

                        pPlayer->m_IsDuelStart = true;
                        pPlayer->m_DuelScore = 0;

                        CCharacter *pChr = pPlayer->GetCharacter();
                        if(!pChr)
                                return;

                        for (int Team = 1; Team < 20; Team++)
                        {
                                if(pSelf->m_pController->Teams().Count(Team) == 0) {
                                        pSelf->m_pController->Teams().SetForceCharacterTeam(pPlayer->GetCid(), Team);
                                        pSelf->m_pController->Teams().SetForceCharacterTeam(pPlayerTarget->GetCid(), Team);
                                        break;
                                }
                        }

                        pChr->ResetJumps();
                        pChr->UnFreeze();
                        pChr->ResetVelocity();
                        pSelf->CreatePlayerSpawn(pChr->m_Pos);
                        pChr->SetPosition(vec2(106.4f * 32.0f, 54.4f * 32.0f));
                        pChr->SetBlock(false);
                        pSelf->CreatePlayerSpawn(pChr->m_Pos);

                        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
                        if(!pChrTarget)
                                return;

                        pChrTarget->ResetJumps();
                        pChrTarget->UnFreeze();
                        pChrTarget->ResetVelocity();
                        pSelf->CreatePlayerSpawn(pChrTarget->m_Pos);
                        pChrTarget->SetPosition(vec2(90.4f * 32.0f, 54.4f * 32.0f));
                        pChrTarget->SetBlock(false);
                        pSelf->CreatePlayerSpawn(pChrTarget->m_Pos);

                        pPlayerTarget->m_IsDuelStart = true;
                        pPlayerTarget->m_DuelScore = 0;

                        return;
                }

                std::uint32_t Bid = pPlayer->m_OrelAndReshkaBid;
                if (pPlayer->m_Points < Bid)
                {
                        pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно пойнтов");
                        return;
                }

                CPlayer *pPlayerTarget = pSelf->m_apPlayers[pPlayer->m_OrelAndReshkaFromClientId];
                if(!pPlayerTarget)
                {
                        pSelf->SendChatTarget(pResult->m_ClientId, "Игрок отключился");
                        pPlayer->m_OrelAndReshkaFromClientId = -1;
                        return;
                }

                if (pPlayerTarget->m_Points < Bid)
                {
                        pSelf->SendChatTarget(pResult->m_ClientId, "У соперника недостаточно пойнтов");
                        return;
                }

                char aBuf[255];
                if (pPlayer->m_OrelAndReshkaIsOrel == rand() % 2)
                {
                        str_format(aBuf, sizeof(aBuf),
                                "Выпала: Решка, вы выиграли у %s %i пойнтов",
                                pSelf->Server()->ClientName(pPlayer->m_OrelAndReshkaFromClientId), Bid);
                        pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
                        pSelf->SendBroadcast("Победа!", pResult->m_ClientId);

                        str_format(aBuf, sizeof(aBuf),
                                "Выпала: Решка, вы проиграли %s %i пойнтов",
                                pSelf->Server()->ClientName(pResult->m_ClientId), Bid);
                        pSelf->SendChatTarget(pPlayer->m_OrelAndReshkaFromClientId, aBuf);
                        pSelf->SendBroadcast("Поражение!", pPlayer->m_OrelAndReshkaFromClientId);

                        pPlayer->m_Points += Bid;
                        pSelf->Score()->ChangePoints(pSelf->Server()->ClientName(pResult->m_ClientId), pPlayer->m_Points);

                        pPlayerTarget->m_Points -=  Bid;
                        pSelf->Score()->ChangePoints(pSelf->Server()->ClientName(pPlayer->m_OrelAndReshkaFromClientId), pPlayerTarget->m_Points);

                        pPlayer->m_OrelAndReshkaFromClientId = -1;
                        return;
                }


                str_format(aBuf, sizeof(aBuf),
                        "Выпал: Орел, вы проиграли %s %i пойнтов",
                        pSelf->Server()->ClientName(pPlayer->m_OrelAndReshkaFromClientId), Bid);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
                pSelf->SendBroadcast("Поражение!", pResult->m_ClientId);

                str_format(aBuf, sizeof(aBuf),
                        "Выпал: Орел, вы выиграли у %s %i пойнтов",
                        pSelf->Server()->ClientName(pResult->m_ClientId), Bid);
                pSelf->SendChatTarget(pPlayer->m_OrelAndReshkaFromClientId, aBuf);
                pSelf->SendBroadcast("Победа!", pPlayer->m_OrelAndReshkaFromClientId);

                pPlayer->m_Points -= Bid;
                pSelf->Score()->ChangePoints(pSelf->Server()->ClientName(pResult->m_ClientId), pPlayer->m_Points);

                pPlayerTarget->m_Points +=  Bid;
                pSelf->Score()->ChangePoints(pSelf->Server()->ClientName(pPlayer->m_OrelAndReshkaFromClientId), pPlayerTarget->m_Points);

                pPlayer->m_OrelAndReshkaFromClientId = -1;
                return;
        }

        pSelf->SendChatTarget(pResult->m_ClientId, "У вас нету активных заявок");
}

void CGameContext::ConNo(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_IsDuelStart)
                return;

        if (pPlayer->m_DuelFromClientId != -1 || pPlayer->m_OrelAndReshkaFromClientId != -1)
        {
                if (pPlayer->m_DuelFromClientId != -1)
                {
                        CPlayer *pPlayerTarget = pSelf->m_apPlayers[pPlayer->m_DuelFromClientId];
                        if(!pPlayerTarget)
                                return;

                        pSelf->SendChatTarget(pResult->m_ClientId, "Вы отказались от дуэли");

                        char aBuf[255];
                        str_format(aBuf, sizeof(aBuf),
                                "%s отказался от дуэли",
                                pSelf->Server()->ClientName(pResult->m_ClientId));
                        pSelf->SendChatTarget(pPlayer->m_DuelFromClientId, aBuf);
                        pPlayer->m_DuelFromClientId = -1;
                        return;
                }

                CPlayer *pPlayerTarget = pSelf->m_apPlayers[pPlayer->m_OrelAndReshkaFromClientId];
                if(!pPlayerTarget)
                        return;

                pSelf->SendChatTarget(pResult->m_ClientId, "Вы отказались от игры в Орел и Решка");

                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "%s отказался от игры в Орел и Решка",
                        pSelf->Server()->ClientName(pResult->m_ClientId));
                pSelf->SendChatTarget(pPlayer->m_OrelAndReshkaFromClientId, aBuf);
                pPlayer->m_OrelAndReshkaFromClientId = -1;
                return;
        }

        pSelf->SendChatTarget(pResult->m_ClientId, "У вас нету активных заявок");
}

void CGameContext::ConCLB(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

    CCharacter *pChr = pPlayer->GetCharacter();
    if(!pChr)
        return;

    pChr->SetBlock(true);
    pChr->SetPosition(vec2(47.5f * 32.0f, 94.0f * 32.0f));
}

void CGameContext::ConMLT(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

    CCharacter *pChr = pPlayer->GetCharacter();
    if(!pChr)
        return;

    pChr->SetBlock(true);
    pChr->SetPosition(vec2(8.2f * 32.0f, 153.4f * 32.0f));
}

void CGameContext::ConEffect(IConsole::IResult *pResult, void *pUserData) {
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 1)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "effect");
                return;
        }

        int EffectID = (pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1);
        const char* EffectName = "";
        if (EffectID == 0)
        {
            EffectName = "ничего";
        }
        else if (EffectID == 1)
        {
            EffectName = "смерти";
        }
        else if (EffectID == 2)
        {
            EffectName = "огненой ауры";
        }
        else if (EffectID == 3)
        {
            EffectName = "хеппи-бердей";
        }
        else if (EffectID == 4)
        {
            EffectName = "хаммер-дробовик";
        }
        else if (EffectID == 5)
        {
            EffectName = "хаммер-лазер";
        }
        else
        {
                return;
        }

	if(pResult->NumArguments() == 1)
        {
                pPlayer->m_EffectID = EffectID;
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Вы установили себе эффект %s",
                        EffectName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        pPlayerTarget->m_EffectID = EffectID;
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s установил эффект %s %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), EffectName, TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConGiveGun(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 1)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "givegun");
                return;
        }

        int WeaponID = (pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1);
        const char* WeaponName = "";
        if (WeaponID == WEAPON_HAMMER)
        {
                WeaponName = "хамер";
        }
        else if (WeaponID == WEAPON_GUN)
        {
                WeaponName = "пистолет";
        }
        else if (WeaponID == WEAPON_SHOTGUN)
        {
                WeaponName = "дробовик";
        }
        else if (WeaponID == WEAPON_GRENADE)
        {
                WeaponName = "гранатомет";
        }
        else if (WeaponID == WEAPON_LASER)
        {
                WeaponName = "лазер";
        }
        else if (WeaponID == WEAPON_NINJA)
        {
                WeaponName = "ниндзя";
        }
        else
        {
                return;
        }

	if(pResult->NumArguments() == 1)
        {
                CCharacter *pChr = pPlayer->GetCharacter();
                if (!pChr)
                        return;

                pChr->GiveWeapon(WeaponID, false);
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Вы выдали себе %s",
                        WeaponName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pChrTarget->GiveWeapon(WeaponID, false);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s выдал %s %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), WeaponName, TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConRemoveGun(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 1)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "removegun");
                return;
        }

        int WeaponID = (pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1);
        const char* WeaponName = "";
        if (WeaponID == WEAPON_HAMMER)
        {
                WeaponName = "хамер";
        }
        else if (WeaponID == WEAPON_GUN)
        {
                WeaponName = "пистолет";
        }
        else if (WeaponID == WEAPON_SHOTGUN)
        {
                WeaponName = "дробовик";
        }
        else if (WeaponID == WEAPON_GRENADE)
        {
                WeaponName = "гранатомет";
        }
        else if (WeaponID == WEAPON_LASER)
        {
                WeaponName = "лазер";
        }
        else if (WeaponID == WEAPON_NINJA)
        {
                WeaponName = "ниндзя";
        }
        else
        {
                return;
        }

	if(pResult->NumArguments() == 1)
        {
                CCharacter *pChr = pPlayer->GetCharacter();
                if (!pChr)
                        return;

                pChr->GiveWeapon(WeaponID, true);
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Вы забрали у себя %s", // это можно убрать помоему 
                        WeaponName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pChrTarget->GiveWeapon(WeaponID, true);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s забрал %s у %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), WeaponName, TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConInfJump(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 1)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "infjump");
                return;
        }

        bool IsEnabled = ((pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1) == 1);
	if(pResult->NumArguments() == 1)
        {
                CCharacter *pChr = pPlayer->GetCharacter();
                if (!pChr)
                        return;

                pChr->SetEndlessJump(IsEnabled);
                pSelf->SendChatTarget(pResult->m_ClientId, (IsEnabled == true) ? "Вы включили бесконечные прыжки" : "Вы выключили бесконечные прыжки");

                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pChrTarget->SetEndlessJump(IsEnabled);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                (IsEnabled == true) ? "Администратор %s включил бесконечные прыжки %s" : "Администратор %s выключил бесконечные прыжки %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConInfHook(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 1)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "infjump");
                return;
        }

        bool IsEnabled = ((pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1) == 1);
	if(pResult->NumArguments() == 1)
        {
                CCharacter *pChr = pPlayer->GetCharacter();
                if (!pChr)
                        return;

                pChr->SetEndlessHook(IsEnabled);
                pSelf->SendChatTarget(pResult->m_ClientId, (IsEnabled == true) ? "Вы включили бесконечный хук" : "Вы выключили бесконечный хук");

                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pChrTarget->SetEndlessHook(IsEnabled);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                (IsEnabled == true) ? "Администратор %s включил бесконечный хук %s" : "Администратор %s выключил бесконечный хук %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConJetpack(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 1)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "jetpack");
                return;
        }

        bool IsEnabled = ((pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1) == 1);
	if(pResult->NumArguments() == 1)
        {
                CCharacter *pChr = pPlayer->GetCharacter();
                if (!pChr)
                        return;

                pChr->SetJetpack(IsEnabled);
                pSelf->SendChatTarget(pResult->m_ClientId, (IsEnabled == true) ? "Вы включили джетпак" : "Вы выключили джетпак");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pChrTarget->SetJetpack(IsEnabled);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                (IsEnabled == true) ? "Администратор %s включил джетпак %s" : "Администратор %s выключил джетпак %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConSolo(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 2)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "solo");
                return;
        }

        bool IsEnabled = ((pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1) == 1);
	if(pResult->NumArguments() == 1)
        {
                CCharacter *pChr = pPlayer->GetCharacter();
                if (!pChr)
                        return;

                pChr->SetSolo(IsEnabled);
                pSelf->SendChatTarget(pResult->m_ClientId, (IsEnabled == true) ? "Вы включили соло мод" : "Вы выключили соло мод");

                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pChrTarget->SetSolo(IsEnabled);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                (IsEnabled == true) ? "Администратор %s включил соло мод %s" : "Администратор %s выключил соло мод %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConFreeze(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 3)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "freeze");
                return;
        }

        bool IsEnabled = ((pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1) == 1);
	if(pResult->NumArguments() == 1)
        {
                CCharacter *pChr = pPlayer->GetCharacter();
                if (!pChr)
                        return;

                pChr->SetDeepFrozen(IsEnabled);
                if (!IsEnabled)
                    pChr->UnFreeze();

                pSelf->SendChatTarget(pResult->m_ClientId, (IsEnabled == true) ? "Вы заморозили себя" : "Вы разморозили себя");

                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;


        pChrTarget->SetDeepFrozen(IsEnabled);
        if (!IsEnabled)
            pChrTarget->UnFreeze();

        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                (IsEnabled == true) ? "Администратор %s заморозил %s" : "Администратор %s разморозил %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConSuper(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 3)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "super");
                return;
        }

        bool IsEnabled = ((pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1) == 1);
	if(pResult->NumArguments() == 1)
        {
                CCharacter *pChr = pPlayer->GetCharacter();
                if (!pChr)
                        return;

                pChr->SetSuper(IsEnabled);
                pSelf->SendChatTarget(pResult->m_ClientId, (IsEnabled == true) ? "Вы включили суперку" : "Вы выключили суперку");

                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pChrTarget->SetSuper(IsEnabled);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                (IsEnabled == true) ? "Администратор %s включил суперку %s" : "Администратор %s выключил суперку %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConInvincible(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 3)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "invincible");
                return;
        }

        bool IsEnabled = ((pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1) == 1);
	if(pResult->NumArguments() == 1)
        {
                CCharacter *pChr = pPlayer->GetCharacter();
                if (!pChr)
                        return;

                pChr->SetInvincible(IsEnabled);
                pSelf->SendChatTarget(pResult->m_ClientId, (IsEnabled == true) ? "Вы включили бессмертие" : "Вы выключили бессмертие");

                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pChrTarget->SetInvincible(IsEnabled);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                (IsEnabled == true) ? "Администратор %s включил бессмертие %s" : "Администратор %s выключил бессмертие %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConColision(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 2)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "colision");
                return;
        }

        bool IsEnabled = ((pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1) == 0);
	if(pResult->NumArguments() == 1)
        {
                CCharacter *pChr = pPlayer->GetCharacter();
                if (!pChr)
                        return;

                pChr->SetCollisionDisabled(IsEnabled);
                pSelf->SendChatTarget(pResult->m_ClientId, (IsEnabled == true) ? "Вы выключили колизию" : "Вы включили колизию");

                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pChrTarget->SetCollisionDisabled(IsEnabled);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                (IsEnabled == true) ? "Администратор %s выключил колизию %s" : "Администратор %s включил колизию %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConHookOthers(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 2)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "colision");
                return;
        }

        bool IsEnabled = ((pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1) == 0);
	if(pResult->NumArguments() == 1)
        {
                CCharacter *pChr = pPlayer->GetCharacter();
                if (!pChr)
                        return;

                pChr->SetHookHitDisabled(IsEnabled);
                pSelf->SendChatTarget(pResult->m_ClientId, (IsEnabled == true) ? "Вы выключили хук по игрокам" : "Вы включили хук по игрокам");

                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pChrTarget->SetHookHitDisabled(IsEnabled);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                (IsEnabled == true) ? "Администратор %s выключил хук по игрокам %s" : "Администратор %s включил хук по игрокам %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConHitOthers(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 2)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() == 0)
        {
                pSelf->Help(pResult, pUserData, "hitothers");
                return;
        }

        bool IsEnabled = ((pResult->NumArguments() == 1) ? pResult->GetInteger(0) : pResult->GetInteger(1) == 0);
	if(pResult->NumArguments() == 1)
        {
                CCharacter *pChr = pPlayer->GetCharacter();
                if (!pChr)
                        return;

		pChr->SetHammerHitDisabled(IsEnabled);
		pChr->SetShotgunHitDisabled(IsEnabled);
		pChr->SetGrenadeHitDisabled(IsEnabled);
		pChr->SetLaserHitDisabled(IsEnabled);
                pSelf->SendChatTarget(pResult->m_ClientId, (IsEnabled == true) ? "Вы выключили хиты по игрокам" : "Вы включили хиты по игрокам");

                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pChrTarget->SetHammerHitDisabled(IsEnabled);
        pChrTarget->SetShotgunHitDisabled(IsEnabled);
        pChrTarget->SetGrenadeHitDisabled(IsEnabled);
        pChrTarget->SetLaserHitDisabled(IsEnabled);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                (IsEnabled == true) ? "Администратор %s выключил хиты по игрокам %s" : "Администратор %s включил хиты по игрокам %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConKill(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (str_length(pResult->GetString(0)) == 0)
        {

                if(pPlayer->m_LastKill && pPlayer->m_LastKill + pSelf->Server()->TickSpeed() * g_Config.m_SvKillDelay > pSelf->Server()->Tick())
                        return;

                pPlayer->m_LastKill = pSelf->Server()->Tick();
                pPlayer->KillCharacter(WEAPON_SELF);
                return;
        }

        if (pPlayer->m_AdminLevel < 3)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pChrTarget->Die(pResult->m_ClientId, WEAPON_HAMMER, false);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s убил %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConGoto(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        CCharacter *pChr = pPlayer->GetCharacter();
        if (!pChr)
                return;

        if (pPlayer->m_AdminLevel < 2)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (str_length(pResult->GetString(0)) == 0)
        {
                pSelf->Help(pResult, pUserData, "goto");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	if(TargetClientId == pResult->m_ClientId)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Вы не можете применить это действие к себе");
		return;
        }

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pSelf->CreatePlayerSpawn(pChr->m_Pos);
        pSelf->CreatePlayerSpawn(pChrTarget->m_Pos);
        pSelf->Teleport(pChr, pChrTarget->m_Pos);
	pChr->ResetJumps();
	pChr->UnFreeze();
	pChr->ResetVelocity();
        pChr->SetBlock(pChrTarget->Core()->m_IsBlockMode);

        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "К вам телепортировался Администратор %s",
                pSelf->Server()->ClientName(pResult->m_ClientId));
        pSelf->SendChatTarget(pPlayerTarget->GetCid(), aBuf);
}

void CGameContext::ConGetHere(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        CCharacter *pChr = pPlayer->GetCharacter();
        if (!pChr)
                return;

        if (pPlayer->m_AdminLevel < 2)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (str_length(pResult->GetString(0)) == 0)
        {
                pSelf->Help(pResult, pUserData, "gethere");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	if(TargetClientId == pResult->m_ClientId)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Вы не можете применить это действие к себе");
		return;
        }

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        pSelf->CreatePlayerSpawn(pChr->m_Pos);
        pSelf->CreatePlayerSpawn(pChrTarget->m_Pos);
        pSelf->Teleport(pChrTarget, pChr->m_Pos);
	pChrTarget->ResetJumps();
	pChrTarget->UnFreeze();
	pChrTarget->ResetVelocity();
        pChrTarget->SetBlock(pChr->Core()->m_IsBlockMode);

        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Вас телепортировал Администратор %s",
                pSelf->Server()->ClientName(pResult->m_ClientId));
        pSelf->SendChatTarget(pPlayerTarget->GetCid(), aBuf);
}

void CGameContext::ConTPSpec(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 2)
        {
            pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
            return;
        }

        pPlayer->m_IsTPSpec = !pPlayer->m_IsTPSpec;
        pSelf->SendChatTarget(pResult->m_ClientId, (pPlayer->m_IsTPSpec == true) ? "Вы включили телепорт в спеке" : "Вы выключили телепорт в спеке");
}

void CGameContext::ConStats(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 3)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (str_length(pResult->GetString(0)) == 0)
        {
                pSelf->Help(pResult, pUserData, "stats");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
            "Статистика %s:\n"
            "IP: %s\n"
            "Уровень админки: %i\n"
            "Уровень: %i\n"
            "XP: %i\n"
            "Пойнты: %i\n"
            "Донат рубли: %i",
            TargetPlayerName, pSelf->Server()->ClientAddrString(TargetClientId, false), pPlayerTarget->m_AdminLevel, pPlayerTarget->m_Level,
            pPlayerTarget->m_XP, pPlayerTarget->m_Points, pPlayerTarget->m_DonateRubles);
        pSelf->SendMotd(aBuf, pResult->m_ClientId);
}

void CGameContext::ConBroadcastC(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 3)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (str_length(pResult->GetString(0)) == 0)
        {
                pSelf->Help(pResult, pUserData, "broadcast");
                return;
        }

        pSelf->SendBroadcast(pResult->GetString(0), -1);
}

void CGameContext::ConMotdC(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 3)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (str_length(pResult->GetString(0)) == 0)
        {
                pSelf->Help(pResult, pUserData, "motd");
                return;
        }

        pSelf->SendMotd(pResult->GetString(0), -1);
}

void CGameContext::ConFakeMsg(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 3)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (str_length(pResult->GetString(1)) == 0)
        {
                pSelf->Help(pResult, pUserData, "fakemsg");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

        const char* aMessage = pResult->GetString(1);
	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        pSelf->SendChat(TargetClientId, TEAM_ALL, aMessage);
}

void CGameContext::ConKickC(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 4)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (str_length(pResult->GetString(1)) == 0)
        {
                pSelf->Help(pResult, pUserData, "kick");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	if(TargetClientId == pResult->m_ClientId)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Вы не можете применить это действие к себе");
		return;
        }

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        if (pPlayerTarget->m_AdminLevel >= pPlayer->m_AdminLevel)
        {
                pSelf->Server()->Kick(pResult->m_ClientId, "Попытка кикнуть одинакого уровеня админа или выше стоящего");
                return;
        }

        const char* Reason = pResult->GetString(1);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s кикнул %s Причина: %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName, Reason);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
        pSelf->Server()->Kick(TargetClientId, Reason);
}

void CGameContext::ConMuteC(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 4)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (str_length(pResult->GetString(2)) == 0)
        {
                pSelf->Help(pResult, pUserData, "mute");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	if(TargetClientId == pResult->m_ClientId)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Вы не можете применить это действие к себе");
		return;
        }

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        if (pPlayerTarget->m_AdminLevel >= pPlayer->m_AdminLevel)
        {
                pSelf->Server()->Kick(pResult->m_ClientId, "Попытка замутить одинакого уровеня админа или выше стоящего");
                return;
        }

        const NETADDR *pAddr = pSelf->Server()->ClientAddr(TargetClientId);
        if(!pAddr)
                return;

        int Seconds = pResult->GetInteger(1);
        const char* Reason = pResult->GetString(2);
        pSelf->m_Mutes.Mute(pAddr, Seconds, Reason, TargetPlayerName, false);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s замутил %s на %i секунд Причина: %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName, Seconds, Reason);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConUnMuteC(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 4)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (str_length(pResult->GetString(1)) == 0)
        {
                pSelf->Help(pResult, pUserData, "unmute");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        const NETADDR *pAddr = pSelf->Server()->ClientAddr(TargetClientId);
        if(!pAddr)
                return;

        const char* Reason = pResult->GetString(1);
        pSelf->m_Mutes.UnmuteAddr(pAddr);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s размутил %s Причина: %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName, Reason);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConBanC(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 4)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (str_length(pResult->GetString(2)) == 0)
        {
                pSelf->Help(pResult, pUserData, "ban");
                return;
        }

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	if(TargetClientId == pResult->m_ClientId)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Вы не можете применить это действие к себе");
		return;
        }

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        CCharacter *pChrTarget = pPlayerTarget->GetCharacter();
        if (!pChrTarget)
                return;

        int Minutes = pResult->GetInteger(1) * 60;
        const char* Reason = pResult->GetString(2);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s заблокировал %s на %i минут Причина: %s",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName, pResult->GetInteger(1), Reason);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
        pSelf->Server()->Ban(pPlayerTarget->GetCid(), Minutes, Reason, false);
}

void CGameContext::ConSetTempAdminLevel(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 5)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() != 2)
        {
                pSelf->Help(pResult, pUserData, "settempadminlevel");
                return;
        }

        int AdminLevel = pResult->GetInteger(1);
        if (AdminLevel < 0 || AdminLevel > 255)
                return;

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        if (!pPlayerTarget->m_IsRegistered)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Игрок не зарегистрирован");
                return;
        }

        pPlayerTarget->m_AdminLevel = AdminLevel;
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s установил %s временный %i уровень админки",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName, AdminLevel);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
        pSelf->SendChatTarget(TargetClientId, "Используйте /acmdlist что-бы посмотреть ваши команды");
}

void CGameContext::ConSetAdminLevel(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 5)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() != 2)
        {
                pSelf->Help(pResult, pUserData, "setadminlevel");
                return;
        }

        int AdminLevel = pResult->GetInteger(1);
        if (AdminLevel < 0 || AdminLevel > 255)
                return;

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        if (!pPlayerTarget->m_IsRegistered)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Игрок не зарегистрирован");
                return;
        }

        pPlayerTarget->m_AdminLevel = AdminLevel;
        pSelf->Score()->ChangeAdminLevel(TargetPlayerName, pPlayerTarget->m_AdminLevel);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s установил %s постоянный %i уровень админки",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName, AdminLevel);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
        pSelf->SendChatTarget(TargetClientId, "Используйте /acmdlist что-бы посмотреть ваши команды");
}

void CGameContext::ConSetLevel(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 5)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() != 2)
        {
                pSelf->Help(pResult, pUserData, "setlevel");
                return;
        }

        int Level = pResult->GetInteger(1);
        if (Level < 0)
                return;

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        if (!pPlayerTarget->m_IsRegistered)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Игрок не зарегистрирован");
                return;
        }

        pPlayerTarget->m_Level = Level;
        pSelf->Score()->ChangeLevel(TargetPlayerName, pPlayerTarget->m_Level);
        pPlayerTarget->m_XP = 0;
        pSelf->Score()->ChangeXP(TargetPlayerName, 0);
        pPlayerTarget->m_TargetXP = pPlayerTarget->m_TargetStageXP * pPlayerTarget->m_Level;
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s установил %s %i уровень",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName, Level);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConSetPoints(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 5)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() != 2)
        {
                pSelf->Help(pResult, pUserData, "setpoints");
                return;
        }

        int Points = pResult->GetInteger(1);
        if (Points < 0)
                return;

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        if (!pPlayerTarget->m_IsRegistered)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Игрок не зарегистрирован");
                return;
        }

        pPlayerTarget->m_Points = Points;
        pSelf->Score()->ChangePoints(TargetPlayerName, pPlayerTarget->m_Points);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s установил %s %i пойтов",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName, Points);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConSetDonatRubles(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

        if (pPlayer->m_AdminLevel < 5)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "У вас недостаточно прав");
                return;
        }

        if (pResult->NumArguments() != 2)
        {
                pSelf->Help(pResult, pUserData, "setdonaterubles");
                return;
        }

        int DonatRubles = pResult->GetInteger(1);
        if (DonatRubles < 0)
                return;

        const char* TargetPlayerName = pResult->GetString(0);
	int TargetClientId = -1;
        for(int i = 0; i < MAX_CLIENTS; i++)
        {
                if(pSelf->m_apPlayers[i] && !str_comp(TargetPlayerName, pSelf->Server()->ClientName(i)))
                {
                        TargetClientId = i;
                        break;
                }
        }

	if(TargetClientId < 0)
	{
                char aBuf[255];
                str_format(aBuf, sizeof(aBuf),
                        "Игрок %s не найден",
                        TargetPlayerName);
                pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		return;
	}

	CPlayer *pPlayerTarget = pSelf->m_apPlayers[TargetClientId];
	if(!pPlayerTarget)
		return;

        if (!pPlayerTarget->m_IsRegistered)
        {
                pSelf->SendChatTarget(pResult->m_ClientId, "Игрок не зарегистрирован");
                return;
        }

        pPlayerTarget->m_DonateRubles = DonatRubles;
        pSelf->Score()->ChangeDonateRubles(TargetPlayerName, pPlayerTarget->m_DonateRubles);
        char aBuf[255];
        str_format(aBuf, sizeof(aBuf),
                "Администратор %s установил %s %i донат рублей",
                pSelf->Server()->ClientName(pResult->m_ClientId), TargetPlayerName, DonatRubles);
        pSelf->SendChat(-1, TEAM_ALL, aBuf);
}

void CGameContext::ConList(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	int ClientId = pResult->m_ClientId;
	if(!CheckClientId(ClientId))
		return;

	if(pResult->NumArguments() > 0)
		pSelf->List(ClientId, pResult->GetString(0));
	else
		pSelf->List(ClientId, "");
}

void CGameContext::Help(IConsole::IResult *pResult, void *pUserData, const char *pCommand)
{
	CGameContext *pSelf = (CGameContext *)pUserData;

        const IConsole::ICommandInfo *pCmdInfo =
                pSelf->Console()->GetCommandInfo(pCommand, CFGFLAG_SERVER | CFGFLAG_CHAT, false);
        if (!pCmdInfo)
                return;

        char aBuf[1024];
        str_format(aBuf, sizeof(aBuf),
            "Справка по команде:\n"
            "Команда: /%s\n"
            "Аргументы: %s\n"
            "Описание: %s",
            pCommand, pCmdInfo->Params(), pCmdInfo->Help());
        pSelf->SendMotd(aBuf, pResult->m_ClientId);
}

void CGameContext::ConSettings(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;

	if(pResult->NumArguments() == 0)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"to check a server setting say /settings and setting's name, setting names are:");
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"teams, cheats, collision, hooking, endlesshooking,");
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"hitting, oldlaser, timeout, votes, pause and scores");
	}
	else
	{
		const char *pArg = pResult->GetString(0);
		char aBuf[256];
		float ColTemp;
		float HookTemp;
		pSelf->GlobalTuning()->Get("player_collision", &ColTemp);
		pSelf->GlobalTuning()->Get("player_hooking", &HookTemp);
		if(str_comp_nocase(pArg, "teams") == 0)
		{
			str_format(aBuf, sizeof(aBuf), "%s %s",
				g_Config.m_SvTeam == SV_TEAM_ALLOWED ?
					"Teams are available on this server" :
				(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO) ?
					"Teams are not available on this server" :
					"You have to be in a team to play on this server", /*g_Config.m_SvTeamStrict ? "and if you die in a team all of you die" : */
				"and all of your team will die if the team is locked");
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
		}
		else if(str_comp_nocase(pArg, "cheats") == 0)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				g_Config.m_SvTestingCommands ?
					"Cheats are enabled on this server" :
					"Cheats are disabled on this server");
		}
		else if(str_comp_nocase(pArg, "collision") == 0)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				ColTemp ?
					"Players can collide on this server" :
					"Players can't collide on this server");
		}
		else if(str_comp_nocase(pArg, "hooking") == 0)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				HookTemp ?
					"Players can hook each other on this server" :
					"Players can't hook each other on this server");
		}
		else if(str_comp_nocase(pArg, "endlesshooking") == 0)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				g_Config.m_SvEndlessDrag ?
					"Players hook time is unlimited" :
					"Players hook time is limited");
		}
		else if(str_comp_nocase(pArg, "hitting") == 0)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				g_Config.m_SvHit ?
					"Players weapons affect others" :
					"Players weapons has no affect on others");
		}
		else if(str_comp_nocase(pArg, "oldlaser") == 0)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				g_Config.m_SvOldLaser ?
					"Lasers can hit you if you shot them and they pull you towards the bounce origin (Like DDRace Beta)" :
					"Lasers can't hit you if you shot them, and they pull others towards the shooter");
		}
		else if(str_comp_nocase(pArg, "timeout") == 0)
		{
			str_format(aBuf, sizeof(aBuf), "The Server Timeout is currently set to %d seconds", g_Config.m_ConnTimeout);
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
		}
		else if(str_comp_nocase(pArg, "votes") == 0)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				g_Config.m_SvVoteKick ?
					"Players can use Callvote menu tab to kick offenders" :
					"Players can't use the Callvote menu tab to kick offenders");
			if(g_Config.m_SvVoteKick)
			{
				str_format(aBuf, sizeof(aBuf),
					"Players are banned for %d minute(s) if they get voted off", g_Config.m_SvVoteKickBantime);

				pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
					g_Config.m_SvVoteKickBantime ?
						aBuf :
						"Players are just kicked and not banned if they get voted off");
			}
		}
		else if(str_comp_nocase(pArg, "pause") == 0)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				g_Config.m_SvPauseable ?
					"/spec will pause you and your tee will vanish" :
					"/spec will pause you but your tee will not vanish");
		}
		else if(str_comp_nocase(pArg, "scores") == 0)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				g_Config.m_SvHideScore ?
					"Scores are private on this server" :
					"Scores are public on this server");
		}
		else
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				"no matching settings found, type /settings to view them");
		}
	}
}

static void ToggleSpecPause(IConsole::IResult *pResult, void *pUserData, int PauseType)
{
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CGameContext *pSelf = (CGameContext *)pUserData;
	IServer *pServ = pSelf->Server();
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	int PauseState = pPlayer->IsPaused();
	if(PauseState > 0)
	{
		char aBuf[128];
		str_format(aBuf, sizeof(aBuf), "You are force-paused for %d seconds.", (PauseState - pServ->Tick()) / pServ->TickSpeed());
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
	}
	else if(pResult->NumArguments() > 0)
	{
		if(-PauseState == PauseType && pPlayer->SpectatorId() != pResult->m_ClientId && pServ->ClientIngame(pPlayer->SpectatorId()) && !str_comp(pServ->ClientName(pPlayer->SpectatorId()), pResult->GetString(0)))
		{
			pPlayer->Pause(CPlayer::PAUSE_NONE, false);
		}
		else
		{
			pPlayer->Pause(PauseType, false);
			pPlayer->SpectatePlayerName(pResult->GetString(0));
		}
	}
	else if(-PauseState != CPlayer::PAUSE_NONE && PauseType != CPlayer::PAUSE_NONE)
	{
		pPlayer->Pause(CPlayer::PAUSE_NONE, false);
	}
	else if(-PauseState != PauseType)
	{
		pPlayer->Pause(PauseType, false);
	}
}

static void ToggleSpecPauseVoted(IConsole::IResult *pResult, void *pUserData, int PauseType)
{
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CGameContext *pSelf = (CGameContext *)pUserData;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	int PauseState = pPlayer->IsPaused();
	if(PauseState > 0)
	{
		IServer *pServ = pSelf->Server();
		char aBuf[128];
		str_format(aBuf, sizeof(aBuf), "You are force-paused for %d seconds.", (PauseState - pServ->Tick()) / pServ->TickSpeed());
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
		return;
	}

	bool IsPlayerBeingVoted = pSelf->m_VoteCloseTime &&
				  (pSelf->IsKickVote() || pSelf->IsSpecVote()) &&
				  pResult->m_ClientId != pSelf->m_VoteVictim;
	if((!IsPlayerBeingVoted && -PauseState == PauseType) ||
		(IsPlayerBeingVoted && PauseState && pPlayer->SpectatorId() == pSelf->m_VoteVictim))
	{
		pPlayer->Pause(CPlayer::PAUSE_NONE, false);
	}
	else
	{
		pPlayer->Pause(PauseType, false);
		if(IsPlayerBeingVoted)
			pPlayer->SetSpectatorId(pSelf->m_VoteVictim);
	}
}

void CGameContext::ConToggleSpec(IConsole::IResult *pResult, void *pUserData)
{
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CGameContext *pSelf = (CGameContext *)pUserData;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	int PauseType = g_Config.m_SvPauseable ? CPlayer::PAUSE_SPEC : CPlayer::PAUSE_PAUSED;

	if(pPlayer->GetCharacter())
	{
		CGameTeams &Teams = pSelf->m_pController->Teams();
		if(Teams.IsPractice(Teams.m_Core.Team(pResult->m_ClientId)))
			PauseType = CPlayer::PAUSE_SPEC;
	}


        if (pPlayer->m_IsTPSpec == true) {
            CCharacter *pChr = pPlayer->GetCharacter();
            if(!pChr) {
                return;
            }

            vec2 TargetPos = vec2(pPlayer->m_pLastTarget->m_TargetX, pPlayer->m_pLastTarget->m_TargetY);
            pSelf->Teleport(pChr, TargetPos);
            pSelf->CreatePlayerSpawn(TargetPos);
            pChr->ResetJumps();
            pChr->UnFreeze();
            pChr->ResetVelocity();
        }

	ToggleSpecPause(pResult, pUserData, PauseType);
}

void CGameContext::ConToggleSpecVoted(IConsole::IResult *pResult, void *pUserData)
{
	ToggleSpecPauseVoted(pResult, pUserData, g_Config.m_SvPauseable ? CPlayer::PAUSE_SPEC : CPlayer::PAUSE_PAUSED);
}

void CGameContext::ConTogglePause(IConsole::IResult *pResult, void *pUserData)
{
	ToggleSpecPause(pResult, pUserData, CPlayer::PAUSE_PAUSED);
}

void CGameContext::ConTogglePauseVoted(IConsole::IResult *pResult, void *pUserData)
{
	ToggleSpecPauseVoted(pResult, pUserData, CPlayer::PAUSE_PAUSED);
}

void CGameContext::ConTeamTop5(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvHideScore)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Showing the team top 5 is not allowed on this server.");
		return;
	}

	if(pResult->NumArguments() == 0)
	{
		pSelf->Score()->ShowTeamTop5(pResult->m_ClientId, 1);
	}
	else if(pResult->NumArguments() == 1)
	{
		if(pResult->GetInteger(0) != 0)
		{
			pSelf->Score()->ShowTeamTop5(pResult->m_ClientId, pResult->GetInteger(0));
		}
		else
		{
			const char *pRequestedName = (str_comp_nocase(pResult->GetString(0), "me") == 0) ?
							     pSelf->Server()->ClientName(pResult->m_ClientId) :
							     pResult->GetString(0);
			pSelf->Score()->ShowPlayerTeamTop5(pResult->m_ClientId, pRequestedName, 0);
		}
	}
	else if(pResult->NumArguments() == 2 && pResult->GetInteger(1) != 0)
	{
		const char *pRequestedName = (str_comp_nocase(pResult->GetString(0), "me") == 0) ?
						     pSelf->Server()->ClientName(pResult->m_ClientId) :
						     pResult->GetString(0);
		pSelf->Score()->ShowPlayerTeamTop5(pResult->m_ClientId, pRequestedName, pResult->GetInteger(1));
	}
	else
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "/top5team needs 0, 1 or 2 parameter. 1. = name, 2. = start number");
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Example: /top5team, /top5team me, /top5team Hans, /top5team \"Papa Smurf\" 5");
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Bad: /top5team Papa Smurf 5 # Good: /top5team \"Papa Smurf\" 5 ");
	}
}

void CGameContext::ConTop(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvHideScore)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Showing the top is not allowed on this server.");
		return;
	}

	if(pResult->NumArguments() > 0)
		pSelf->Score()->ShowTop(pResult->m_ClientId, pResult->GetInteger(0));
	else
		pSelf->Score()->ShowTop(pResult->m_ClientId);
}

void CGameContext::ConTimes(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	int Offset = 1;
	const char *pRequestedName = nullptr;

	// input validation
	if(pResult->NumArguments() == 1)
	{
		if(pResult->GetInteger(0) != 0)
		{
			Offset = pResult->GetInteger(0);
		}
		else
		{
			pRequestedName = pResult->GetString(0);
		}
	}
	else if(pResult->NumArguments() == 2)
	{
		pRequestedName = pResult->GetString(0);
		Offset = pResult->GetInteger(1);
	}
	else if(pResult->NumArguments() > 2)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "/times needs 0, 1 or 2 parameter. 1. = name, 2. = start number");
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Example: /times, /times me, /times Hans, /times \"Papa Smurf\" 5");
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Bad: /times Papa Smurf 5 # Good: /times \"Papa Smurf\" 5 ");
		return;
	}

	// execution
	if(g_Config.m_SvHideScore)
	{
		if(pRequestedName && str_comp_nocase(pRequestedName, "me") != 0 && str_comp_nocase(pRequestedName, pSelf->Server()->ClientName(pResult->m_ClientId)) != 0)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Showing the times of others is not allowed on this server.");
			return;
		}
		pRequestedName = pSelf->Server()->ClientName(pResult->m_ClientId);
		pSelf->Score()->ShowTimes(pResult->m_ClientId, pRequestedName, Offset);
	}
	else if(!pRequestedName)
	{
		pSelf->Score()->ShowTimes(pResult->m_ClientId, Offset);
	}
	else
	{
		if(str_comp_nocase(pRequestedName, "me") == 0)
			pRequestedName = pSelf->Server()->ClientName(pResult->m_ClientId);
		pSelf->Score()->ShowTimes(pResult->m_ClientId, pRequestedName, Offset);
	}
}

void CGameContext::ConDND(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	pPlayer->m_DND = pResult->NumArguments() == 0 ? !pPlayer->m_DND : pResult->GetInteger(0);
	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", pPlayer->m_DND ? "You will not receive any further global chat and server messages" : "You will receive global chat and server messages");
}

void CGameContext::ConWhispers(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	pPlayer->m_Whispers = pResult->NumArguments() == 0 ? !pPlayer->m_Whispers : pResult->GetInteger(0);
	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", pPlayer->m_Whispers ? "You will receive whispers" : "You will not receive any further whispers");
}

void CGameContext::ConMap(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvMapVote == 0)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"/map is disabled");
		return;
	}

	if(pResult->NumArguments() <= 0)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Example: /map adr3 to call vote for Adrenaline 3. This means that the map name must start with 'a' and contain the characters 'd', 'r' and '3' in that order");
		return;
	}

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(pSelf->RateLimitPlayerVote(pResult->m_ClientId) || pSelf->RateLimitPlayerMapVote(pResult->m_ClientId))
		return;

	pSelf->Score()->MapVote(pResult->m_ClientId, pResult->GetString(0));
}

void CGameContext::ConMapInfo(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(pResult->NumArguments() > 0)
		pSelf->Score()->MapInfo(pResult->m_ClientId, pResult->GetString(0));
	else
		pSelf->Score()->MapInfo(pResult->m_ClientId, pSelf->Server()->GetMapName());
}

void CGameContext::ConTimeout(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	const char *pTimeout = pResult->NumArguments() > 0 ? pResult->GetString(0) : pPlayer->m_aTimeoutCode;

	if(!pSelf->Server()->IsSixup(pResult->m_ClientId))
	{
		for(int i = 0; i < pSelf->Server()->MaxClients(); i++)
		{
			if(i == pResult->m_ClientId)
				continue;
			if(!pSelf->m_apPlayers[i])
				continue;
			if(str_comp(pSelf->m_apPlayers[i]->m_aTimeoutCode, pTimeout))
				continue;
			if(pSelf->Server()->SetTimedOut(i, pResult->m_ClientId))
			{
				if(pSelf->m_apPlayers[i]->GetCharacter())
					pSelf->SendTuningParams(i, pSelf->m_apPlayers[i]->GetCharacter()->m_TuneZone);
				return;
			}
		}
	}
	else
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Your timeout code has been set. 0.7 clients can not reclaim their tees on timeout; however, a 0.6 client can claim your tee ");
	}

	pSelf->Server()->SetTimeoutProtected(pResult->m_ClientId);
	str_copy(pPlayer->m_aTimeoutCode, pResult->GetString(0), sizeof(pPlayer->m_aTimeoutCode));
}

void CGameContext::ConPractice(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(pSelf->ProcessSpamProtection(pResult->m_ClientId, false))
		return;

	if(!g_Config.m_SvPractice)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Practice mode is disabled");
		return;
	}

	CGameTeams &Teams = pSelf->m_pController->Teams();

	int Team = Teams.m_Core.Team(pResult->m_ClientId);

	if(!Teams.IsValidTeamNumber(Team) || (Team == TEAM_FLOCK && g_Config.m_SvTeam != SV_TEAM_FORCED_SOLO))
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Join a team to enable practice mode, which means you can use /r, but can't earn a rank.");
		return;
	}

	if(Teams.TeamFlock(Team))
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Practice mode can't be enabled in team 0 mode.");
		return;
	}

	if(Teams.GetSaving(Team))
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Practice mode can't be enabled while team save or load is in progress");
		return;
	}

	if(Teams.IsPractice(Team))
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Team is already in practice mode");
		return;
	}

	bool VotedForPractice = pResult->NumArguments() == 0 || pResult->GetInteger(0);

	if(VotedForPractice == pPlayer->m_VotedForPractice)
		return;

	pPlayer->m_VotedForPractice = VotedForPractice;

	int NumCurrentVotes = 0;
	int TeamSize = 0;

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(Teams.m_Core.Team(i) == Team)
		{
			CPlayer *pPlayer2 = pSelf->m_apPlayers[i];
			if(pPlayer2 && pPlayer2->m_VotedForPractice)
				NumCurrentVotes++;
			TeamSize++;
		}
	}

	int NumRequiredVotes = TeamSize / 2 + 1;

	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "'%s' voted to %s /practice mode for your team, which means you can use practice commands, but you can't earn a rank. Type /practice to vote (%d/%d required votes)", pSelf->Server()->ClientName(pResult->m_ClientId), VotedForPractice ? "enable" : "disable", NumCurrentVotes, NumRequiredVotes);
	pSelf->SendChatTeam(Team, aBuf);

	if(NumCurrentVotes >= NumRequiredVotes)
	{
		Teams.SetPractice(Team, true);
		pSelf->SendChatTeam(Team, "Practice mode enabled for your team, happy practicing!");
		pSelf->SendChatTeam(Team, "See /practicecmdlist for a list of all available practice commands. Most commonly used ones are /telecursor, /lasttp and /rescue");
	}
}

void CGameContext::ConUnPractice(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(pSelf->ProcessSpamProtection(pResult->m_ClientId, false))
		return;

	CGameTeams &Teams = pSelf->m_pController->Teams();

	int Team = Teams.m_Core.Team(pResult->m_ClientId);

	if(g_Config.m_SvTeam != SV_TEAM_FORCED_SOLO && Team == TEAM_FLOCK)
	{
		log_info("chatresp", "Practice mode can't be disabled for team 0");
		return;
	}

	if(!Teams.IsPractice(Team))
	{
		log_info("chatresp", "Team isn't in practice mode");
		return;
	}

	if(Teams.GetSaving(Team))
	{
		log_info("chatresp", "Practice mode can't be disabled while team save or load is in progress");
		return;
	}

	if(Teams.Count(Team) > g_Config.m_SvMaxTeamSize && pSelf->m_pController->Teams().TeamLocked(Team))
	{
		log_info("chatresp", "Can't disable practice. This team exceeds the maximum allowed size of %d players for regular team", g_Config.m_SvMaxTeamSize);
		return;
	}

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(Teams.m_Core.Team(i) == Team)
		{
			CPlayer *pPlayer2 = pSelf->m_apPlayers[i];
			if(pPlayer2)
			{
				if(pPlayer2->m_VotedForPractice)
					pPlayer2->m_VotedForPractice = false;

				if(!g_Config.m_SvPauseable && pPlayer2->IsPaused() == -1 * CPlayer::PAUSE_SPEC)
					pPlayer2->Pause(CPlayer::PAUSE_PAUSED, true);
			}
		}
	}

	// send before kill, in case team isn't locked
	char aBuf[256];
	str_format(aBuf, sizeof(aBuf), "'%s' disabled practice mode for your team", pSelf->Server()->ClientName(pResult->m_ClientId));
	pSelf->SendChatTeam(Team, aBuf);

	Teams.KillCharacterOrTeam(pResult->m_ClientId, Team);
	Teams.SetPractice(Team, false);
}

void CGameContext::ConPracticeCmdList(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;

	char aPracticeCommands[256] = "Available practice commands: ";
	for(const IConsole::ICommandInfo *pCmd = pSelf->Console()->FirstCommandInfo(pResult->m_ClientId, CMDFLAG_PRACTICE);
		pCmd; pCmd = pSelf->Console()->NextCommandInfo(pCmd, pResult->m_ClientId, CMDFLAG_PRACTICE))
	{
		char aCommand[64];

		str_format(aCommand, sizeof(aCommand), "/%s%s", pCmd->Name(), pSelf->Console()->NextCommandInfo(pCmd, pResult->m_ClientId, CMDFLAG_PRACTICE) ? ", " : "");

		if(str_length(aCommand) + str_length(aPracticeCommands) > 255)
		{
			pSelf->SendChatTarget(pResult->m_ClientId, aPracticeCommands);
			aPracticeCommands[0] = '\0';
		}
		str_append(aPracticeCommands, aCommand);
	}
	pSelf->SendChatTarget(pResult->m_ClientId, aPracticeCommands);
}

void CGameContext::ConSwap(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	const char *pName = pResult->GetString(0);

	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(!g_Config.m_SvSwap)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Swap is disabled on this server.");
		return;
	}

	if(g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Swap is not available on forced solo servers.");
		return;
	}

	CGameTeams &Teams = pSelf->m_pController->Teams();

	int Team = Teams.m_Core.Team(pResult->m_ClientId);

	if(!Teams.IsValidTeamNumber(Team))
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"You aren't in a valid team.");
		return;
	}

	int TargetClientId = -1;
	if(pResult->NumArguments() == 1)
	{
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			if(pSelf->m_apPlayers[i] && !str_comp(pName, pSelf->Server()->ClientName(i)))
			{
				TargetClientId = i;
				break;
			}
		}
	}
	else
	{
		int TeamSize = 1;
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			if(pSelf->m_apPlayers[i] && Teams.m_Core.Team(i) == Team && i != pResult->m_ClientId)
			{
				TargetClientId = i;
				TeamSize++;
			}
		}
		if(TeamSize != 2)
			TargetClientId = -1;
	}

	if(TargetClientId < 0)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Player not found");
		return;
	}

	if(TargetClientId == pResult->m_ClientId)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Can't swap with yourself");
		return;
	}

	int TargetTeam = Teams.m_Core.Team(TargetClientId);
	if(TargetTeam != Team)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Player is on a different team");
		return;
	}

	CPlayer *pSwapPlayer = pSelf->m_apPlayers[TargetClientId];
	if(Team == TEAM_FLOCK || Teams.TeamFlock(Team))
	{
		CCharacter *pChr = pPlayer->GetCharacter();
		CCharacter *pSwapChr = pSwapPlayer->GetCharacter();
		if(!pChr || !pSwapChr || pChr->m_DDRaceState != ERaceState::STARTED || pSwapChr->m_DDRaceState != ERaceState::STARTED)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "You and other player need to have started the map");
			return;
		}
	}
	else if(!Teams.IsStarted(Team) && !Teams.TeamFlock(Team))
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Need to have started the map to swap with a player.");
		return;
	}
	if(pSelf->m_World.m_Core.m_apCharacters[pResult->m_ClientId] == nullptr || pSelf->m_World.m_Core.m_apCharacters[TargetClientId] == nullptr)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "You and the other player must not be paused.");
		return;
	}

	bool SwapPending = pSwapPlayer->m_SwapTargetsClientId != pResult->m_ClientId;
	if(SwapPending)
	{
		if(pSelf->ProcessSpamProtection(pResult->m_ClientId))
			return;

		Teams.RequestTeamSwap(pPlayer, pSwapPlayer, Team);
		return;
	}

	Teams.SwapTeamCharacters(pPlayer, pSwapPlayer, Team);
}

void CGameContext::ConCancelSwap(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;

	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(!g_Config.m_SvSwap)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Swap is disabled on this server.");
		return;
	}

	if(g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Swap is not available on forced solo servers.");
		return;
	}

	CGameTeams &Teams = pSelf->m_pController->Teams();

	int Team = Teams.m_Core.Team(pResult->m_ClientId);

	if(!pSelf->m_pController->Teams().IsValidTeamNumber(Team))
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"You aren't in a valid team.");
		return;
	}

	bool SwapPending = pPlayer->m_SwapTargetsClientId != -1 && !pSelf->Server()->ClientSlotEmpty(pPlayer->m_SwapTargetsClientId);

	if(!SwapPending)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"You do not have a pending swap request.");
		return;
	}

	Teams.CancelTeamSwap(pPlayer, Team);
}

void CGameContext::ConSave(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(!g_Config.m_SvSaveGames)
	{
		pSelf->SendChatTarget(pResult->m_ClientId, "Save-function is disabled on this server");
		return;
	}

	const char *pCode = "";
	if(pResult->NumArguments() > 0)
		pCode = pResult->GetString(0);

	pSelf->Score()->SaveTeam(pResult->m_ClientId, pCode, g_Config.m_SvSqlServerName);
}

void CGameContext::ConLoad(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(!g_Config.m_SvSaveGames)
	{
		pSelf->SendChatTarget(pResult->m_ClientId, "Save-function is disabled on this server");
		return;
	}

	if(pResult->NumArguments() > 0)
		pSelf->Score()->LoadTeam(pResult->GetString(0), pResult->m_ClientId);
	else
		pSelf->Score()->GetSaves(pResult->m_ClientId);
}

void CGameContext::ConTeamRank(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(pResult->NumArguments() > 0)
	{
		if(!g_Config.m_SvHideScore)
			pSelf->Score()->ShowTeamRank(pResult->m_ClientId, pResult->GetString(0));
		else
			pSelf->Console()->Print(
				IConsole::OUTPUT_LEVEL_STANDARD,
				"chatresp",
				"Showing the team rank of other players is not allowed on this server.");
	}
	else
		pSelf->Score()->ShowTeamRank(pResult->m_ClientId,
			pSelf->Server()->ClientName(pResult->m_ClientId));
}

void CGameContext::ConRank(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(pResult->NumArguments() > 0)
	{
		if(!g_Config.m_SvHideScore)
			pSelf->Score()->ShowRank(pResult->m_ClientId, pResult->GetString(0));
		else
			pSelf->Console()->Print(
				IConsole::OUTPUT_LEVEL_STANDARD,
				"chatresp",
				"Showing the rank of other players is not allowed on this server.");
	}
	else
		pSelf->Score()->ShowRank(pResult->m_ClientId,
			pSelf->Server()->ClientName(pResult->m_ClientId));
}

void CGameContext::ConLock(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Teams are disabled");
		return;
	}

	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);

	bool Lock = pSelf->m_pController->Teams().TeamLocked(Team);

	if(pResult->NumArguments() > 0)
		Lock = !pResult->GetInteger(0);

	if(Team == TEAM_FLOCK || !pSelf->m_pController->Teams().IsValidTeamNumber(Team))
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"This team can't be locked");
		return;
	}

	if(pSelf->ProcessSpamProtection(pResult->m_ClientId, false))
		return;

	char aBuf[512];
	if(Lock)
	{
		pSelf->UnlockTeam(pResult->m_ClientId, Team);
	}
	else
	{
		pSelf->m_pController->Teams().SetTeamLock(Team, true);

		if(pSelf->m_pController->Teams().TeamFlock(Team))
			str_format(aBuf, sizeof(aBuf), "'%s' locked your team.", pSelf->Server()->ClientName(pResult->m_ClientId));
		else
			str_format(aBuf, sizeof(aBuf), "'%s' locked your team. After the race starts, killing will kill everyone in your team.", pSelf->Server()->ClientName(pResult->m_ClientId));
		pSelf->SendChatTeam(Team, aBuf);
	}
}

void CGameContext::ConUnlock(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Teams are disabled");
		return;
	}

	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);

	if(Team == TEAM_FLOCK || !pSelf->m_pController->Teams().IsValidTeamNumber(Team))
		return;

	if(pSelf->ProcessSpamProtection(pResult->m_ClientId, false))
		return;

	pSelf->UnlockTeam(pResult->m_ClientId, Team);
}

void CGameContext::UnlockTeam(int ClientId, int Team) const
{
	m_pController->Teams().SetTeamLock(Team, false);

	char aBuf[512];
	str_format(aBuf, sizeof(aBuf), "'%s' unlocked your team.", Server()->ClientName(ClientId));
	SendChatTeam(Team, aBuf);
}

void CGameContext::AttemptJoinTeam(int ClientId, int Team)
{
	CPlayer *pPlayer = m_apPlayers[ClientId];
	if(!pPlayer)
		return;

	if(IsRunningKickOrSpecVote(ClientId))
	{
		Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"You are running a vote, please try again after the vote is done!");
		return;
	}
	else if(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO)
	{
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Teams are disabled");
		return;
	}
	else if(g_Config.m_SvTeam == SV_TEAM_MANDATORY && Team == 0 && pPlayer->GetCharacter() && pPlayer->GetCharacter()->m_LastStartWarning < Server()->Tick() - 3 * Server()->TickSpeed())
	{
		Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"You must join a team and play with somebody or else you can't play");
		pPlayer->GetCharacter()->m_LastStartWarning = Server()->Tick();
	}

	if(!m_pController->Teams().IsValidTeamNumber(Team))
	{
		auto EmptyTeam = m_pController->Teams().GetFirstEmptyTeam();
		if(!EmptyTeam.has_value())
		{
			Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
				"No empty team left.");
			return;
		}
		Team = EmptyTeam.value();
	}

	char aError[512];
	if(pPlayer->m_LastDDRaceTeamChange + (int64_t)Server()->TickSpeed() * g_Config.m_SvTeamChangeDelay > Server()->Tick())
	{
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"You can't change teams that fast!");
	}
	else if(Team != TEAM_FLOCK && m_pController->Teams().TeamLocked(Team) && !m_pController->Teams().IsInvited(Team, ClientId))
	{
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			g_Config.m_SvInvite ?
				"This team is locked using /lock. Only members of the team can unlock it using /lock." :
				"This team is locked using /lock. Only members of the team can invite you or unlock it using /lock.");
	}
	else if(Team != TEAM_FLOCK && m_pController->Teams().Count(Team) >= g_Config.m_SvMaxTeamSize && !m_pController->Teams().TeamFlock(Team) && !m_pController->Teams().IsPractice(Team))
	{
		char aBuf[512];
		str_format(aBuf, sizeof(aBuf), "This team already has the maximum allowed size of %d players", g_Config.m_SvMaxTeamSize);
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
	}
	else if(!m_pController->Teams().SetCharacterTeam(pPlayer->GetCid(), Team, aError, sizeof(aError)))
	{
		Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aError);
	}
	else
	{
		if(PracticeByDefault())
		{
			// joined an empty team
			if(m_pController->Teams().Count(Team) == 1)
				m_pController->Teams().SetPractice(Team, true);
		}

		char aBuf[512];
		str_format(aBuf, sizeof(aBuf), "'%s' joined team %d",
			Server()->ClientName(pPlayer->GetCid()),
			Team);
		SendChat(-1, TEAM_ALL, aBuf);
		pPlayer->m_LastDDRaceTeamChange = Server()->Tick();

		if(m_pController->Teams().IsPractice(Team))
			SendChatTarget(pPlayer->GetCid(), "Practice mode enabled for your team, happy practicing!");

		if(m_pController->Teams().TeamFlock(Team))
			SendChatTarget(pPlayer->GetCid(), "Team 0 mode enabled for your team. This will make your team behave like team 0.");
	}
}

void CGameContext::ConInvite(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	auto *pController = pSelf->m_pController;
	const char *pName = pResult->GetString(0);

	if(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Teams are disabled");
		return;
	}

	if(!g_Config.m_SvInvite)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Invites are disabled");
		return;
	}

	int Team = pController->Teams().m_Core.Team(pResult->m_ClientId);
	if(Team != TEAM_FLOCK && pController->Teams().IsValidTeamNumber(Team))
	{
		int Target = -1;
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			if(!str_comp(pName, pSelf->Server()->ClientName(i)))
			{
				Target = i;
				break;
			}
		}

		if(Target < 0)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Player not found");
			return;
		}

		if(pController->Teams().IsInvited(Team, Target))
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Player already invited");
			return;
		}

		if(pSelf->m_apPlayers[pResult->m_ClientId] && pSelf->m_apPlayers[pResult->m_ClientId]->m_LastInvited + g_Config.m_SvInviteFrequency * pSelf->Server()->TickSpeed() > pSelf->Server()->Tick())
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Can't invite this quickly");
			return;
		}

		pController->Teams().SetClientInvited(Team, Target, true);
		pSelf->m_apPlayers[pResult->m_ClientId]->m_LastInvited = pSelf->Server()->Tick();

		char aBuf[512];
		str_format(aBuf, sizeof(aBuf), "'%s' invited you to team %d. Use /team %d to join.", pSelf->Server()->ClientName(pResult->m_ClientId), Team, Team);
		pSelf->SendChatTarget(Target, aBuf);

		str_format(aBuf, sizeof(aBuf), "'%s' invited '%s' to your team.", pSelf->Server()->ClientName(pResult->m_ClientId), pSelf->Server()->ClientName(Target));
		pSelf->SendChatTeam(Team, aBuf);
	}
	else
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Can't invite players to this team");
}

void CGameContext::ConTeam0Mode(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	auto *pController = pSelf->m_pController;

	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvTeam == SV_TEAM_FORBIDDEN || g_Config.m_SvTeam == SV_TEAM_FORCED_SOLO || g_Config.m_SvTeam == SV_TEAM_MANDATORY)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Team mode change disabled");
		return;
	}

	if(!g_Config.m_SvTeam0Mode)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Team mode change is disabled on this server.");
		return;
	}

	int Team = pController->Teams().m_Core.Team(pResult->m_ClientId);
	bool Mode = pController->Teams().TeamFlock(Team);

	if(Team == TEAM_FLOCK || !pController->Teams().IsValidTeamNumber(Team))
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"This team can't have the mode changed");
		return;
	}

	if(pController->Teams().GetTeamState(Team) != ETeamState::OPEN)
	{
		pSelf->SendChatTarget(pResult->m_ClientId, "Team mode can't be changed while racing");
		return;
	}

	if(pResult->NumArguments() > 0)
		Mode = !pResult->GetInteger(0);

	if(pSelf->ProcessSpamProtection(pResult->m_ClientId, false))
		return;

	char aBuf[512];
	if(Mode)
	{
		if(pController->Teams().Count(Team) > g_Config.m_SvMaxTeamSize)
		{
			str_format(aBuf, sizeof(aBuf), "Can't disable team 0 mode. This team exceeds the maximum allowed size of %d players for regular team", g_Config.m_SvMaxTeamSize);
			pSelf->SendChatTarget(pResult->m_ClientId, aBuf);
		}
		else
		{
			pController->Teams().SetTeamFlock(Team, false);

			str_format(aBuf, sizeof(aBuf), "'%s' disabled team 0 mode.", pSelf->Server()->ClientName(pResult->m_ClientId));
			pSelf->SendChatTeam(Team, aBuf);
		}
	}
	else
	{
		if(pController->Teams().IsPractice(Team))
		{
			pSelf->SendChatTarget(pResult->m_ClientId, "Can't enable team 0 mode with practice mode on.");
		}
		else
		{
			pController->Teams().SetTeamFlock(Team, true);

			str_format(aBuf, sizeof(aBuf), "'%s' enabled team 0 mode. This will make your team behave like team 0.", pSelf->Server()->ClientName(pResult->m_ClientId));
			pSelf->SendChatTeam(Team, aBuf);
		}
	}
}

void CGameContext::ConTeam(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(pResult->NumArguments() > 0)
	{
		pSelf->AttemptJoinTeam(pResult->m_ClientId, pResult->GetInteger(0));
	}
	else
	{
		char aBuf[512];
		if(!pPlayer->IsPlaying())
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "You can't check your team while you are dead/a spectator.");
		}
		else
		{
			int TeamSize = 0;
			const int PlayerTeam = pSelf->GetDDRaceTeam(pResult->m_ClientId);

			// Count players in team
			for(int ClientId = 0; ClientId < MAX_CLIENTS; ClientId++)
			{
				const CPlayer *pOtherPlayer = pSelf->m_apPlayers[ClientId];
				if(!pOtherPlayer || !pOtherPlayer->IsPlaying())
					continue;

				if(pSelf->GetDDRaceTeam(ClientId) == PlayerTeam)
					TeamSize++;
			}

			str_format(aBuf, sizeof(aBuf), "You are in team %d having %d %s", PlayerTeam, TeamSize, TeamSize > 1 ? "players" : "player");
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
		}
	}
}

void CGameContext::ConJoin(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	int Target = -1;
	const char *pName = pResult->GetString(0);
	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		if(!str_comp(pName, pSelf->Server()->ClientName(i)))
		{
			Target = i;
			break;
		}
	}

	if(Target == -1)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Player not found");
		return;
	}

	int Team = pSelf->GetDDRaceTeam(Target);
	if(pSelf->ProcessSpamProtection(pResult->m_ClientId, false))
		return;

	pSelf->AttemptJoinTeam(pResult->m_ClientId, Team);
}

void CGameContext::ConConverse(IConsole::IResult *pResult, void *pUserData)
{
	// This will never be called
}

void CGameContext::ConWhisper(IConsole::IResult *pResult, void *pUserData)
{
	// This will never be called
}

void CGameContext::ConSetEyeEmote(IConsole::IResult *pResult,
	void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	if(pResult->NumArguments() == 0)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			(pPlayer->m_EyeEmoteEnabled) ?
				"You can now use the preset eye emotes." :
				"You don't have any eye emotes, remember to bind some.");
		return;
	}
	else if(str_comp_nocase(pResult->GetString(0), "on") == 0)
		pPlayer->m_EyeEmoteEnabled = true;
	else if(str_comp_nocase(pResult->GetString(0), "off") == 0)
		pPlayer->m_EyeEmoteEnabled = false;
	else if(str_comp_nocase(pResult->GetString(0), "toggle") == 0)
		pPlayer->m_EyeEmoteEnabled = !pPlayer->m_EyeEmoteEnabled;
	pSelf->Console()->Print(
		IConsole::OUTPUT_LEVEL_STANDARD,
		"chatresp",
		(pPlayer->m_EyeEmoteEnabled) ?
			"You can now use the preset eye emotes." :
			"You don't have any eye emotes, remember to bind some.");
}

void CGameContext::ConEyeEmote(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(g_Config.m_SvEmotionalTees == -1)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Emotes are disabled.");
		return;
	}

	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(pResult->NumArguments() == 0)
	{
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Emote commands are: /emote surprise /emote blink /emote close /emote angry /emote happy /emote pain /emote normal");
		pSelf->Console()->Print(
			IConsole::OUTPUT_LEVEL_STANDARD,
			"chatresp",
			"Example: /emote surprise 10 for 10 seconds or /emote surprise (default 1 second)");
	}
	else
	{
		if(!pPlayer->CanOverrideDefaultEmote())
			return;

		int EmoteType = 0;
		if(!str_comp_nocase(pResult->GetString(0), "angry"))
			EmoteType = EMOTE_ANGRY;
		else if(!str_comp_nocase(pResult->GetString(0), "blink"))
			EmoteType = EMOTE_BLINK;
		else if(!str_comp_nocase(pResult->GetString(0), "close"))
			EmoteType = EMOTE_BLINK;
		else if(!str_comp_nocase(pResult->GetString(0), "happy"))
			EmoteType = EMOTE_HAPPY;
		else if(!str_comp_nocase(pResult->GetString(0), "pain"))
			EmoteType = EMOTE_PAIN;
		else if(!str_comp_nocase(pResult->GetString(0), "surprise"))
			EmoteType = EMOTE_SURPRISE;
		else if(!str_comp_nocase(pResult->GetString(0), "normal"))
			EmoteType = EMOTE_NORMAL;
		else
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD,
				"chatresp", "Unknown emote... Say /emote");
			return;
		}

		int Duration = 1;
		if(pResult->NumArguments() > 1)
			Duration = std::clamp(pResult->GetInteger(1), 1, 86400);

		pPlayer->OverrideDefaultEmote(EmoteType, pSelf->Server()->Tick() + Duration * pSelf->Server()->TickSpeed());
	}
}

void CGameContext::ConSpecTeam(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	if(pResult->NumArguments())
		pPlayer->m_SpecTeam = pResult->GetInteger(0);
	else
		pPlayer->m_SpecTeam = !pPlayer->m_SpecTeam;
}

void CGameContext::ConSayTime(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	int ClientId;
	char aBufName[MAX_NAME_LENGTH];

	if(pResult->NumArguments() > 0)
	{
		for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId++)
			if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
				break;

		if(ClientId == MAX_CLIENTS)
			return;

		str_format(aBufName, sizeof(aBufName), "%s's", pSelf->Server()->ClientName(ClientId));
	}
	else
	{
		str_copy(aBufName, "Your", sizeof(aBufName));
		ClientId = pResult->m_ClientId;
	}

	CPlayer *pPlayer = pSelf->m_apPlayers[ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;
	if(pChr->m_DDRaceState != ERaceState::STARTED)
		return;

	char aBufTime[32];
	char aBuf[64];
	int64_t Time = (int64_t)100 * (float)(pSelf->Server()->Tick() - pChr->m_StartTime) / ((float)pSelf->Server()->TickSpeed());
	str_time(Time, TIME_HOURS, aBufTime, sizeof(aBufTime));
	str_format(aBuf, sizeof(aBuf), "%s current race time is %s", aBufName, aBufTime);
	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
}

void CGameContext::ConSayTimeAll(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;
	if(pChr->m_DDRaceState != ERaceState::STARTED)
		return;

	char aBufTime[32];
	char aBuf[64];
	int64_t Time = (int64_t)100 * (float)(pSelf->Server()->Tick() - pChr->m_StartTime) / ((float)pSelf->Server()->TickSpeed());
	const char *pName = pSelf->Server()->ClientName(pResult->m_ClientId);
	str_time(Time, TIME_HOURS, aBufTime, sizeof(aBufTime));
	str_format(aBuf, sizeof(aBuf), "%s's current race time is %s", pName, aBufTime);
	pSelf->SendChat(-1, TEAM_ALL, aBuf, pResult->m_ClientId);
}

void CGameContext::ConTime(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;

	char aBufTime[32];
	char aBuf[64];
	int64_t Time = (int64_t)100 * (float)(pSelf->Server()->Tick() - pChr->m_StartTime) / ((float)pSelf->Server()->TickSpeed());
	str_time(Time, TIME_HOURS, aBufTime, sizeof(aBufTime));
	str_format(aBuf, sizeof(aBuf), "Your time is %s", aBufTime);
	pSelf->SendBroadcast(aBuf, pResult->m_ClientId);
}

static const char s_aaMsg[4][128] = {"game/round timer.", "broadcast.", "both game/round timer and broadcast.", "racetime."};

void CGameContext::ConSetTimerType(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;

	if(!CheckClientId(pResult->m_ClientId))
		return;

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	char aBuf[128];

	if(pResult->NumArguments() > 0)
	{
		int OldType = pPlayer->m_TimerType;
		bool Result = false;

		if(str_comp_nocase(pResult->GetString(0), "default") == 0)
			Result = pPlayer->SetTimerType(CPlayer::TIMERTYPE_DEFAULT);
		else if(str_comp_nocase(pResult->GetString(0), "gametimer") == 0)
			Result = pPlayer->SetTimerType(CPlayer::TIMERTYPE_GAMETIMER);
		else if(str_comp_nocase(pResult->GetString(0), "broadcast") == 0)
			Result = pPlayer->SetTimerType(CPlayer::TIMERTYPE_BROADCAST);
		else if(str_comp_nocase(pResult->GetString(0), "both") == 0)
			Result = pPlayer->SetTimerType(CPlayer::TIMERTYPE_GAMETIMER_AND_BROADCAST);
		else if(str_comp_nocase(pResult->GetString(0), "none") == 0)
			Result = pPlayer->SetTimerType(CPlayer::TIMERTYPE_NONE);
		else
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Unknown parameter. Accepted values: default, gametimer, broadcast, both, none");
			return;
		}

		if(!Result)
		{
			pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", "Selected timertype is not supported by your client");
			return;
		}

		if((OldType == CPlayer::TIMERTYPE_BROADCAST || OldType == CPlayer::TIMERTYPE_GAMETIMER_AND_BROADCAST) && (pPlayer->m_TimerType == CPlayer::TIMERTYPE_GAMETIMER || pPlayer->m_TimerType == CPlayer::TIMERTYPE_NONE))
			pSelf->SendBroadcast("", pResult->m_ClientId);
	}

	if(pPlayer->m_TimerType <= CPlayer::TIMERTYPE_SIXUP && pPlayer->m_TimerType >= CPlayer::TIMERTYPE_GAMETIMER)
		str_format(aBuf, sizeof(aBuf), "Timer is displayed in %s", s_aaMsg[pPlayer->m_TimerType]);
	else if(pPlayer->m_TimerType == CPlayer::TIMERTYPE_NONE)
		str_copy(aBuf, "Timer isn't displayed.");

	pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp", aBuf);
}

void CGameContext::ConRescue(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;

	CGameTeams &Teams = pSelf->m_pController->Teams();
	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);
	if(!g_Config.m_SvRescue && !Teams.IsPractice(Team))
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Rescue is not enabled on this server and you're not in a team with /practice turned on. Note that you can't earn a rank with practice enabled.");
		return;
	}

	bool GoRescue = true;

	if(pPlayer->m_RescueMode == RESCUEMODE_MANUAL)
	{
		// if character can't set his rescue state then we should rescue him instead
		GoRescue = !pChr->TrySetRescue(RESCUEMODE_MANUAL);
	}

	if(GoRescue)
	{
		pChr->Rescue();
		pChr->UnFreeze();
	}
}

void CGameContext::ConRescueMode(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	CGameTeams &Teams = pSelf->m_pController->Teams();
	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);
	if(!g_Config.m_SvRescue && !Teams.IsPractice(Team))
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Rescue is not enabled on this server and you're not in a team with /practice turned on. Note that you can't earn a rank with practice enabled.");
		return;
	}

	if(str_comp_nocase(pResult->GetString(0), "auto") == 0)
	{
		if(pPlayer->m_RescueMode != RESCUEMODE_AUTO)
		{
			pPlayer->m_RescueMode = RESCUEMODE_AUTO;

			pSelf->SendChatTarget(pPlayer->GetCid(), "Rescue mode changed to auto.");
		}

		return;
	}

	if(str_comp_nocase(pResult->GetString(0), "manual") == 0)
	{
		if(pPlayer->m_RescueMode != RESCUEMODE_MANUAL)
		{
			pPlayer->m_RescueMode = RESCUEMODE_MANUAL;

			pSelf->SendChatTarget(pPlayer->GetCid(), "Rescue mode changed to manual.");
		}

		return;
	}

	if(str_comp_nocase(pResult->GetString(0), "list") == 0)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Available rescue modes: auto, manual");
	}
	else if(str_comp_nocase(pResult->GetString(0), "") == 0)
	{
		char aBuf[64];
		str_format(aBuf, sizeof(aBuf), "Current rescue mode: %s.", pPlayer->m_RescueMode == RESCUEMODE_MANUAL ? "manual" : "auto");
		pSelf->SendChatTarget(pPlayer->GetCid(), aBuf);
	}
	else
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "Unknown argument. Check '/rescuemode list'");
	}
}

void CGameContext::ConBack(IConsole::IResult *pResult, void *pUserData)
{
	auto *pSelf = static_cast<CGameContext *>(pUserData);
	if(auto *pChr = pSelf->GetPracticeCharacter(pResult))
	{
		auto *pPlayer = pChr->GetPlayer();
		if(!pPlayer->m_LastDeath.has_value())
		{
			pSelf->SendChatTarget(pPlayer->GetCid(), "There is nowhere to go back to.");
			return;
		}
		pChr->GetLastRescueTeeRef(pPlayer->m_RescueMode) = pPlayer->m_LastDeath.value();
		pChr->Rescue();
		pChr->UnFreeze();
	}
}

void CGameContext::ConTeleTo(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;
	CPlayer *pCallingPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pCallingPlayer)
		return;
	CCharacter *pCallingCharacter = pCallingPlayer->GetCharacter();
	if(!pCallingCharacter)
		return;

	CGameTeams &Teams = pSelf->m_pController->Teams();
	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);
	if(!Teams.IsPractice(Team))
	{
		pSelf->SendChatTarget(pCallingPlayer->GetCid(), "You're not in a team with /practice turned on. Note that you can't earn a rank with practice enabled.");
		return;
	}

	vec2 Pos = {};

	if(pResult->NumArguments() == 0)
	{
		// Set calling tee's position to the origin of its spectating viewport
		Pos = pCallingPlayer->m_ViewPos;
	}
	else
	{
		// Search for player with this name
		int ClientId;
		for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId++)
		{
			if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
				break;
		}
		if(ClientId == MAX_CLIENTS)
		{
			pSelf->SendChatTarget(pCallingPlayer->GetCid(), "No player with this name found.");
			return;
		}

		CPlayer *pDestPlayer = pSelf->m_apPlayers[ClientId];
		if(!pDestPlayer)
			return;
		CCharacter *pDestCharacter = pDestPlayer->GetCharacter();
		if(!pDestCharacter)
			return;

		// Set calling tee's position to that of the destination tee
		Pos = pDestCharacter->m_Pos;
	}

	// Teleport tee
	pSelf->Teleport(pCallingCharacter, Pos);
	pCallingCharacter->ResetJumps();
	pCallingCharacter->UnFreeze();
	pCallingCharacter->ResetVelocity();
	pCallingPlayer->m_LastTeleTee.Save(pCallingCharacter);
}

void CGameContext::ConTeleXY(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;
	CPlayer *pCallingPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pCallingPlayer)
		return;
	CCharacter *pCallingCharacter = pCallingPlayer->GetCharacter();
	if(!pCallingCharacter)
		return;

	CGameTeams &Teams = pSelf->m_pController->Teams();
	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);
	if(!Teams.IsPractice(Team))
	{
		pSelf->SendChatTarget(pCallingPlayer->GetCid(), "You're not in a team with /practice turned on. Note that you can't earn a rank with practice enabled.");
		return;
	}

	vec2 Pos = {};

	if(pResult->NumArguments() == 1)
	{
		pSelf->SendChatTarget(pCallingPlayer->GetCid(), "Can't recognize specified arguments. Usage: /tpxy x y, e.g. /tpxy 9 3.");
		return;
	}
	else
	{
		float BaseX = 0.f, BaseY = 0.f;

		CMapItemLayerTilemap *pGameLayer = pSelf->m_Layers.GameLayer();
		constexpr float OuterKillTileBoundaryDistance = 201 * 32.f;
		float MapWidth = (pGameLayer->m_Width * 32) + (OuterKillTileBoundaryDistance * 2.f), MapHeight = (pGameLayer->m_Height * 32) + (OuterKillTileBoundaryDistance * 2.f);

		const auto DetermineCoordinateRelativity = [](const char *pInString, const float AbsoluteDefaultValue, float &OutFloat) -> bool {
			// mode 0 = abs, 1 = sub, 2 = add

			// Relative?
			const char *pStrDelta = str_startswith(pInString, "~");

			float d;
			if(!str_tofloat(pStrDelta ? pStrDelta : pInString, &d))
				return false;

			// Is the number valid?
			if(std::isnan(d) || std::isinf(d))
				return false;

			// Convert our gleaned 'display' coordinate to an actual map coordinate
			d *= 32.f;

			OutFloat = (pStrDelta ? AbsoluteDefaultValue : 0) + d;
			return true;
		};

		if(!DetermineCoordinateRelativity(pResult->GetString(0), pCallingPlayer->m_ViewPos.x, BaseX))
		{
			pSelf->SendChatTarget(pCallingPlayer->GetCid(), "Invalid X coordinate.");
			return;
		}
		if(!DetermineCoordinateRelativity(pResult->GetString(1), pCallingPlayer->m_ViewPos.y, BaseY))
		{
			pSelf->SendChatTarget(pCallingPlayer->GetCid(), "Invalid Y coordinate.");
			return;
		}

		Pos = {std::clamp(BaseX, (-OuterKillTileBoundaryDistance) + 1.f, (-OuterKillTileBoundaryDistance) + MapWidth - 1.f), std::clamp(BaseY, (-OuterKillTileBoundaryDistance) + 1.f, (-OuterKillTileBoundaryDistance) + MapHeight - 1.f)};
	}

	// Teleport tee
	pSelf->Teleport(pCallingCharacter, Pos);
	pCallingCharacter->ResetJumps();
	pCallingCharacter->UnFreeze();
	pCallingCharacter->ResetVelocity();
	pCallingPlayer->m_LastTeleTee.Save(pCallingCharacter);
}

void CGameContext::ConTeleCursor(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;

	CGameTeams &Teams = pSelf->m_pController->Teams();
	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);
	if(!Teams.IsPractice(Team))
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "You're not in a team with /practice turned on. Note that you can't earn a rank with practice enabled.");
		return;
	}

	// default to view pos when character is not available
	vec2 Pos = pPlayer->m_ViewPos;
	if(pResult->NumArguments() == 0 && !pPlayer->IsPaused() && pPlayer->GetCharacter() && pPlayer->GetCharacter()->IsAlive())
	{
		vec2 Target = vec2(pChr->Core()->m_Input.m_TargetX, pChr->Core()->m_Input.m_TargetY);
		Pos = pPlayer->m_CameraInfo.ConvertTargetToWorld(pPlayer->GetCharacter()->GetPos(), Target);
	}
	else if(pResult->NumArguments() > 0)
	{
		int ClientId;
		for(ClientId = 0; ClientId < MAX_CLIENTS; ClientId++)
		{
			if(str_comp(pResult->GetString(0), pSelf->Server()->ClientName(ClientId)) == 0)
				break;
		}
		if(ClientId == MAX_CLIENTS)
		{
			pSelf->SendChatTarget(pPlayer->GetCid(), "No player with this name found.");
			return;
		}
		CPlayer *pPlayerTo = pSelf->m_apPlayers[ClientId];
		if(!pPlayerTo)
			return;
		CCharacter *pChrTo = pPlayerTo->GetCharacter();
		if(!pChrTo)
			return;
		Pos = pChrTo->m_Pos;
	}
	pSelf->Teleport(pChr, Pos);
	pChr->ResetJumps();
	pChr->UnFreeze();
	pChr->ResetVelocity();
	pPlayer->m_LastTeleTee.Save(pChr);
}

void CGameContext::ConLastTele(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;
	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return;

	CGameTeams &Teams = pSelf->m_pController->Teams();
	int Team = pSelf->GetDDRaceTeam(pResult->m_ClientId);
	if(!Teams.IsPractice(Team))
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "You're not in a team with /practice turned on. Note that you can't earn a rank with practice enabled.");
		return;
	}
	if(!pPlayer->m_LastTeleTee.GetPos().x)
	{
		pSelf->SendChatTarget(pPlayer->GetCid(), "You haven't previously teleported. Use /tp before using this command.");
		return;
	}
	pPlayer->m_LastTeleTee.Load(pChr);
	pPlayer->Pause(CPlayer::PAUSE_NONE, true);
}

CCharacter *CGameContext::GetPracticeCharacter(IConsole::IResult *pResult)
{
	if(!CheckClientId(pResult->m_ClientId))
		return nullptr;
	CPlayer *pPlayer = m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return nullptr;
	CCharacter *pChr = pPlayer->GetCharacter();
	if(!pChr)
		return nullptr;

	CGameTeams &Teams = m_pController->Teams();
	int Team = GetDDRaceTeam(pResult->m_ClientId);
	if(!Teams.IsPractice(Team))
	{
		SendChatTarget(pPlayer->GetCid(), "You're not in a team with /practice turned on. Note that you can't earn a rank with practice enabled.");
		return nullptr;
	}
	return pChr;
}

void CGameContext::ConPracticeToTeleporter(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	CCharacter *pChr = pSelf->GetPracticeCharacter(pResult);
	if(pChr)
	{
		if(pSelf->Collision()->TeleOuts(pResult->GetInteger(0) - 1).empty())
		{
			pSelf->SendChatTarget(pChr->GetPlayer()->GetCid(), "There is no teleporter with that index on the map.");
			return;
		}

		ConToTeleporter(pResult, pUserData);
		pChr->ResetJumps();
		pChr->UnFreeze();
		pChr->ResetVelocity();
		pChr->GetPlayer()->m_LastTeleTee.Save(pChr);
	}
}

void CGameContext::ConPracticeToCheckTeleporter(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	CCharacter *pChr = pSelf->GetPracticeCharacter(pResult);
	if(pChr)
	{
		if(pSelf->Collision()->TeleCheckOuts(pResult->GetInteger(0) - 1).empty())
		{
			pSelf->SendChatTarget(pChr->GetPlayer()->GetCid(), "There is no checkpoint teleporter with that index on the map.");
			return;
		}

		ConToCheckTeleporter(pResult, pUserData);
		pChr->ResetJumps();
		pChr->UnFreeze();
		pChr->ResetVelocity();
		pChr->GetPlayer()->m_LastTeleTee.Save(pChr);
	}
}

void CGameContext::ConPoints(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(pResult->NumArguments() > 0)
	{
		if(!g_Config.m_SvHideScore)
			pSelf->Score()->ShowPoints(pResult->m_ClientId, pResult->GetString(0));
		else
			pSelf->Console()->Print(
				IConsole::OUTPUT_LEVEL_STANDARD,
				"chatresp",
				"Showing the global points of other players is not allowed on this server.");
	}
	else
		pSelf->Score()->ShowPoints(pResult->m_ClientId,
			pSelf->Server()->ClientName(pResult->m_ClientId));
}

void CGameContext::ConTopPoints(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvHideScore)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Showing the global top points is not allowed on this server.");
		return;
	}

	if(pResult->NumArguments() > 0)
		pSelf->Score()->ShowTopPoints(pResult->m_ClientId, pResult->GetInteger(0));
	else
		pSelf->Score()->ShowTopPoints(pResult->m_ClientId);
}

void CGameContext::ConTimeCP(IConsole::IResult *pResult, void *pUserData)
{
	CGameContext *pSelf = (CGameContext *)pUserData;
	if(!CheckClientId(pResult->m_ClientId))
		return;

	if(g_Config.m_SvHideScore)
	{
		pSelf->Console()->Print(IConsole::OUTPUT_LEVEL_STANDARD, "chatresp",
			"Showing the checkpoint times is not allowed on this server.");
		return;
	}

	CPlayer *pPlayer = pSelf->m_apPlayers[pResult->m_ClientId];
	if(!pPlayer)
		return;

	const char *pName = pResult->GetString(0);
	pSelf->Score()->LoadPlayerTimeCp(pResult->m_ClientId, pName);
}
