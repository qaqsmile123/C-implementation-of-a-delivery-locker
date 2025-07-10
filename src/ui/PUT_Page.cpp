#include "PUT_Page.h"

static void textarea_event_handler(lv_event_t * e)
{
    lv_obj_t * ta = lv_event_get_target(e);
    Serial.printf("Enter was pressed. The current text is: %s\n", lv_textarea_get_text(ta));
}

static void btnm_event_handler(lv_event_t * e)
{
    //obj是触发事件的主体：btnm
    lv_obj_t * obj = lv_event_get_target(e);
    //获取到事件函数参数中的数据 参数就是ta
    lv_obj_t * ta = (lv_obj_t *)lv_event_get_user_data(e);
    //获取每次点击按钮矩阵中的文本
    const char * txt = lv_btnmatrix_get_btn_text(obj, lv_btnmatrix_get_selected_btn(obj));

    if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) lv_textarea_del_char(ta);
    else if(strcmp(txt, LV_SYMBOL_NEW_LINE) == 0) {

        //给ta发送一个LV_EVENT_READY事件，刚好可以触发ta文本框的事件
        // lv_event_send(ta, LV_EVENT_READY, NULL);
        //Serial.printf("手机号：%s\n",lv_textarea_get_text(ta));
        
        //发送一个mqtt请求
        mqtt_sendData(lv_textarea_get_text(ta));
        //清空文本框的内容
        lv_textarea_set_text(ta,"");
    }
    
    else lv_textarea_add_text(ta, txt);

}

void createPUTPage(lv_obj_t * parent){

    lv_obj_t *label = lv_label_create(parent);
    lv_obj_align(label, LV_ALIGN_TOP_MID, -70, 2);
    lv_obj_set_style_text_font(label,&myfont,0);
    lv_label_set_text(label,"请输入手机号：");
    
    lv_obj_t * ta = lv_textarea_create(parent);
    lv_textarea_set_one_line(ta, true);//保证输入的内容在一行
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 30);//放置位置
    //添加一个事件，参数：1.谁的事件 2.事件函数 3.事件触发源头 4.事件函数的参数
    lv_obj_add_event_cb(ta, textarea_event_handler, LV_EVENT_READY, ta);
    //保证光标一直在输入框中闪烁
    lv_obj_add_state(ta, LV_STATE_FOCUSED); /*获取焦点*/
    //创建了一个字符数组
    static const char * btnm_map[] = {"1", "2", "3", "\n",
                                      "4", "5", "6", "\n",
                                      "7", "8", "9", "\n",
                                      LV_SYMBOL_BACKSPACE, "0", LV_SYMBOL_NEW_LINE, ""
                                     };
    //创建按一个按钮矩阵                                
    lv_obj_t * btnm = lv_btnmatrix_create(parent);
    lv_obj_set_style_text_font(btnm,&lv_font_montserrat_14,0);
    lv_obj_set_size(btnm, 260, 200);
    lv_obj_align(btnm, LV_ALIGN_BOTTOM_MID, 0, -3);
    lv_obj_add_event_cb(btnm, btnm_event_handler, LV_EVENT_VALUE_CHANGED, ta);
    lv_obj_clear_flag(btnm, LV_OBJ_FLAG_CLICK_FOCUSABLE); //保证文本框的焦点，
    //设置按钮矩阵和字符数组进行匹配
    lv_btnmatrix_set_map(btnm, btnm_map);
}

