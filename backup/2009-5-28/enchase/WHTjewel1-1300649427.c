// SN:TnDTlWmHF2C48FIV
//  Written by ZHANG TIANSHI(Lonely 2007/10/15)	Wed May 27 10:19:46 2009
#include <ansi.h>
inherit ITEM;

int is_enchase_ob(){ return 1; }

void create()
{
	set_name(WHT "宝石碎片" NOR, ({ "WHT jewel1" }));
	set_weight(50);
	set("unit", "块");
	set("long", WHT "一片宝石碎片。\n" NOR );
	set("value", 80000);
	set("enchase/SN", 4);
	set("enchase/level", 1);
	set("enchase/cur_firm", 89);
	set("enchase/wspe_data", ([
		"con" : 2,
	]));
	set("enchase/aspe_data", ([
		"effjing_recover" : 20,
	]));
	set("wear_msg", HIC "$N" HIC "轻轻地把$n"HIC "戴在头上。\n" NOR);
	set("remove_msg", HIC "$N" HIC "轻轻地把$n"HIC "从头上摘了下来。\n" NOR);

	setup();
}

int query_autoload(){ return 1; }
