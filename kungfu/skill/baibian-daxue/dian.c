// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.
// dian.c ���־�

#include <ansi.h>

inherit F_SSERVER;

#include <balance.h> 
 
int perform(object me, object target)
{
        object weapon;
        string msg;
        int ap, dp;
        /*
        if (userp(me) && ! me->query("can_perform/baibian-daxue/dian"))
                return notify_fail("�㻹û���ܹ�����ָ�㣬�޷�ʩչ�����־�����\n");
        */                 
        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! target || ! me->is_fighting(target))
                return notify_fail("�����־���ֻ�ܶ�ս���еĶ���ʹ�á�\n");
 
        if (! objectp(weapon = me->query_temp("weapon")) ||
            (string)weapon->query("skill_type") != "staff")
                return notify_fail("���á����־������б�����ȣ�\n");

        if ((int)me->query_skill("force") < 100)
                return notify_fail("����ڹ���򲻹���\n");

        if ((int)me->query_skill("staff") < 150)
                return notify_fail("����ȷ��������ң��޷�ʹ�á����־�����\n");

        if (! living(target))
              return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        msg = HIG "$N" HIG "���е�" + weapon->name() +
              HIG "��Ȼ��ù����쳣�����������������$n" HIG "�ദ��Ѩ��\n" NOR;

        ap = ap_power(me, "staff");   
        dp = dp_power(target, "dodge");
        
        if (ap / 2 + random(ap) > dp)
        {
                msg += HIY "$n" HIY "��֮�£���æ�ֵ���һʱ����Ͼ������\n" NOR;
                target->start_busy(ap / 1000 + 2);
        } else 
        {
                msg += HIC "����$p" HIC "�����鶯���㿪��$n" HIC "�Ĳ���������" NOR;
                me->start_busy(2);
        }
        message_combatd(msg, me, target);

        return 1;
}
