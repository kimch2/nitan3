//Room: qixiang6.c

inherit ROOM;

void create ()
{
        set ("short", "麒祥街");
        set("long", @LONG
麒祥街是长安城北一条东西向的大道，街面宽敞洁净，街上十分热闹，人来
人往川流不息。大街的两旁商家云集，大大小小的店铺比邻而立。北边是一家药
店，往东就到了和连德大道交叉的连德北口。
LONG );
        set("exits", 
        ([ //sizeof() == 4
		"south" : "/d/changan/weapon-shop",
        	"north" : "/d/changan/yaopu",
        	"west" : "/d/changan/qixiang5",
        	"east" : "/d/changan/liande-beikou",
        ]));

        set("outdoors", "changan");
        set("coor/x", -370);
	set("coor/y", 40);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}
