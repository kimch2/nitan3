#include <ansi.h>
inherit ITEM;
// by dream
int is_enchase_ob() { return 1; }
void create()
{
        set_name(WHT "宝石碎片" NOR, ({ "WHT jewel1","jewel1" }));
        set_weight(20);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", WHT "一片白色的宝石碎片。\n" NOR);
                set("unit", "片");
                set("value", 80000);
                set("no_identify", 1);
                set("wear_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "戴在头上。\n" NOR);
                set("remove_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "从头上摘了下来。\n" NOR);
                    
                set("enchase/type", "all");   // 可镶嵌的道具类型"all"或"sword"或"blade"或"cloth"等
                set("enchase/cur_firm", 90);  // 抗磨损度
                set("enchase/wspe_data",      // 兵器上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性


                        "int" : 1,           // 增加智慧
                        "dex" : 1,           // 增加根骨

                        ////////////////////     兵器特有属性

                        "cold_attack"   : 10, // 冷冻攻击
                        "suck_neili"    : 3, // 吸取内力3
                        "suck_jingli"   : 3, // 吸取精力

                   ]));

                set("enchase/aspe_data",       // 防具上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性

                        "int" : 1,           // 增加智慧
                        "dex" : 1,            // 增加根骨
                        ////////////////////     防具特有属性

                        "reduce_cold"   : 10, // 化解冷冻：10%
                        "defense": 10,       // 防御等级：+10
                        "max_jingli"    : 3, // 精力上限：+3%

                        "jing_recover_speed"   : 5, // 精气恢复速度: +5%
                   ]));
        }
        setup();
}

int query_autoload()
{
        return 1;
}



