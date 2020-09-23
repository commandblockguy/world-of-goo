/*
 *--------------------------------------
 * Program Name:
 * Author:
 * License:
 * Description:
 *--------------------------------------
*/

#include <graphx.h>
#include <keypadc.h>
#include <tice.h>
#include "gfx/gfx.h"
#include "gooball.h"
#include "util.h"

int main(void)
{
    uint24_t x = LCD_WIDTH / 2;
    uint8_t y = LCD_HEIGHT / 2;

    gen_lookups();
    gfx_Begin();
    gfx_SetDrawBuffer();
    gfx_SetTransparentColor(7);

    add_goo(140 * 256, 100 * 256);
    add_goo(180 * 256, 100 * 256);
    add_goo(140 * 256, 140 * 256);
    add_goo(180 * 256, 140 * 256);
    connect(&gooballs[0], &gooballs[3]);

    while(!kb_IsDown(kb_KeyClear)) {
        if(kb_IsDown(kb_KeyLeft)) x--;
        if(kb_IsDown(kb_KeyRight)) x++;
        if(kb_IsDown(kb_KeyUp)) y--;
        if(kb_IsDown(kb_KeyDown)) y++;

        if(kb_IsDown(kb_Key2nd)) {
            gooball_t *gooball = add_goo(x * 256, y * 256);
            if(gooball->num_connections < 2) remove_goo(gooball);
            while(kb_IsDown(kb_Key2nd)) kb_Scan();
        }


        for(uint8_t i = 0; i < num_gooballs; i++) {
            process_goo(&gooballs[i]);
        }
        for(uint8_t i = 0; i < num_gooballs; i++) {
            apply_velocity(&gooballs[i]);
        }

        gfx_FillScreen(0xFF);
        gfx_SetColor(74);
        // Draw connections
        for(uint8_t i = 0; i < num_gooballs; i++) {
            gooball_t *gooball = &gooballs[i];
            for(uint8_t j = 0; j < gooball->num_connections; j++) {
                if(!gooball->connections[j].primary) continue;
                gooball_t *other = gooball->connections[j].gooball;
                gfx_Line(gooball->position_x / 256, gooball->position_y / 256,
                         other->position_x / 256, other->position_y / 256);
            }
        }
        for(uint8_t i = 0; i < num_gooballs; i++) {
            gooball_t *gooball = &gooballs[i];
            gfx_TransparentSprite(common, gooball->position_x / 256 - common_width / 2,
                                  gooball->position_y / 256 - common_height / 2);
        }
        gfx_SetColor(0);
        gfx_FillRectangle_NoClip(0, GROUND_Y / 256, LCD_WIDTH, LCD_HEIGHT - GROUND_Y / 256);

        gfx_SetColor(181);
        gfx_FillCircle(x, y, GOOBALL_RADIUS / 256);

        gfx_SwapDraw();

        kb_Scan();
    }


    gfx_End();
    return 0;
}
