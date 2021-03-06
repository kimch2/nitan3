// Room: /city/duchang.c
// YZC 1995/12/04 

inherit ROOM;

void create()
{
	set("short", "赌场");
	set("long", @LONG
这里是扬州赌坊的大堂，四周的房间里传出来吆五喝六的赌博声。赌坊布置
豪华，充满了温暖和欢乐，酒香中混合着上等脂粉的香气，银钱敲击，发出一阵
阵清脆悦耳的声音。世间几乎没有任何一种音乐能比得上。这赌坊实在是个很奢
侈的地方，随时都在为各式各样奢侈的人，准备着各式各样奢侈的享受。
LONG );
	set("item_desc", ([
		"paizi" : "二楼的拱猪房正处于测试阶段。\n",
	]));
	set("objects", ([
		CLASS_D("shenlong")+"/pang" : 1,
	]));
        set("no_fight",0);
	set("exits", ([
		"west" : "/d/city/daxiao",
		"up"   : "/d/city/duchang2",
		"east" : "/d/city/nandajie1",
	]));
	set("coor/x", -10);
	set("coor/y", -20);
	set("coor/z", 0);
	setup();
}
