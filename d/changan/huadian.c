//Room: huadian.c

inherit ROOM;

void create ()
{
        set ("short", "花店");
        set("long", @LONG
还没进门，你就已经闻到一股清新的花香。店中各种名花，琳琅满目，不乏
珍奇异种。来客不分高低贵贱，这里的主人总是笑脸相迎。许多城中的小伙子都
喜欢在这儿逗留，想挑选一些美丽的鲜花送给心上的姑娘。
LONG );
        set("exits", 
        ([ //sizeof() == 1
                "south" : "/d/changan/huarui3",
        ]));
        set("objects", 
        ([ //sizeof() == 1
                "/d/changan/npc/girl" : 1,
        ]));

        set("coor/x", -410);
	set("coor/y", -20);
	set("coor/z", 0);
	setup();
	replace_program(ROOM);
}
