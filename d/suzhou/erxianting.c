// Room: /d/suzhou/erxianting.c
// Date: May 31, 98  Java

inherit ROOM;

void create()
{
	set("short", "二仙亭");
        set("long", @LONG
二仙亭在千人石东侧，建于北宋。亭壁嵌两方石碑，分别刻着五代宋初陈抟
与唐代吕洞宾二仙像。亭柱上有两幅石刻楹联：『梦中说梦原非梦，元里求元便
是元』；『昔日岳阳曾显迹，今朝虎阜再留踪』。
LONG );
	set("outdoors", "suzhou");
	set("no_clean_up", 0);
	set("exits", ([
		"west"  : "/d/suzhou/qianrenshi",
		"north" : "/d/suzhou/bailianchi",
	]));
	set("coor/x", 200);
	set("coor/y", -190);
	set("coor/z", 10);
	setup();
	replace_program(ROOM);
}

