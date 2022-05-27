/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2021. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_SWITCHTABS                 2       /* control type: tab, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_CANVAS                  2       /* control type: canvas, callback function: (none) */
#define  TABPANEL_TIMEDIFF                3       /* control type: scale, callback function: (none) */
#define  TABPANEL_ANGLE_MS                4       /* control type: scale, callback function: (none) */
#define  TABPANEL_ANGLE_DIFFERENCE        5       /* control type: scale, callback function: (none) */
#define  TABPANEL_ANGLE_TIME              6       /* control type: scale, callback function: (none) */
#define  TABPANEL_SPEEDOMETER             7       /* control type: scale, callback function: (none) */
#define  TABPANEL_GRAPH                   8       /* control type: graph, callback function: (none) */
#define  TABPANEL_QUIT                    9       /* control type: command, callback function: quit */
#define  TABPANEL_FINDLINE                10      /* control type: command, callback function: find_line */
#define  TABPANEL_REVERSEPIXEL            11      /* control type: command, callback function: reverse_pixel */
#define  TABPANEL_THRESHOLD               12      /* control type: command, callback function: threshold */
#define  TABPANEL_RESIZEIMAGE             13      /* control type: command, callback function: resize_image */
#define  TABPANEL_LOADIMAGE               14      /* control type: command, callback function: load_image */
#define  TABPANEL_LED                     15      /* control type: LED, callback function: (none) */
#define  TABPANEL_THRESHOLD_VALUE         16      /* control type: scale, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_2_CANVAS_4              2       /* control type: canvas, callback function: (none) */
#define  TABPANEL_2_CANVAS_5              3       /* control type: canvas, callback function: (none) */
#define  TABPANEL_2_SPEEDOMETER_2         4       /* control type: scale, callback function: (none) */
#define  TABPANEL_2_ANGLE_DIFF            5       /* control type: scale, callback function: (none) */
#define  TABPANEL_2_CANVAS_3              6       /* control type: canvas, callback function: (none) */
#define  TABPANEL_2_CANVAS_1              7       /* control type: canvas, callback function: (none) */
#define  TABPANEL_2_CANVAS_2              8       /* control type: canvas, callback function: (none) */
#define  TABPANEL_2_RECOGNITION           9       /* control type: command, callback function: one_press_recognition */
#define  TABPANEL_2_QUIT                  10      /* control type: command, callback function: quit2 */

     /* tab page panel controls */
#define  TABPANEL_3_TEXTMSG_2             2       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_3_TEXTMSG               3       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_3_TEXTMSG_3             4       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_3_TEXTMSG_6             5       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_3_TEXTMSG_9             6       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_3_TEXTMSG_8             7       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_3_TEXTMSG_4             8       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_3_PICTUREBUTTON         9       /* control type: pictButton, callback function: quit3 */
#define  TABPANEL_3_TEXTMSG_7             10      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_3_PICTURE_5             11      /* control type: picture, callback function: (none) */
#define  TABPANEL_3_PICTURE_2             12      /* control type: picture, callback function: (none) */
#define  TABPANEL_3_PICTURE_3             13      /* control type: picture, callback function: (none) */
#define  TABPANEL_3_PICTURE               14      /* control type: picture, callback function: (none) */
#define  TABPANEL_3_PICTURE_4             15      /* control type: picture, callback function: (none) */
#define  TABPANEL_3_TEXTMSG_5             16      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK find_line(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK load_image(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK one_press_recognition(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK quit2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK quit3(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK resize_image(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK reverse_pixel(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK threshold(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
