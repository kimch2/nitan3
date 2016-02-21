// sanhuang.c  ���Ʊ�����   by sinb

#include <ansi.h>
#include "medicine.h"

void create()
{
        set_name(HIY "����" HIR"������" NOR, ({"baola dan", "dan"}));
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", "����һ�ŻƻƵ�ҩ�裬���䵱�ɵ�����ʥҩ������ޱȡ�\n");
                set("base_unit", "��");
                set("base_value", 50000);
                set("base_weight", 80);
                set("only_do_effect", 1);
                set("mixture", ([
                        "herb" : ([ "niuhuang"   : 1,
                                    "dahuang"   : 1,
                                    "shengdi"  : 1,
                                    "lurong"   : 1,
                                    "xiefen"   : 1,
                                    "honghua"  : 1,
                                    "moyao"    : 1,
                                    "zzfen"    : 1,
                                    "heshouwu" : 1,
                                    "renshen"  : 1,
                                    "dangui"   : 1,
                                    "chenpi"   : 1 ]),
                        "neili" : 300,
                        "jing"  : 50,
                        "time"  : 5,
                        "min_level" : 150,
                        "medical" : 250,
                ]));
        }
        setup();
}

int do_effect(object me)
{
        mapping my;

        if (time() - me->query_temp("last_eat/sanhuang") < 120)
        {
                write("��շ��ù�ҩ����ҩ�Է�����Ч���Ժ���ܼ������á�\n");
                return 1;
        }

        me->set_temp("last_eat/sanhuang", time());

        message_vision(YEL "$N" YEL "����һ��" + name() +
                       YEL "��ͷ��"MAG"����"YEL"������\n", me);
        tell_object(me, YEL "��һ�����ֻ���þ��������������㣬��������֮����\n" NOR);

        my = me->query_entire_dbase();
        my["eff_jing"] = my["max_jing"];
        my["jing"] = my["max_jing"];
        my["eff_qi"] = my["max_qi"];
        my["qi"] = my["max_qi"];
        if (my["jingli"] < my["max_jingli"])
                my["jingli"] = my["max_jingli"];
        if (my["neili"] < my["max_neili"])
                my["neili"] = my["max_neili"];

        me->start_busy(3);

        add_amount(-1); 
        if (query_amount() < 1) 
                destruct(this_object()); 

        return 1;
}
