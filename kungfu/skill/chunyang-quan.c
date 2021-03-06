// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.
// chunyang-quan.c 纯阳拳

#include <ansi.h>
inherit SKILL;

int chunyangwuji(object me, object victim, object weapon, int damage);

string *action_msg = ({
        "$N施展「玄鹤捕食」，身子腾空跃起，两臂张开，如仙鹤临空，紧跟着右拳左掌突然随势向前疾探，打向$n$l",
        "$N左手使出「金蛇戏龟」，左右虚晃几招，右拳“嗖”地一声从掌风中猛穿出来，击向$n的$l",
        "$N气沉丹田，猛然怒喝一声，向前一个弓步，双掌「野马上槽」，带着风雷之声奋力击向$n",
        "$N腾空飞起，一式「黑虎掏心」，右拳对着$n胸口雷霆般猛捣过去",
        "$N往前一个猛冲，一式「二龙戏珠」，双拳突然同时击出，直取$n的双目",
        "$N上身向后一仰，正好避开$n的攻击，突然一转身，反手一招「麒麟反挂」，一把扭住$n臂膀，右拳却捣向$n的面门",
        "$N向前一个弓箭步，使出「青龙夺食」，一记勾拳，不顾一切地捣向$n",
        "$N运起丹田之气，猛喝一声，突然一式「赤龙穿云」，右拳变的赤红，挟带着狂风，如雷似电捣往$n$l",
});

int valid_enable(string usage) { return  usage=="cuff" || usage=="parry"; }

int valid_combine(string combo) { return combo=="sanhua-juding"; }

int valid_learn(object me)
{
        if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
                return notify_fail("练纯阳拳必须空手。\n");
        if ((int)me->query_skill("force", 1) < 15)
                return notify_fail("你的内功火候不够，无法学纯阳拳。\n");
        if ((int)me->query("max_neili") < 50)
                return notify_fail("你的内力太弱，无法练纯阳拳。\n");
        return 1;
}

mapping query_action(object me, object weapon)
{
        return ([
                "action": action_msg[random(sizeof(action_msg))], 
                "damage": 120 + random(20), 
                "attack": 60 + random(10), 
                "dodge" : 60 + random(10), 
                "parry" : 60 + random(10), 
//                "post_action": (: chunyangwuji :),
                "damage_type" : random(2)?"瘀伤":"内伤", 
        ]); 
}

int practice_skill(object me)
{
        if ((int)me->query("qi") < 70)
                return notify_fail("你的体力太低了。\n");
        if ((int)me->query("neili") < 70)
                return notify_fail("你的内力不够练纯阳拳。\n");
        me->receive_damage("qi", 60);
        me->add("neili", -60);
        return 1;
}

string perform_action_file(string action)
{
        return __DIR__"chunyang-quan/" + action;
}

void finish_chunyangwuji(object me, int skill_lvl) 
{ 
        me->set_temp("apply/attack", me->query_temp("cyq-wuji/old_attack")); 
        me->set_temp("apply/damage", me->query_temp("cyq-wuji/old_damage")); 
        me->delete_temp("perform/chunyangwuji"); 
        me->delete_temp("cyq-wuji"); 
        me->start_busy(2+random(2)); 
} 
int chunyangwuji(object me, object victim, object weapon, int damage) 
{        
        string msg; 
        int cyq_wuji = me->query_temp("perform/chunyangwuji"); 
        int skill_lvl = me->query_skill("chunyang-quan", 1)/4; 
        int exp_m = me->query("combat_exp", 1);
        int exp_t = victim->query("combat_exp", 1);

        if( damage > 0 ) {
                if( victim->query("qi") <= 0 ) {
                        msg = HIR"\n\t只见$N拳风威猛，当第"+chinese_number(cyq_wuji)+"招发出时，$n已经受不住$N的拳风！\n"NOR;
                        if( me->is_killing(victim->query("id")))
                                victim->receive_wound("qi", victim->query("max_qi"), me);
                        message_vision(msg , me, victim); 
                        finish_chunyangwuji(me, skill_lvl);                       
                        return 1;
                }else msg = HIY"\n\t$N一招命中，不待$n反应第"+chinese_number(cyq_wuji+1)+"招已经以排山倒海之势向$n攻来！\n"NOR;
        }else{
                if( random(exp_m) < random(exp_t / 2) ){
                        msg = HIW"\n\t$N一招不中，顿时乱了自己的招数，纯阳无极的威猛招式再也使不出来！\n"NOR;
                        message_vision(msg , me, victim); 
                        finish_chunyangwuji(me, skill_lvl);                       
                        return 1;
                }else{
                        msg = HIY"\n\t$N一招不中，急转身形将第"+chinese_number(cyq_wuji+1)+"招迅速递出，速度较前快且发力更深！\n"NOR;
                        me->add_temp("apply/damage", skill_lvl / 3);
                }
        }
        if( cyq_wuji >= me->query_temp("cyq-wuji/max_times")){
                msg = HIY"\n\t$N一口气向$n连续攻出"+chinese_number(cyq_wuji)+"招，纯阳拳的威力着实不可被轻视！\n"NOR;
                message_vision(msg , me, victim); 
                finish_chunyangwuji(me, skill_lvl); 
                return 1;
        }
        me->add("neili", -skill_lvl / 3); 
        me->add("jingli", -skill_lvl / 4); 
        me->add_temp("perform/chunyangwuji", 1); 
        me->add_temp("apply/attack", skill_lvl / 2); 
        me->add_temp("apply/damage", -skill_lvl / 2); 
        message_vision(msg , me, victim); 
        COMBAT_D->do_attack(me, victim, me->query_temp("weapon"));
        return 1;
} 

