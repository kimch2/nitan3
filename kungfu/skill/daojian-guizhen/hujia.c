// This is player's own perform (Write by Lonely@nt2)
// Create by 心荻(Xiaot) at Fri Mar 10 11:38:48 2006
// 胡笳十八拍(hujia)

#include <ansi.h>
#include <combat.h>

#define PFM "「" HIW "胡笳十八拍" NOR "」"

inherit F_SSERVER;
#include <balance.h>

string *weapon_sk = ({
        "sword", "blade", "staff", "whip", "club", "hammer", "axe"
});

int perform(object me, object target)
{
        object weapon;
        string msg;
        int ap, dp;
        int count;
        int i, attack_time;

        if (! me->query("can_perform/" + "daojian-guizhen" + "/" + "hujia"))
                return notify_fail("你还没有受到高手指点，还不会运用" PFM "。\n");

        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! target || ! me->is_fighting(target))
                return notify_fail(PFM "只能对战斗中的对手使用。\n");

        if (member_array("sword", weapon_sk) != -1)
        {
                if (! objectp(weapon = me->query_temp("weapon"))
                   || (string)weapon->query("skill_type") != "sword")
                        return notify_fail("你所使用的武器不对，难以施展" PFM "。\n");
        } else
        {
                if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
                        return notify_fail(PFM "只能空手使用。\n");             
        }
        
        if ((int)me->query_skill("daojian-guizhen", 1) < 400)
                return notify_fail("你" + to_chinese("daojian-guizhen") + "不够娴熟，难以施展" PFM "。\n");

        if (member_array("sword", weapon_sk) == -1)
        {
                if (me->query_skill_prepared("sword") != "daojian-guizhen")
                        return notify_fail("你没有准备" + to_chinese("daojian-guizhen") + "，难以施展" PFM "。\n");
        }
        
        if (me->query("neili") < 300)
                return notify_fail("你现在的真气不够，难以施展" PFM "。\n");

        if (! living(target))
                return notify_fail("对方都已经这样了，用不着这么费力吧？\n");

        msg = HIW "$N左手支颐，武器轻轻挥出，长叹一声，脸现寂寥之意，正是一招“胡笳十八拍”。" + "\n" + NOR;

        ap = ap_power(me, "sword");
        dp = dp_power(target, "dodge");
        attack_time = 6;

        if (ap * 2 / 3 + random(ap * 2) > dp)
        {
                msg += HIM "$n已然目惊口呆，哪里能躲得开，身上连中数招！" + "\n" NOR;
                count = ap / 10;
                me->add_temp("apply/attack", count);
                me->add_temp("apply/damage", count);
        } else
        {
                msg += NOR + CYN "可是$p识破了$P这一招，小心应对$P的攻击。" + "\n" NOR;
                count = 0;
        }

        message_sort(msg, me, target);

        attack_time += me->query("jieti/times") * 2;
        if (attack_time > 13)
                attack_time = 13;

        me->add("neili", -attack_time * 20);

        for (i = 0; i < attack_time; i++)
        {
                if (! me->is_fighting(target))
                        break;

                if (random(2) && ! target->is_busy()) target->start_busy(1);

                COMBAT_D->do_attack(me, target, (member_array("sword", weapon_sk) == -1 ? 0 : weapon), 0);
        }
        me->add_temp("apply/attack", -count);
        me->add_temp("apply/damage", -count);
        me->start_busy(3 + random(attack_time/ 3));

        return 1;
}

