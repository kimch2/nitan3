 //tunxiang.c   ����ʽ
//   looo/2001/6/21
#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

int exert(object me, object target)
{
        object weapon;
        string msg;
        int ap, dp;
        int bs;
        int damage;

        if (! target || target == me)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! target || ! me->is_fighting(target))
                return notify_fail(" ����ʽֻ�ܶ�ս���еĶ���ʹ�á�\n");

        if (me->query_temp("weapon") ||
            me->query_temp("secondary_weapon"))
            return notify_fail("�������ֲ���ʹ������ʽ\n");

        if (me->query_skill("force") < 250)
                return notify_fail("����ڹ�����Ϊ����,����óȻʹ������ʽ��\n");

        if( (int)me->query_skill("lamaism",1) < 200 )
                return notify_fail("��������ķ�������óȻʹ������ʽ���»��߻���ħ\n");

        if (me->query_skill("longxiang", 1) < 250)
                return notify_fail("������������������죬����óȻʹ������ʽ��\n");

        if (me->query("max_neili") < 4000)
                return notify_fail("�����������������óȻʹ������ʽ��\n");

        if( (int)me->query("neili", 1) < 1000 )
                 return notify_fail("��û�������ˣ�ʹ������ʽ��������\n");

        if (me->query_skill_mapped("force") != "longxiang")
                return notify_fail("��û�м������������������óȻʹ������ʽ��\n");

        msg = HIY "$N" HIY "����һ�����ޱ�һ�𣬺�Ȼ�������������������ɱ��--����ʽ��\n"
              HIY "\n\nֻ��" HIY "$N" HIY "˫�۶�ʱ����һȦ," HIY "$n" HIY "��ͬ��ǰ��Բ����ȫ�ڡ��������������������֮�£���\n"NOR;

        ap = me->query_skill("longxiang") + me->query("str") * 10;


 if (random(me->query_skill("force")) > target->query_skill("force")/2 || !living(target))
        {
                damage = ap + random(ap)*3/2;
                damage += me->query("jiali");
                bs = 4;
                me->add("neili", -800);
                me->add("neili",-me->query("jiali"));
                me->start_busy(bs);


         msg += COMBAT_D->do_damage(me, target, REMOTE_ATTACK, damage, 150,
             HIR "�������ȭӰ��Ӱ����ķ�����ѹ������" HIR "$n" HIR "����һ�������޷���̣����¼�����Ѫ����\n"NOR);
        } else

        {
           target->add("neili", -200);
                me->add("neili", -600);
           target->start_busy(2);
                me->start_busy(5);
                msg +=HIW "$n" HIW  "���¾��ģ�����ȫ�������࿹������������ײһ����" HIW "$n" HIW "����һ����Ѫ������\n���˳�����վҲվ���ȣ���\n"NOR;
        }
                message_combatd(msg, me, target);

                return 1;
}

