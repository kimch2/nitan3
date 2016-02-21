#include <room.h>
inherit ROOM;

void create()
{
	set("short", "大官道");
        set("long", @LONG
你走在一条尘土飞扬的大官道上。不时有两三骑快马从身边飞驰而过，扬起
一路尘埃。道路两旁是整整齐齐的杨树林。
LONG );
       set("exits", ([
		"east" : "/d/beijing/shi_2",
		"north" : "/d/beijing/guandao_2",
	]));
	set("objects", ([
		"/d/beijing/npc/tangzi" : 1,
	]));
	set("outdoors", "beijing");
	set("coor/x", -70);
	set("coor/y", 310);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}
