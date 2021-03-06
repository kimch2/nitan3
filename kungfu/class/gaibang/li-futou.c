// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.

#include <ansi.h>
#include "gaibang.h"

inherit NPC;
inherit F_MASTER;

#include "fight.h"

void create()
{
	set_name("李斧头", ({"li futou", "futou", "li"}));
	set("title", "丐帮六袋弟子");
        set("bunch/name","丐帮");
	set("gender", "男性");
	set("age", 32);
	set("long", 
		"李斧头生性豪爽，擅使大板斧，当着无敌，是丐帮有名的豪杰之士。\n");

	set("attitude", "heroism");
	set("class", "beggar");
	set("str", 30);
	set("int", 18);
	set("con", 28);
	set("dex", 20);
	set("max_qi", 650);
	set("max_jing", 400);
	set("neili",600);
	set("max_neili", 600);
	set("jiali", 50);
	set("level", 7);
	set("combat_exp", 166000);
	
	set_skill("force", 60);
        set_skill("huntian-qigong", 60);
        set_skill("unarmed", 60);
        set_skill("changquan", 60);
        set_skill("dodge", 60);
        set_skill("feiyan-zoubi", 60);
        set_skill("hand", 60);
        set_skill("shexing-diaoshou", 60);
        set_skill("strike", 60);
        set_skill("xiaoyaoyou", 60);
        set_skill("parry", 60);
        set_skill("staff", 60);
        set_skill("fengmo-zhang", 60);
        set_skill("jiaohua-bangfa", 60);
        set_skill("blade", 60);
        set_skill("liuhe-dao", 60);
        set_skill("begging", 60);
        set_skill("checking", 60);
        set_skill("literate", 60);
        set_skill("martial-cognize", 45);
	set_skill("stealing", 60); 
	
        map_skill("force", "huntian-qigong");
        map_skill("hand", "shexing-diaoshou");
        map_skill("blade", "liuhe-dao");
        map_skill("strike", "xiaoyaoyou");
        map_skill("staff", "fengmo-zhang");
        map_skill("parry", "shexing-diaoshou");
        map_skill("dodge", "feiyan-zoubi");
        map_skill("unarmed", "changquan");
	
	prepare_skill("hand", "shexing-diaoshou");
	
	create_family("丐帮", 19, "弟子");
	set("master_ob",1);
	setup();

	carry_object("/clone/cloth/cloth")->wear();
}

void attempt_apprentice(object ob)
{
        string title = ob->query("title");
        int lvl = ob->query("family/beggarlvl");

        if (! permit_recruit(ob))
                return;
        
        if (ob->query("gender") != "男性") 
                return;
                
        if (ob->query("shen") < 0)
        {
                command("say 你身为丐帮弟子，竟然不做好事？");
                return;
        }
        command("nod");
        command("say 我便收你为徒，日后可得行侠仗义，莫损我丐帮名声。");
        command("recruit " + ob->query("id"));

        if (ob->query("class") != "beggar")
                ob->set("class", "beggar");

        if (lvl > 0)
        {
                ob->set("family/beggarlvl", lvl);
                ob->set("title", title);
        }
}
