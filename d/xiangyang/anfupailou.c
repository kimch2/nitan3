// Room: /d/xiangyang/anfupailou.c
// Date: Jan. 8 1999 by Winder

inherit ROOM;

void create()
{
	set("short", "安抚府牌楼");
        set("long", @LONG
这里是安抚使府前的一座牌楼，高达二丈，由黄玉制成，和东面的将军府屋
顶的琉璃瓦在阳光的照耀下黄白掩映，金碧辉煌，令人目为之眩，感觉分外巍峨
庄严。你抬头望去，但见牌楼上四个大金字『安抚使府』。牌楼下站着两派手那
刀枪的宋兵，紧紧盯着每一个靠近这儿的人。北面就是安抚府大院了。南面就是
襄阳的中央广场。
LONG );
        set("outdoors", "xiangyang");
	set("no_clean_up", 0);

	set("exits", ([
		"south" : "/d/xiangyang/guangchang",
		"north" : "/d/xiangyang/anfugate",
	]));
	set("coor/x", -290);
	set("coor/y", -80);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}
