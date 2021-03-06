// Room: /d/suzhou/nandajie1.c
// Date: May 31, 98  Java

inherit ROOM;

void create()
{
	set("short", "南大街");
        set("long", @LONG
南大街乃是城里的繁华地段，一到晚上，一派灯红酒绿，尤为热闹。笑声、
歌声、琴声、箫声，汇成一片送入了你的耳朵，你不禁心猿意马，很想就此停步
享受一番。苏州的城里秀丽景色此时已使你心旷神怡，此时此刻只想与这大自然
秀丽的景色融为一体。从这里看去，苏州南城的街景是一片喧闹。在街的东北面
是一个茶馆，正东望去是一座苏州园林。
LONG );
	set("outdoors", "suzhou");
	set("no_clean_up", 0);
	set("exits", ([
		"east"      : "/d/suzhou/shizilin",
		"west"      : "/d/suzhou/jubaozhai",
		"north"     : "/d/suzhou/baodaiqiao",
		"south"     : "/d/suzhou/nandajie2",
		"southwest" : "/d/suzhou/shuyuan",
		"northeast" : "/d/suzhou/chaguan",
	]));
	set("coor/x", 200);
	set("coor/y", -230);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}

