//Room: /d/dali/badidian.c
//Date: June. 29 1998 by Java

inherit ROOM;
void create()
{
	set("short","巴的甸");
        set("long", @LONG
这里就是大理西部要镇巴的甸，是乌夷的重要治府，多个乌夷部落分布在附
近。坐于东泸水谷地正中，巴的甸四周多农田，以此为中心，道路四通八达连接
各方的部落。
LONG );
	set("outdoors", "dali");
	set("exits",([ /* sizeof() == 1 */
	    "north"      : "/d/dali/zhenxiong",
	    "south"      : "/d/dali/lushui",
	    "eastup"     : "/d/dali/lushuieast",
	]));
	set("no_clean_up", 0);
	setup();
	replace_program(ROOM);
}

