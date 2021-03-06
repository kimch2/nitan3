// Room: /city/dangpu.c
// YZC 1995/12/04 

inherit ROOM;

void create()
{
	set("short", "当铺");
        set("long", @LONG
这是一家官家开的当铺，正对着对面的赌场，紧邻着的则是扬州有名的妓院
丽春院，那些手头不宽的狂赌客和嫖客自然是这里的常客了。一个五尺高的柜台
挡在你的面前，上面漆的字已经很模糊了，依稀是『童叟无欺』几个字。柜台上
挂着块牌子 (paizi)，后坐着的唐老板，一双精明的小眼睛上上下下打量着你。
LONG );
	set("no_fight", 1);
	set("no_steal", 1);
	set("no_beg",1);
	set("item_desc", ([
		"paizi" : "公平交易\n
sell        卖 
buy         买
pawn        当
redeem      赎
value       估价
",
	]));
	set("objects", ([
		"/d/city/npc/tang" : 1,
	]));
	set("exits", ([
		"west" : "/d/city/nandajie1",
	]));

	set("coor/x", 10);
	set("coor/y", -20);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}