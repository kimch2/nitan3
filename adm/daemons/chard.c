// chard.c
// From ES2
// Modified by Xiang@XKX
// Updated by Doing

#pragma optimize
#pragma save_binary

#define HUMAN_RACE "/adm/daemons/race/human"
#define BEAST_RACE "/adm/daemons/race/beast"
#define MONSTER_RACE "/adm/daemons/race/monster"
#define STOCK_RACE "/adm/daemons/race/stock"
#define BIRD_RACE "/adm/daemons/race/bird"
#define FISH_RACE "/adm/daemons/race/fish"
#define SNAKE_RACE "/adm/daemons/race/snake"
#define INSECT_RACE "/adm/daemons/race/insect"
 
void create() { seteuid(getuid()); }
 
void setup_char(object ob)
{
        string race;
        mapping my;
 
        if (! stringp(race = ob->query("race")))
        {
                race = "人类";
                ob->set("race", "人类");
        }

        my = ob->query_entire_dbase();

        switch(race)
        {
        case "人类":
                HUMAN_RACE->setup_human(ob);
                break;        
        case "妖魔":
                MONSTER_RACE->setup_monster(ob);
                break;        
        case "野兽":
                BEAST_RACE->setup_beast(ob);
                break;        
        case "家畜":
                STOCK_RACE->setup_stock(ob);
                break;
        case "飞禽":
                BIRD_RACE->setup_bird(ob);
                break;
        case "游鱼":
                FISH_RACE->setup_fish(ob);
                break;
        case "蛇类":
                SNAKE_RACE->setup_snake(ob);
                break;
        case "昆虫":
                INSECT_RACE->setup_insect(ob);
                break;
        default: 
                error("Chard: undefined race " + race + ".\n");
        }
 
        if (undefinedp(my["jing"]))
                my["jing"] = my["max_jing"];
        if (undefinedp(my["qi"]))
                my["qi"] = my["max_qi"];
 
        if (undefinedp(my["eff_jing"]) || my["eff_jing"] > my["max_jing"])
                my["eff_jing"] = my["max_jing"];
        if (undefinedp(my["eff_qi"]) || my["eff_qi"] > my["max_qi"])
                my["eff_qi"] = my["max_qi"];
 
        if (undefinedp(my["shen_type"])) my["shen_type"] = 0;
 
        if (undefinedp(my["shen"]))
        {
                if (userp(ob))        
                        my["shen"] = 0;
                else
                {
                        mixed n;
                        n = count_mul(my["shen_type"], count_div(my["combat_exp"], 10)); 
                        // sscanf(n, "%d", n);
                        my["shen"] = atoi(n);
                } 
        }
 
        if (! ob->query_max_encumbrance())
                ob->set_max_encumbrance(40000 + ob->query("str") * 4000 + ob->query_str() * 600);
 
        ob->reset_action();
        ob->update_killer();
}
 
varargs object make_corpse(object victim, object killer)
{
        int i;
        object corpse, *inv;

        if (victim->is_ghost())
        {
                inv = all_inventory(victim);
                inv->owner_is_killed(killer);
                inv -= ({ 0 });
                i = sizeof(inv);
                while(i--) inv[i]->move(environment(victim));
                return 0;
        }

        corpse = new(CORPSE_OB);
        corpse->make_corpse(victim, killer);
        return corpse;
}
