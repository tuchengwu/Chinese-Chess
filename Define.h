#ifndef __DEFINE_H__
#define __DEFINE_H__

#define WIDTH 640
#define HEIGHT 360

enum TAG { NONE, RED, BLACK }; //棋子颜色

enum TYPE { JIANG, SHI, XIANG, MA, CHE, PAO, BING }; //棋子类型
static char* TypeStr[] = { "jiang","shi","xiang","ma","che","pao","bing" };

#endif
