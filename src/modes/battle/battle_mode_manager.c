#include "battle_mode_manager.h"

void battle_init(Battle_manager* battle) {
    battle->bombs = NULL;
    battle->bricks = NULL;
    battle->collision = NULL;
    battle->fires = NULL;
    battle->power_ups = NULL;
    battle->entities = NULL;

    battle->current_map = 0;
    battle->win_goal = 0;
    battle->state = BATTLE_MENU_PLAYERS;
    
    battle->battle_time = 120;
    battle->battle_clock = 0;
    battle->battle_tick = SECOND;

    for (int i = 0; i < MAX_BATTLE_PLAYERS; ++i) {
        battle->players_on[i] = 0;
        battle->players_wins[i] = 0;
    }

    // Variables regarding players are set in the room starter
}

void battle_selection(Battle_manager* battle) {
    /*switch(battle->state) {
        case BATTLE_MENU_PLAYERS:
        for ()
    }*/
}

void battle_load(Battle_manager* battle, Screen* screen) {
    battle->camera = new_camera(get_center_x(screen, 256) + 8, get_center_y(screen, 224), BASE_WIDTH, BASE_HEIGHT, 0.25f, CAM_FOLLOW);

    int base_x = 48;
    int base_y = 48;

    int s_x = base_x;
    int s_y = base_y;

    srand(clock());

    battle->battle_clock = battle->battle_time;
    battle->battle_tick = SECOND;

    battle_free(battle);
    add_collision(&battle->collision, s_x-16,s_y-32,16*14,16);
    add_collision(&battle->collision, s_x-16,s_y+16*10,16*14,16);

    add_collision(&battle->collision, s_x-32,s_y-32,16,16*13);
    add_collision(&battle->collision, s_x+16*12,s_y-32,16,16*13);

    //for (int i = 0; i < 10000000; ++i) add_collision(&battle->collision, i*16,-16,16,16);

    for (int i = 0; i < 6; ++i) {
        for(int j = 0; j < 5; ++j){
            add_collision(&battle->collision, s_x + 32*i, s_y + 32*j, 16, 16);
        }
    }

    for (int i = 0; i < 11; ++i) {
        for(int j = 0; j < 13; ++j){
            if(i == 0 && j == 0 || i == 0 && j == 1 || i == 1 && j == 0 || 
                i == 10 && j == 12 || i == 9 && j == 12 || i == 10 && j == 11 ||
                i == 10 && j == 0 || i == 9 && j == 0 || i == 10 && j == 1 ||
                i == 0 && j == 12 || i == 0 && j == 11 || i == 1 && j == 12 ||
                i == 4 && j == 6 || i == 5 && j == 6 || i == 6 && j == 6 || i == 4 && j == 5 || i == 6 && j == 7 ||

                coll_meeting(&battle->collision, (s_x-16) + j*16, (s_y-16) + i*16, 16, 16)) continue;

            if (rand()%100 < 65)
                add_brick(&battle->bricks, &battle->collision, (s_x-16) + j*16, (s_y-16) + i*16, 16, 16);
        }
    }

    for (int p = 0; p < MAX_BATTLE_PLAYERS; ++p) {
        switch(p) {
            case 0:
                s_x = base_x;
                s_y = base_y;
                break;
            case 1:
                s_x = base_x + 16*12;
                s_y = base_y + 16*10;
                break;
            case 2:
                s_x = base_x + 16*12;
                s_y = base_y;
                break;
            case 3:
                s_x = base_x;
                s_y = base_y + 16*10;
                break;
            case 4:
                s_x = base_x + 16*(12/2);
                s_y = base_y + 16*(10/2);
                break;
        }

        battle->players[p] = new_player(&battle->entities, s_x-16 ,s_y-16,16,16,1,p);

        if (p < 2) {
            //if (p != 0) {
                battle->players[p].player_on = true;
                battle->players_on[p] = 1;
           // }
        }

        if (!battle->players[p].player_on) {
            battle->players[p].base->x = battle->players[p].base->y = -512;
        }
    }
}

void battle_update(Battle_manager* battle, Controller* controllers, Screen* screen) {

    battle->camera.follow_x = get_center_x(screen, 256) + 8;
    battle->camera.follow_y = get_center_y(screen, 224);

    int alive_players = MAX_BATTLE_PLAYERS;

    battle_clock_count(&battle->battle_clock, &battle->battle_tick);

    int gc = 0;
    for (int p = 0; p < MAX_BATTLE_PLAYERS; ++p) {

        if (battle->battle_clock == 0 && battle->battle_tick == 0) // Times up
        {
           player_kill(&battle->players[p], false);
        }

        if (!battle->players[p].player_on) {
            alive_players--;
            continue;
        }
        check_inputs(&controllers[gc]);
        
        p_update(&battle->players[p], &controllers[gc], &battle->entities, &battle->collision,
            &battle->bombs, &battle->fires, &battle->power_ups);


        gc++;
        if (battle->players[p].base->alive == false && battle->players[p].death_timer == 0) alive_players--;
    }

    b_update(&battle->bombs, &battle->fires, &battle->collision, &battle->power_ups, &battle->entities);
    f_update(&battle->fires, &battle->bombs);
    bri_update(&battle->bricks, &battle->fires, &battle->collision, &battle->power_ups);
    pw_update(&battle->power_ups, &battle->fires);
    cam_update(&battle->camera);

    if (alive_players <= 1) {
        for (int p = 0; p < MAX_BATTLE_PLAYERS; ++p) {
            if (battle->players[p].base->alive == true) {
                battle->players_wins[p]++;
            }
        }
        battle_load(battle, screen);
    }
    /*if (alive_players == 1) {
        battle->camera.state = CAM_FOLLOW;
        set_follow(&battle->camera, battle->players[0].base.x - battle->camera.width/2, 
            battle->players[0].base.y - battle->camera.height/2, 0.15f);
    }*/
}

void battle_clock_count(int* clock, int* second) {
    // Tick down if its not 0
    if (*clock >= 0) {
        if (*second <= 0) {
            (*second) = SECOND;
            if(*clock > 0)(*clock)--;
        } else {
            (*second)--;
        }
    }
}
void battle_free(Battle_manager* battle) {
    free_all_bricks(&battle->bricks, &battle->collision);
    free_collisions(&battle->collision);
    free_all_bombs(&battle->bombs, &battle->collision, &battle->entities);
    free_all_fires(&battle->fires);
    free_all_powerups(&battle->power_ups);
    free_all_entities(&battle->entities);
}