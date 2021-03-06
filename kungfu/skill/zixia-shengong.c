// Created by Lonely (06/04/2002)
// zixia-shengong.c 华山 紫霞神功

inherit FORCE;

#include <ansi.h>

int valid_enable(string usage)
{
        return usage == "force";
}

int valid_force(string force) 
{
        return 1; 
}

int query_neili_improve(object me) 
{
        int lvl; 
        lvl = (int)me->query_skill("zixia-shengong", 1); 
        return lvl * lvl * 20 * 15 / 100 / 200; 
}

int valid_learn(object me)
{
        int i, t, level;
        
        level = (int)me->query_skill("zixia-shengong", 1);
        
        if (level <= 100)
                for (i = 1; i < level / 10; i++)
                        t *= 2;
        
        if (me->query("gender") == "无性" && level > 49)
                return notify_fail("公公无根无性，紫霞神功再难更上一层楼。\n");

        if ((int)me->query_skill("zixia-shengong", 1) >= 45 
        &&  (int)me->query_skill("huagong-dafa", 1) >= 100
        &&  (int)me->query_int() < 100 )
                return notify_fail("你受高深的邪派内功牵绊，与紫霞神功水火难容，以你的智商再难更上一层楼。\n");

        if (level <= 100)
        {
                if (level > 10 && (int)me->query("shen") < t * 100) 
                        return notify_fail("你的侠义正气太低了，无法领会更高深的紫霞神功。\n");
        }
        else
        {
                if ((int)me->query("shen") < (51200 + (level - 100) * 1000)) 
                        return notify_fail("你的侠义正气太低了，无法领会更高深的紫霞神功。\n");
        }
        return ::valid_learn(me);
}

int practice_skill(object me)
{
        return notify_fail("紫霞神功只能用学(learn)的来增加熟练度。\n"); 
}

string exert_function_file(string func)
{
        return __DIR__"zixia-shengong/" + func;
}

mapping heal_msg(object me)
{
        return ([
                "apply_short"  :  "脸上紫气忽隐忽现。",
                "start_self"    : MAG "你就地盘坐，运使紫霞神功疗伤，内息返诸柔善，鸣天鼓，饮玉浆，荡华池，叩金梁，流通周身脉络。\n" NOR,
                "start_other"   : MAG + me->name() + "就地盘坐，脸现紫气，开始运功疗伤。\n" NOR,
                "finish_self"   : MAG "你催运紫霞神功疗伤多时，一道真气在体内运转无碍，内伤已经平复，当即站起身来。\n" NOR,
                "finish_other"  : MAG "但见"+ me->name()+ "头顶白雾弥漫，脸上紫气大盛，忽然间一声长啸，站起身来。\n" NOR,
        ]);
}

mapping exercise_msg(object me)
{
        return ([
                "apply_short"  : "双手捏个剑诀，脸上紫气浮现。",
                "exercise_msg" : MAG "你运功引导内息返诸柔善，鸣天鼓，饮玉浆，荡华池，叩金梁，流通周身脉络。\n" NOR,
                "start_self"   : MAG "你屏息静气，坐了下来，左手搭在右手之上，在胸前捏了个剑诀，引导内息游走各处经脉。\n" NOR,
                "start_other"  : MAG + me->name() + "坐了下来，少顷脸上隐隐笼罩了一层紫气。\n" NOR,
                "halt_self"    : MAG "你心神一动，将内息压回丹田，双臂一振站了起来。\n" NOR,
                "halt_other"   : MAG + me->name() + "深吸一口气，双臂一振站了起来。\n" NOR,
                "finish_self"  : MAG "你将内息走满一个周天，只感到全身通泰，丹田中暖烘烘的，双手一分，缓缓站了起来。\n"NOR,
                "finish_other" : MAG + me->name() + "脸上紫气退了下去，站了起来。\n" NOR,
        ]);
}

