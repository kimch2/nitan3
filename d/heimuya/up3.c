// Room: carol/heimuya

#include <ansi.h>
inherit ROOM;

void check_trigger();
void on_board();
void arrive();
void close_passage();
void reset();
int do_insert(string arg);

void create()
{
        set("short", "悬崖");
        set("long", @LONG
又一座陡峭的悬崖立在了你的面前。悬崖上仿佛有座白色的建筑，可云烟缭
绕，看不清晰。崖上又垂下一条绳索(rope)，看上去象是悬崖上下互通消息的工
具，要想办法找其他的牌子啊。绳索的上面似乎隐约有一个吊篮(basket)，悬崖
的墙上似乎有一个匣子(xiazi)。
LONG );

    set("exits", ([
        "westdown" : "/d/heimuya/up2",
    ]));

    set("item_desc", ([
        "basket": "那是一个掉篮，看样子有些古怪。\n",
        "rope"  : "绳子就是绳子，可能是向上的工具。\n",
        "xiazi" : "这是一个匣子，看样子是一个开关，匣子的侧面有一个插孔。\n",
    ]));
    set("no_clean_up", 0);
    setup();
}


void init()
{
    add_action("do_insert", "insert");
}

void check_trigger()
{
    object room;

    if(!query("exits/enter") ) {
        if( !(room = find_object("/d/heimuya/basket")) )
            room = load_object("/d/heimuya/basket");
        if( room = find_object("/d/heimuya/basket") ) {
            if((int)room->query("insert_trigger")==0 ) {
                room->set("insert_trigger", 1);
                set("exits/enter", "/d/heimuya/basket");
                room->set("exits/out", __FILE__);
                message("vision", "“叭”的一声，你手中的令牌已被折断，只见一个"
                        "掉篮(basket)从天而降，你不禁一怔！\n", this_object());
                set("item_desc", ([
                    "basket" : "这是一个掉篮，是传说中的交通工具。\n",
                ]));
                
                remove_call_out("on_board");
                call_out("on_board", 15);
            }
            else
                message("vision", "看样子你得等下拨儿。\n",this_object() );
        }
        else
            message("vision", "ERROR: basket not found\n", this_object() );
    }
}


void on_board()
{
    object room, me = this_player();

    if( !query("exits/enter") ) return;
    tell_object(me, HIG"只见掉篮缓缓向上，四周无限美好，"
                "你似乎置身于一处人间仙境无异。\n"NOR);

    if( room = find_object("/d/heimuya/basket") )
    {
        room->delete("exits/out");
    }
    delete("exits/enter");
    remove_call_out("arrive");
    call_out("arrive",20);
}

void arrive()
{
    object room;
    if( room = find_object("/d/heimuya/basket") )
    {
        room->set("exits/out", "/d/heimuya/up4");
        message("vision", "噔的一声，你似乎一下从天上到了地上。\n",room );
    }
    remove_call_out("close_passage");
    call_out("close_passage", 20);
}

void close_passage()
{
    object room;
    if( room = find_object("/d/heimuya/basket") ) {
        room->delete("exits/out");
        room->delete("insert_trigger");
    }
}

int do_insert(string arg)
{
    string dir;
    object ob;
    object me;

    me=this_player();

    if( !arg || arg=="" ) return 0;

    if( arg=="卡" ) arg = "card3";   
    
    if( arg=="card3")
    {
        if( !objectp(ob = present(arg, me)) )
                return notify_fail("你身上没有这样东西。\n");
        if( !me->query_temp("mark/cards") )
                me->set_temp("mark/cards",1);
        if( me->query_temp("mark/cards")==4)
        {
                me->delete_temp("mark/cards");
                destruct(ob);
                check_trigger();
        }
        else    {
                me->add_temp("mark/cards",1);
                message("vision", "噔的一声，你看到掉篮向下一节。\n",me );
        }
    }
    else
    {
        if( !objectp(ob = present(arg, me)) )
                return notify_fail("你身上没有这样东西。\n");
        else    return notify_fail(ob->name() + "好像跟插孔不合，怎么也插不进去。\n");
    }
    return 1;
}

