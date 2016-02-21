//Room: fufeng3.c

inherit ROOM;

void create ()
{
        set ("short", "扶风道");
        set("long", @LONG
扶风道是长安城中心大街，青石板路笔直的伸展出去，一路上行人比肩，笙
歌处处。景物之美，直如神仙境地。西面有一条大路通往长安城的西城门。
LONG );
        set("exits", 
        ([ //sizeof() == 3
		"north" : "/d/changan/fufeng2",
        	"south" : "/d/changan/fufeng4",
        	"west" : "/d/changan/xian-dadao",
        ]));

        set("outdoors", "changan");
        set("coor/x", -420);
	set("coor/y", 0);
	set("coor/z", 0);
	setup();
        replace_program(ROOM);
}
