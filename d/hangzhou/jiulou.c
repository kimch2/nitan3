// jiulou.c
// Date: Nov.1997 by Venus
#include <room.h>
inherit ROOM;
void create()
{
        set("short", "楼外楼");
        set("long", @LONG
提起『楼外楼』方圆百里无人不知。这里各式名菜是杭州一绝。来游湖的人
们都一定会来这里尝尝。整个大堂闹哄哄的挤满了各式人等。店小二跑来跑去，
忙着招呼客人。墙上挂着价格牌子(paizi)。
LONG );
        set("exits", ([
            "west"      : "/d/hangzhou/road10",
            "east"      :"/d/hangzhou/chufang",
            "up"        : "/d/hangzhou/loveroom",
        ]));
        set("objects", ([
            "/d/hangzhou/npc/xiaoer2" : 1,
        ]));
        set("item_desc", ([
                "paizi" : @LONG
百果油包(baozi)               ：每个七十文铜板
东坡肉(pork)                  ：每碗五两白银
龙井虾仁(shrimp)              ：每碗五十文铜板
酒瓶(jiuping)                 ：每个十六两白银
西湖醋鱼(fish)                ：每碗一两白银又二十文铜板
叫化童鸡(chicken)             ：每只一两白银又二十文铜板
桂花鲜栗羹(Guihua geng)       ：每碗一两白银又二十文铜板
LONG ]));
        setup();
        replace_program(ROOM);
}

