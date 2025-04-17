#ifndef MASHY_LIB_CONSOLE_H_INCLUDED
#define MASHY_LIB_CONSOLE_H_INCLUDED

/*
void Clear_console(); // ID = 1295
Integer Show_console(Integer tf); // ID = 1728
Integer Is_console_visible(); // ID = 1729
Integer Float_console(Integer tf); // ID = 1730
Integer Is_console_floating(); // ID = 1731
Integer Set_console_position(Integer x,Integer y,Integer w,Integer h); // ID = 1732
Integer Get_console_position(Integer &x,Integer &y,Integer &w,Integer &h); // ID = 1733
Integer Minimize_console(Integer tf); // ID = 1734
Integer Is_console_minimized(); // ID = 1735
Integer Console_to_clipboard(); // ID = 1736
*/
void console_toggle_visible(){
    Show_console( Is_console_visible() ? 0 : 1 );
}

void console_force_onscreen_floating(){ // cant find a way to undock the console <= sometime crashes
    Minimize_console(0);
    Float_console(1);
    Show_console(1);
}
#endif