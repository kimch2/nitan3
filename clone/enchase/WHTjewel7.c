#include <ansi.h>
inherit ITEM;
// by dream
int is_enchase_ob() { return 1; }
void create()
{
        set_name(WHT "神之宝石" NOR, ({ "WHT jewel7","jewel7" }));
        set_weight(50);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", WHT "一块闪烁这奇异光芒的白色宝石，"
                    "让人感到一阵阵的心悸。\n" NOR);
                set("unit", "块");
                set("value", 200000);
                set("no_identify", 1);
                set("can_be_enchased", 1);    // 只有最高等级的宝石才有此标志
		set("magic/type", "magic"); 
		set("magic/power", 15 + random(16)); 
		
                set("wear_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "戴在头上。\n" NOR);
                set("remove_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "从头上摘了下来。\n" NOR);
                
                set("enchase/SN", 8);    
                set("magic/cur_firm", 90);  // 抗磨损度
                set("magic/wspe_data",      // 兵器上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性


                        "int" : 10,           // 增加智慧
                        "dex" : 10,           // 增加根骨

                        ////////////////////     兵器特有属性

                        "cold_attack"   : 95, // 冷冻攻击
                        "suck_neili"    : 30, // 吸取内力
                        "suck_jingli"   : 30, // 吸取精力

                   ]));

                set("magic/aspe_data",       // 防具上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性

                        "int" : 10,           // 增加智慧
                        "dex" : 10,            // 增加根骨
                        ////////////////////     防具特有属性

                        "reduce_cold"   : 95, // 化解冷冻：95%
                        "defense": 95,       // 防御等级：+95
                        "max_jingli"    : 30, // 精力上限：+30%

                        "jing_recover_speed"   : 50, // 精气恢复速度: +50%
                        "jingli_recover_speed" : 5, // 精力恢复速度：+5%
                   ]));
        }
        setup();
}

int query_autoload()
{
        return 1;
}

