// stone.c
// ALN Sep / 97 
// Modified by haiyan

inherit ROOM;
#include <ansi.h>

string *practice_types = ({
        "blade",
        "club",
        "staff",
        "whip",
        "hammer",
        "unarmed",
        "parry",
        "dodge"
});
        
int *cost(object, string);
void dropoff(object);

void create()
{
        set("short", "巨石");
        set("long", @LONG
你站在悬崖上的巨石上，四周山风阵阵，你头发凌乱。巨石摇摇晃晃，你凝
气守中，双足稳稳站定。环顾四周，一片寂静，巨石底下是万丈深渊，似乎在此
很适宜练功。
LONG );

        set("outdoors", "shenlong");
        set("cost", 10);
        setup();
}

void init()
{
        object me = this_player();
        int lvl = (int)me->query_skill("dodge", 1);

        if( interactive(me) && !random(lvl) )
        {
            call_out("dropoff", 1, me);             
            return;
        }

        add_action("do_practice",  "practice");
        add_action("do_practice",  "lian");
        add_action("do_jump", "jump");
}

int do_practice(string arg)
{
        object weapon, me = this_player();
        string skillarg, str, multiple;
        int w, *cost, times;

        if( me->is_busy() )
        {
                tell_object(me, "你现在正忙着呢。\n");
                return 1;
        }

        if( me->is_fighting() )
        {
                tell_object(me, "你已经在战斗中了，学一点实战经验吧。\n");
                return 1;
        }

        if( !arg ) return ("/cmds/skill/enable")->main(me, arg);

        if(sscanf(arg, "%s %d", skillarg, times) != 2 )
                skillarg = arg;

        if( (w = member_array(skillarg, practice_types)) == -1 )
        {
                tell_object(me, "这项技能不能在这儿练习。\n");
                return 1;
        }

        if( w < 5 )
        {
                if( !objectp(weapon = me->query_temp("weapon")) )
                {
                        tell_object(me, "你没有拿武器怎么练习？\n");
                        return 1;
                } else
                if( weapon->query("skill_type") != skillarg )
                {
                        tell_object(me, "你手中的武器不适合练你想练习的技能。\n");
                        return 1;
                }
        }

        if( skillarg != "hammer" && me->query_skill(skillarg, 1) < 30 )
        {
                tell_object(me, "你还是先去跟师傅多学学吧。\n");
                return 1;
        }

        if( (int)me->query_skill(skillarg, 1) > 100 )
        {
                tell_object(me, "你已经不用在这儿花时间练习这项技能了。\n");
                return 1;
        }                

        if(!times || times == 0) {
                times = 1;
                multiple="";
        }
        else    multiple="反复";

        switch(skillarg)
        {
                case "blade"  : str = "挥舞" + weapon->name();  break; 
                case "club"   : str = "挥舞" + weapon->name();  break;
                case "staff"  : str = "挥舞" + weapon->name();  break;
                case "whip"   : str = "挥舞" + weapon->name();  break;
                case "hammer" : str = "挥舞" + weapon->name();  break;
                case "unarmed": str = ""; break;
                case "parry"  : str = "奋力拚博"; break;
                case "dodge"  : str = "纵闪跳跃"; break;
        }                
        message_vision("$N在狂风中" + str + multiple + "练习着" +
                       to_chinese(skillarg) + "。\n", me);

        for(times; times > 0; times--)
        {
                cost = cost(me, skillarg);
                if( (int)me->query("jing") < cost[0]
                    ||  (int)me->query("qi") < cost[1] )
                {
                        tell_object(me, "你疲惫不堪，气喘吁吁，只好停了下来。\n");
                        return 1;
                }
                me->receive_damage("jing", cost[0]);
                me->receive_damage("qi", cost[1]);
                me->improve_skill(skillarg, random((int)me->query_int()));

                if( !random((int)me->query_skill("dodge", 1) + 30 ))
                {
                        dropoff(me);
                        return 1;
                }
        }
        return 1;
}

int do_jump(string arg)
{
        object me = this_player(), *inv = all_inventory(me);
        int mydodge = me->query_skill("dodge", 1);

        if( !arg || arg != "down" )
                return notify_fail("你要往哪跳？\n");

        tell_object(me, "你纵身一跳，身子随即直落下去。\n");
        message("vision", me->name() + "纵身向海里跳去。\n", environment(me), me);
      
        if( random(mydodge) < 5 )
        {       
                me->receive_wound("jing", me->query("max_jing")/3 + random(me->query("max_jing")/2));
                me->receive_wound("qi", me->query("max_qi")/3 + random(me->query("max_qi")/2));

                for(int i = 0; i < sizeof(inv); i++)
                        if(!userp(inv[i]))   destruct(inv[i]);
                me->move(__DIR__"huangdao");
                message("vision", "你发现一个浑身水淋淋的家伙被海水冲上岸来，不由得走近一看，原来是" +
                        me->name() + "。\n", environment(me), me);
                me->unconcious();
                return 1;
        }

        me->move(__DIR__"sea");
        tell_object(me, "你“扑通”落入海里，苦涩的海水直往你的口鼻灌入。\n");
        message("vision", "只见" + me->name() + "从高处跳了下来。\n", environment(me), me);
        return 1;
}

void dropoff(object me)
{
        object *inv = all_inventory(me);

        tell_object(me, "巨石一晃，你立足不稳掉下海里了。\n");
        message("vision", me->name() + "失足掉了下去。\n", environment(me), me);
        me->move(__DIR__"sea");
        tell_object(me, "一股巨浪向你扑来，你咕噜咕噜吞了几口海水。\n");
        me->unconcious();
        me->receive_wound("jing", me->query("max_jing")/3 + random(me->query("max_jing")/2));
        me->receive_wound("qi", me->query("max_qi")/3 + random(me->query("max_qi")/2));

        for(int i = 0; i < sizeof(inv); i++)
            if(!userp(inv[i]))   destruct(inv[i]);

        me->move(__DIR__"huangdao");
        message("vision", "你发现一个浑身水淋淋的家伙被海水冲上岸来，不由得走近一看，原来是" +
                me->name() + "。\n", environment(me), me);
}

int *cost(object ob, string skill)
{
     switch(skill)
     {
          case "blade"  : return ({10, 25});
          case "club"   : return ({15, 20});
          case "staff"  : return ({20, 15});
          case "whip"   : return ({30, 15});
          case "unarmed": return ({15, 30});
          case "parry"  : return ({10, 20});
          case "dodge"  : return ({20, 10});
     }
     return ({15, 15});
}

