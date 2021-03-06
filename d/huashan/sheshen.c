// sheshenya.c

#include <ansi.h>
inherit ROOM;

void create()
{
        set("short", "舍身崖");
        set("long", @LONG
这是一段极窄极险的山崖，四周云雾飞绕，下面渊深无底。如不小心，
一失足掉下去，只怕连骨头都找不到。边上有个小山洞(dong)。很多江湖
中人想不开的时候都选择在这里跳(jump)崖自杀。
LONG );
        set("exits", ([ /* sizeof() == 1 */
                "eastdown" : __DIR__"canglong",
        ]));
        set("item_desc", ([
                "dong" : "紧靠飞崖下石壁的小洞，黑呼呼的不知有多深。\n",
        ]));
        set("outdoors", "huashan" );

        setup();
}

void init()
{
        object me = this_player();
        add_action("do_enter", "enter");
        add_action("do_enter", "zuan");
	add_action("do_jump", "jump");        
        if (me->query_temp("xunshan")) 
                me->set_temp("xunshan/sheshen", 1);
        return;
}

int do_enter(string arg)
{
        object me;
        mapping fam;

        me = this_player();
        if( !arg || arg=="" ) return 0;
        if( arg=="dong" )
        {
                if( (fam = me->query("family")) && fam["family_name"] == "丐帮")
                {
                        message("vision",
                                me->name() + "运起丐帮缩骨功，一弯腰往洞里钻了进去。",
                                environment(me), ({me}) );
                        me->move("/d/gaibang/undertre");
                        message("vision",
                                me->name() + "从洞里走了进来。\n",
                                environment(me), ({me}) );
                                return 1;
                } else  
                        return notify_fail("这么小的洞，你钻得进去吗？\n");
        }
}

int do_jump(string arg)
{
	object ob;
	int new_gin;
	ob = this_player();
        new_gin = random( ob->query("max_qi")*3 );
        if ( new_gin > ob->query("qi") ) new_gin = ob->query("qi") + 1;
        if( !arg || arg=="" ) return 0;
        if( arg != "down" )
		return notify_fail("你想要跳崖自杀？这么想不开啊？\n");
	tell_object(ob, HIR"你不加思索，纵身一跃，跳下了悬崖。。。。\n"NOR);
	message("vision", NOR"只见" + ob->query("name") + "万念俱灰，纵身跳入了悬崖之中。。。。\n"NOR, 
	        environment(ob), ob);
        if((random((int)ob->query("kar")) < 5 ) && ((int)ob->query_skill("dodge") < 200))
	{
                ob->set_temp("die_reason", "跳崖自杀而亡");
		ob->die();
		return 1;
	}
        ob->receive_damage("qi", new_gin);
        ob->move("/d/yihua/yanfeng");
	return 1;
}
