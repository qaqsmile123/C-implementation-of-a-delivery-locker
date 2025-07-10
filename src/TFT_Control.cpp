#include "TFT_Control.h"

//TFT_eSPI tft = TFT_eSPI();       // 定义一个显示屏对象

static const uint16_t screenWidth  = 480;
static const uint16_t screenHeight = 320;

//缓冲区
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 10 ];

//初始化一个tft显示屏并设置分辨率
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
//定义一个触摸屏对象
TAMC_GT911 tp = TAMC_GT911(TOUCH_SDA, TOUCH_SCL, TOUCH_INT, TOUCH_RST, screenWidth, screenHeight);


/* 显示缓冲区的回调函数 */
void my_disp_flush( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p )
{
    //计算需要刷新的区域的宽度和高度
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );
    //开始向tft屏幕写入数据
    tft.startWrite();   //启动tft的写入操作
    tft.setAddrWindow( area->x1, area->y1, w, h );   //设置屏幕刷新的窗口区域（起始x，起始y，宽，高）
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );    //将lvgl的颜色数据推送至屏幕，转换为16位颜色格式
    tft.endWrite(); //结束tft的写入操作

    // 通知LVGL刷新操作已完成
    lv_disp_flush_ready( disp_drv );    // 告诉LVGL当前刷新任务已完成，可以进行下一帧渲染
}

/*触摸屏的回调函数*/
//我们自己写业务
void my_touchpad_read( lv_indev_drv_t * indev_drv, lv_indev_data_t * data )
{
    tp.read();  //读取触摸屏的当前状态和数据
    //如果用户触摸了屏幕
    if(tp.isTouched){

        data->state = LV_INDEV_STATE_PR;    //设置输入设备状态为“按下”
        for(int i = 0; i < tp.touches; i++){    //遍历所有触摸点(支持多点触摸)
            data->point.x = tp.points[i].x;    //获取触摸点的x坐标并传递给lvgl
            data->point.y = tp.points[i].y;    //获取触摸点的y坐标并传递给lvgl
            //串口打印触摸的坐标信息
            // Serial.print("Touch ");Serial.print(i+1);Serial.print(": ");;
            // Serial.print("  x: ");Serial.print(tp.points[i].x);
            // Serial.print("  y: ");Serial.print(tp.points[i].y);
            // Serial.print("  size: ");Serial.println(tp.points[i].size);
            // Serial.println(' ');
        }
    } else {

        data->state = LV_INDEV_STATE_REL;   //如果没有触摸，设置状态为“释放”
    }
}

void Screen_Init(){

    tft.init();//初始化tft显示屏
    tft.setRotation(3);//显示屏旋转几次

    tp.begin();//触摸屏的初始化
    tp.setRotation(ROTATION_LEFT);//触摸屏旋转几次

    lv_init();//lvgl的初始化
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );//初始化缓冲区

    //初始化显示驱动
    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;      //定义显示驱动结构体
    lv_disp_drv_init( &disp_drv );      //初始化显示驱动
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;     //设置水平分辨率为480像素
    disp_drv.ver_res = screenHeight;    //设置垂直分辨率为320像素
    disp_drv.flush_cb = my_disp_flush;  //指定显示刷新回调函数为my_disp_flush
    disp_drv.draw_buf = &draw_buf;      //将缓冲区绑定到显示驱动
    lv_disp_drv_register( &disp_drv );  //向lvgl注册显示驱动

    //初始化输入设备驱动
    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;        //定义输入设备驱动结构体
    lv_indev_drv_init( &indev_drv );        //初始化输入设备驱动
    indev_drv.type = LV_INDEV_TYPE_POINTER; //设置输入设备类型为指针(适用于触摸屏)
    indev_drv.read_cb = my_touchpad_read;   //指定触摸屏读取回调函数为my_touchpad_read
    lv_indev_drv_register( &indev_drv );    //向lvgl注册输入设备驱动


    /* 创建一个label标签 */
    //lv_scr_act() LVGL活动屏幕对象
    // lv_obj_t *label = lv_label_create( lv_scr_act() );//创建一个label的标签，放到整个屏幕上
    // lv_label_set_text( label, "Hello Ardino and LVGL!");
    // lv_obj_align( label, LV_ALIGN_CENTER, 20, 0 );//参数： 1.对象，2.起始位置，3.x轴的偏移量，4.y轴的偏移量
}

void Screen_Loop(){

    lv_timer_handler(); //让lvgl开始工作，调用lvgl的定时器处理函数，确保gui持续刷新并响应事件
}