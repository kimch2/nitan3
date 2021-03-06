#include <ansi.h>

inherit NPC;
inherit F_DEALER;
inherit F_NOCLONE;

mixed ask_me();
mixed ask_me3();

void create()
{
        set_name("南贤", ({ "nan xian", "nan", "xian" }));
        set("long", @LONG
此人衣着青杉，须发皆白。看似其貌不扬，实
却为世上头号人物。这便是武林中号称「南贤
北丑」的南贤。其为人亦正亦邪，年轻时曾干
下了无数轰天动地的大举。生平决战无数，仅
在北丑手下失过手。
LONG);
        set("nickname", HIW "天下至贤" NOR);
        set("title", HIC "武林泰斗" NOR);
        set("gender", "男性" );
        set("age", 74);
        set("attitude", "friendly");
        set("max_jing", 50000);
        set("max_qi", 50000);
        set("max_jingli", 50000);
        set("jingli", 50000);
        set("max_neili", 50000);
        set("neili", 50000);
        set("jiali", 350);
        set("str", 50);
        set("int", 50);
        set("con", 50);
        set("dex", 50);
        set("combat_exp", 12000000);

/*
        set_skill("force", 500);
        set_skill("huntian-baojian", 500);
        set_skill("parry", 500);
        set_skill("dodge", 500);
        set_skill("unarmed", 500);
        set_skill("sword", 500);
        set_skill("lunhui-jian", 500);
        set_skill("poison", 500);
        set_skill("medical", 500);
        set_skill("lingbo-weibu", 500);
        set_skill("qiankun-danuoyi", 500);
        set_skill("buddhism", 500);
        set_skill("taoism", 500);
        set_skill("literate", 500);
        set_skill("martial-cognize", 500);

        map_skill("force", "huntian-baojian");
        map_skill("parry", "qiankun-danuoyi");
        map_skill("unarmed", "huntian-baojian");
        map_skill("dodge", "lingbo-weibu");
        map_skill("sword", "lunhui-jian");

        prepare_skill("unarmed", "huntian-baojian");

        set("chat_chance_combat", 120);
        set("chat_msg_combat", ({
                (: perform_action, "sword.hui" :),
                (: perform_action, "sword.hui" :),
                (: perform_action, "sword.hui" :),
                (: exert_function, "sword" :),
                (: exert_function, "sword" :),
                (: exert_function, "sword" :),
                (: exert_function, "recover" :),
                (: exert_function, "regenerate" :),
                (: exert_function, "powerup" :),
        }));
*/

        set("inquiry", ([
                "name" : "姓名不过是一种代号而已，问它有何意义？",
                "北丑" : "你尽提那个龌龊的家伙做甚？",
                "泥潭" : (: ask_me :),                
                "礼物" : (: ask_me :),  
                "gift" : (: ask_me :),
                "比武": (: ask_me3 :),
                "比武大会": (: ask_me3 :),
                "华山绝顶": (: ask_me3 :),
                "参加比武": (: ask_me3 :),          
        ]));

        setup();
        carry_object("/clone/misc/cloth")->wear();

/*
        if (! clonep(this_object()))
        {
                move("/d/xiakedao/haibin");
                message_vision(WHT "\n突然间你眼前一花，身后似乎多了个"
                               "人影。\n" NOR, this_object());
                set("startroom", "/d/xiakedao/haibin");
        }
*/
        set("startroom", "/d/city/wumiao");
        check_clone(); 
}

// 接受询问
mixed accept_ask(object who, string topic)
{
        // return ULTRA_QUEST_D->accept_ask(this_object(), who, topic);
}

void fight_ob(object ob)
{
        if (is_fighting(ob))
                return;

        command("say 哼！不知死活。");
        command("exert powerup");
        command("exert sword");
        ::fight_ob(ob);
}

int accept_fight(object who)
{
        command("say 没兴趣。");
        return 0;
}

int accept_ansuan(object who)
{
        return notify_fail("你刚想暗算，可是眼前一花，看不太清楚。\n");
}

mixed ask_me()
{
        object me = this_player();
        object ob;
        
        if ((int) me->query("welcomegift"))
        {
                command("say 呵呵～这就是您的不是了！老夫已经给过您礼物了！");
                return 1;
        }
        else
        {
                me->set("welcomegift", 1);
                me->add("food", 5000);
                me->add("water", 10000);
                /*
                me->set("kar", 30);
                me->set("per", 30);
                */
                me->set("tianfu", 4);
                // me->add("potential", 1000);
                
                ob = new("/adm/npc/obj/gift");
                ob->move(me); 

                // ob->move(this_object()); 
                // command("give gift to " + me->query("id"));       
                
        /*
                CHANNEL_D->do_channel(me, "es", sprintf("在下%s今日光临〖泥潭〗（61.128.162.81 : 2001）\n            现已得到南贤的帮助和奖励！即日起开始玩泥巴了！", 
                                      me->name(1)));
        */
                                      
                message_vision(HIW "$N" HIW "凌空一指，突然天空中一道" HIM "七" HIY "彩" HIR "霞" HIC "光" HIW "笼罩$n，转瞬即逝！\n" NOR, 
                               this_object(), me);
                               
                tell_object(me, HIG "南贤送给了你一个礼物（gift），你可以 look gift 来看看是什么礼物。\n" NOR);
                return 1;
        }
}

mixed ask_me3()
{
        object me = this_player();

        tell_object(me, HIY "南贤说道：既然你要参加比武，那么老夫就送你一程！\n" NOR);
        message_vision(HIY "只见南贤伸手一招，一道红光照向$N，$N腾空飞去！\n" NOR, me);
        me->move("/d/huashan/houzhiwangyu");
        return 1;
}
void unconcious()
{
        die();
}

int accept_object(object who, object ob)
{
        return ULTRA_QUEST_D->accept_object(this_object(), who, ob);
}
