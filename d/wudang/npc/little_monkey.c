// little_monkey.c

inherit NPC;

void create()
{
	set_name("С��",({ "little monkey", "monkey"}) );
	set("gender", "����");
	set("race", "Ұ��");
	set("age", 1);
	set("long", "����һֻ��Ƥ��С���ӣ�����������ȴϲ��ģ��������\n");
	set("combat_exp", 500);
	set("shen_type", -1);
	set("str", 11);
	set("dex", 11);
	set("con", 11);
	set("int", 11);
        set("attitude", "peaceful");
        setup();
}