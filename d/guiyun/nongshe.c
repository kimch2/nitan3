// Room: nongshe.c
// By Dumbness, 2003-4-28 21:34

inherit ROOM;

void create()
{
        set("short", "无名农舍");
        set("long", @LONG
竹林深处，一户普通的人家。一位老人斜卧在阴暗的角落里，不时发出痛
苦的呻吟声。普通的小屋，平淡无奇的老人，可正当你松了一口气打量小屋四
周的时候，却突然感觉有道目光透视着你灵魂的深处，让你有股莫名的恐惧。
LONG );
	set("outdoors", "guiyun"); 
	set("exits", ([
		"southwest" : __DIR__"zhulin",
	]));
	set("objects", ([ 
		__DIR__"npc/laoren" : 1, 
	])); 
	set("coor/x", -190);
	set("coor/y", -130);
	set("coor/z", 0);
	setup(); 
	replace_program(ROOM); 
}