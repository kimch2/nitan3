// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.
// san.c  天女散花

#include <ansi.h>
#include <combat.h>
inherit F_SSERVER;
#include <balance.h>

int perform(object me, object target)
{
        object weapon;
        int damage, ap, dp;
        string msg, pmsg;

        if (userp(me) && ! me->query("can_perform/tianyu-qijian/san"))
                return notify_fail("你还不了解这个绝招的运功窍门，无法施展天女散花。\n");
                
        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! target || ! me->is_fighting(target))
                return notify_fail("「天女散花」只能对战斗中的对手使用。\n");

        if (! objectp(weapon = me->query_temp("weapon"))
        || (string)weapon->query("skill_type") != "sword")
                return notify_fail("你使用的武器不对。\n");
                
        if ((int)me->query_skill("tianyu-qijian", 1) < 100)
                return notify_fail("你的天羽奇剑不够娴熟，不会使用。\n");

        if ((int)me->query_skill("xiaowuxiang", 1)<100
                && (int)me->query_skill("bahuang-gong", 1)<100
                && (int)me->query_skill("beiming-shengong", 1)<100)
                return notify_fail("你的逍遥派内功火候不够。\n");

        if (me->query_skill_mapped("force") != "xiaowuxiang"
                && me->query_skill_mapped("force") != "bahuang-gong"
                && me->query_skill_mapped("force") != "beiming-shengong")
                return notify_fail("你没有运用逍遥派内功！\n"); 

        if (me->query("neili") < 300)
                return notify_fail("你现在内力太弱，不能使用「天女散花」。\n");
                        
        msg = CYN "\n$N凝神息气，手腕疾抖，挽出千万个剑花，铺天盖地飞向$n。\n" NOR;
        
        ap = ap_power(me, "sword");
        dp = dp_power(target, "dodge");
        
        if (ap / 2 + random(ap) > dp)
        {
                damage = da_power(me, "sword");
                
                me->add("neili", -200);
                pmsg = HIR "\n只见$N剑花聚为一线，穿向$n。\n$n只觉一股热流穿心而过，喉头一甜，鲜血狂喷而出！\n\n" NOR;

                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 50, pmsg);
                me->start_busy(2);
        } else 
        {
                msg += CYN "可是$p猛地向前一跃,跳出了$P的攻击范围。\n" NOR;
                me->add("neili", -100);
                me->start_busy(3);
        }
        message_combatd(msg, me, target);
        return 1;
}

