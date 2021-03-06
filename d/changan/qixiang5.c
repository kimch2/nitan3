//Room: qixiang5.c

inherit ROOM;

void create ()
{
        set ("short", "麒祥街");
        set("long", @LONG
麒祥街是长安城北一条东西向的大道，街面宽敞洁净，街上十分热闹，人来
人往川流不息。大街的两旁商家云集，大大小小的店铺比邻而立。北边是一家首
饰店，南边就是长安城有名的河洛镖局。
LONG );
        set("exits", 
        ([ //sizeof() == 4
		"south" : "/d/changan/biaoju-damen",
        	"north" : "/d/changan/shoushi-dian",
        	"west" : "/d/changan/qixiang4",
       		"east" : "/d/changan/qixiang6",
        ]));

        set("outdoors", "changan");
        set("coor/x", -380);
	set("coor/y", 40);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}
