#include "text.h"

void lv_text(void)
{
		lv_obj_t *label = lv_label_create(lv_scr_act());
		lv_obj_set_style_text_color(label,lv_palette_main(LV_PALETTE_RED),LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(lv_scr_act(),lv_palette_main(LV_PALETTE_BLUE),LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(label,&lv_font_montserrat_14,LV_STATE_DEFAULT);
		lv_label_set_text(label,"Hello World!!!");
}
