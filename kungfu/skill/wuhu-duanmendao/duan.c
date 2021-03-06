// by Lonely

#include <ansi.h>
inherit F_SSERVER;
 
int perform(object me, object target)
{
        object weapon, ob;
        string msg;
        int i, skill;
 
        if (! me->is_fighting())
                return notify_fail("五虎断门刀「断」字决只能在战斗中使用。\n");
                
        if (me->query_temp("perform")) 
                return notify_fail("你上一招未使完！\n");
                
        if (! objectp(weapon = me->query_temp("weapon"))
        ||  (string)weapon->query("skill_type") != "blade")
                return notify_fail("你使用的武器不对。\n");
                
        if ((int)me->query("neili") < 200)
                return notify_fail("你的真气不够！\n");
                
        if ((int)me->query_skill("blade") < 60 ||
            me->query_skill_mapped("blade") != "wuhu-duanmendao")
                return notify_fail("你的「五虎断门刀」还不到家，无法使用「断」字决！\n");
                
        msg = HIY "$N猛然伏地，使出「五虎断门刀」断字决，顿时五道白光直向前滚去！\n" NOR;
        message_combatd(msg, me);
        me->clean_up_enemy();
        ob = me->select_opponent();
        me->add("neili", -150);
        me->set_temp("perform", 1);
        skill = me->query_skill("wuhu-duanmendao", 1);
        me->add("apply/attack", skill);
        
        for(i = 0; i < 5; i++)
        if (me->is_fighting(ob) && ob->is_fighting(me) && ob->query("eff_qi") > 0)
        {
              me->set_temp("action_msg", "紧接着"); 
              if (! weapon->query("equipped")) break;
              
              COMBAT_D->do_attack(me, ob,weapon, 0);
        } else break;
          me->add("apply/attack", -skill);
        
        me->start_call_out( (: call_other, __FILE__, "del_perform", me :), 3);;
        return 1;
}

void del_perform(object me)
{
        if (! me) return;
        me->delete_temp("perform");
}


