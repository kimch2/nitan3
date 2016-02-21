// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.

#include <ansi.h>
#include <combat.h>
 
inherit F_SSERVER;
#include <balance.h>

#define HUN "��" HIR "����һ��" NOR "��"

int perform(object me, object target)
{
        string msg;
        int ap, dp;
        int damage;

        if (userp(me) && ! me->query("can_perform/jiuyang-shengong/hun"))
                return notify_fail("�㻹û���ܹ�����ָ�㣬�޷�ʩչ" HUN "��\n");

        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! target || ! me->is_fighting(target))
                return notify_fail(HUN "ֻ�ܶ�ս���еĶ���ʹ�á�\n");
 
        if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
                return notify_fail(HUN "ֻ�ܿ���ʩչ��\n");

        if (me->query_skill("jiuyang-shengong", 1) < 180)
                return notify_fail("��ľ����񹦻�������죬����ʩչ" HUN "��\n");

        if (me->query_skill_mapped("unarmed") != "jiuyang-shengong")
                return notify_fail("������û�м���������Ϊȭ�ţ�����ʩչ" HUN "��\n");

        if (me->query_skill_mapped("force") != "jiuyang-shengong")
                return notify_fail("������û�м���������Ϊ�ڹ�������ʩչ" HUN "��\n");

        if (me->query_skill_prepared("unarmed") != "jiuyang-shengong")
                return notify_fail("������û��׼��ʹ�þ����񹦣�����ʩչ" HUN "��\n");

        if (me->query("neili") < 300)
                return notify_fail("�����������������ʩչ" HUN "��\n");
 
        if (! living(target))
                return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        msg = HIR "$N" HIR "��ǰһ����˫�ֻ�Ȧ���ĵ�̫��֮�⡣���Ķ�ʱ��"
              "��һ�����ţ���$n" HIR "�����ȥ��\n" NOR;
 
        ap = ap_power(me, "force") + me->query("con") * 10;
        dp = dp_power(target, "force") + target->query("con") * 10;

        me->add("neili", -50);

        if (ap / 2 + random(ap) < dp)
        {
                msg += HIY "Ȼ��$n" HIY "ȫ���ֵ������ڽ�$N" HIY
                       "���������Ų�����\n" NOR;
                me->start_busy(3);
        } else
        {
                me->add("neili", -200);
                me->start_busy(1);
                damage = da_power(me, "force");
                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 50,
                                           HIR "$n" HIR "��æ�������ˣ�����������"
                                           "�ø��죬ֻ��$p" HIR "һ���ҽУ���Ѫ��"
                                           "����\n" NOR);
        }
        message_combatd(msg, me, target);
        return 1;
}
