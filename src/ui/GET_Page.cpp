#include "GET_Page.h"

static void textarea_event_handler(lv_event_t * e)
{
    lv_obj_t * ta = lv_event_get_target(e);
    Serial.printf("Enter was pressed. The current text is: %s\n", lv_textarea_get_text(ta));
}

static void btnm_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t * ta = (lv_obj_t *)lv_event_get_user_data(e);
    const char * txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));

    if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) lv_textarea_del_char(ta);
    else if(strcmp(txt, LV_SYMBOL_NEW_LINE) == 0) {

        //lv_event_send(ta, LV_EVENT_READY, NULL);
        //Serial.printf("取件码：%s\n",lv_textarea_get_text(ta));
        //获取用户输入的取件码的响应返回值
        char *boxId = ParseID(lv_textarea_get_text(ta));
       
        if(*boxId > '0'){
            Serial.printf("取件成功：%s\n",lv_textarea_get_text(ta));
            
             //清空文本框的内容
            lv_textarea_set_text(ta,"");

        }else {

            Serial.printf("取件码不正确:%s\n",lv_textarea_get_text(ta));

        }
    }
    
    else lv_textarea_add_text(ta, txt);

}

void createGETPage(lv_obj_t * parent){

    lv_obj_t *label = lv_label_create(parent);
    lv_obj_align(label, LV_ALIGN_TOP_MID, -70, 2);
    lv_obj_set_style_text_font(label, &myfont, 0);
    lv_label_set_text(label,"请输入取件码：");

    lv_obj_t * ta = lv_textarea_create(parent);
    lv_textarea_set_one_line(ta, true);
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, ta);
    lv_obj_add_state(ta, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/

    static const char * btnm_map[] = {"1", "2", "3", "\n",
                                      "4", "5", "6", "\n",
                                      "7", "8", "9", "\n",
                                      LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_NEW_LINE, ""
                                     };

    lv_obj_t * btnm = lv_btnmatrix_create(parent);
    lv_obj_set_style_text_font(btnm,&lv_font_montserrat_14,0);
    lv_obj_set_size(btnm, 260, 200);
    lv_obj_align(btnm, LV_ALIGN_BOTTOM_MID, 0, -3);
    lv_obj_add_event_cb(btnm, btnm_event_handler, LV_EVENT_VALUE_CHANGED, ta);
    lv_obj_clear_flag(btnm, LV_OBJ_FLAG_CLICK_FOCUSABLE); /*To keep the text area focused on button clicks*/
    lv_btnmatrix_set_map(btnm, btnm_map);
}