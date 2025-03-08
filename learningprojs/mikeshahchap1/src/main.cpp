#include <iostream>
#include "ppm.h"
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

void test1() {
    PPM ppm1 {"spritefight_ascii.ppm"};
    ppm1.darken();
    ppm1.savePPM("spritefight_darkened.ppm");
}

void test2() {
    PPM ppm1 {"spritefight_ascii.ppm"};
    ppm1.lighten();
    ppm1.savePPM("spritefight_lightened.ppm");
}

int main() {
    test1();
    test2();
    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.