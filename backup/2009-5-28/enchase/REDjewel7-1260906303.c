// SN:2Ec`\j;dThe^]jU<
//  Written by ZHANG TIANSHI(Lonely 2007/10/15)	Wed May 27 13:35:33 2009
#include <ansi.h>
inherit ITEM;

int is_enchase_ob(){ return 1; }

void create()
{
	set_name(RED "神之宝石" NOR, ({ "RED jewel7" }));
	set_weight(50);
	set("unit", "块");
	set("long", RED "一块闪烁这奇异光芒的宝石，让人感到一阵阵的心悸。\n" NOR );
	set("value", 200000);
	set("can_be_enchased", 1);
	set("magic/type", "magic");
	set("magic/power", 27);
	set("enchase/SN", 2);
	set("enchase/level", 7);
	set("enchase/cur_firm", 98);
	set("enchase/wspe_data", ([
		"max_jingli" : 4000,
		"ap_power" : 7,
		"da_power" : 7,
		"ap_power" : 7,
		"add_poison" : 23,
		"add_magic" : 23,
		"max_experience" : 400000,
	]));
	set("enchase/aspe_data", ([
		"dp_power" : 7,
		"avoid_fire" : 7,
		"max_jingli" : 4000,
		"unarmed_damage" : 800,
		"avoid_lighting" : 7,
		"avoid_cold" : 7,
		"defense" : 40,
	]));
	set("wear_msg", HIC "$N" HIC "轻轻地把$n"HIC "戴在头上。\n" NOR);
	set("remove_msg", HIC "$N" HIC "轻轻地把$n"HIC "从头上摘了下来。\n" NOR);

	setup();
}

int query_autoload(){ return 1; }
