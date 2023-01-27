```C
struct game_state gs;

int main() {
    load_config(); // and persistent settings like controls
    query_saves(); // check if save games are available
    main_loop(); // main loop
    clean_up(); // game cleanup
}

enum {
    MAIN_MENU,
    GAME_PLAY,
    GAME_MENU,
    EXIT
} game_mode = menu;

void main_loop() {
    while (game_mode != EXIT) {
        main_menu_loop();
        game_loop();
        game_menu_loop();
    }
}

void main_menu_loop() {
    struct menu_state ms = INITIALIZE_MAIN_MENU;
    while (game_mode == MAIN_MENU) {
        /*
        buttons:
            play - push start values to db and start game
            load_game - does the same as play after loading saved state into gs
            exit - set game_mode to EXIT then return
        */
    }
    update_menu_state(ms);
    render_menu(ms);
}

void game_menu() {
    struct menu_state ms = INITIALIZE_GAME_MENU;
    while (game_mode == GAME_MENU) {
        /*
        buttons:
            continue - set game_mode to GAME
            save - write gamestate to disk. continue.
            main menu - set game_mode to MAIN_MENU
        */
    }
    update_menu_state(ms);
    render_menu(ms);
}

void game_loop() {
    while (game_mode == GAME) {
        update_game_state(game_mode); // sets game mode to GAME_MENU if menu key pressed
        render_game_state(gs);
    }
}
```
