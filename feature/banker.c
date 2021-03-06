// banker.c 银行家
// This is a inheritable object.
// Each dealer should support buy, sell, list, value 4 commands
// Updated by Lonely for banghui tax of withdraw

#include <dbase.h>
#include <ansi.h>

#ifndef STORAGE_D
#define STORAGE_D "/adm/daemons/storaged.c"
#endif

#define REBOOT_CMD      "/cmds/arch/reboot" 

int do_check()
{
        // here we use 3 units to display bank infos
        int tax;
        mixed total;
        
        total = this_player()->query("balance");
        
        tax = (int)environment(this_object())->query("bunch/tax");
                
        if (intp(tax) && tax > 0 && tax < 11)
                write("敝商号的取钱的手续费为百分之" + chinese_number(tax) + "。\n");       
        else
                write("敝商号的取钱的手续费为零。\n");      

#ifdef LONELY_IMPROVED        
        if (! total || count_lt(total, 0))
#else
        if (! total || total < 0)
#endif
        {
                this_player()->set("balance", 0);
                write("您在敝商号没有存钱。\n");
                return 1;
        }
        write(this_object()->query("name")+"悄悄告诉你：您在弊商号共存有" + 
              MONEY_D->money_str(total) + "。\n");
              
        return 1;                                                               
}

int do_convert(string arg)
{
        string from, to;
        int amount, bv1, bv2, bv;
        object from_ob, to_ob;
        object me;

        me = this_player();                                                     

        if (me->is_busy())
                return notify_fail("你还是等有空了再说吧！\n");

        if (this_object()->is_fighting())
                return notify_fail("哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");

        if (! arg || sscanf(arg, "%d %s to %s", amount, from, to) != 3)
        {
                return notify_fail("命令格式：convert|duihuan <数量> <货币单位> to <新货币单位>\n");
        }

        from_ob = present(from + "_money", me);
        to_ob = present(to + "_money", me);

        if (! to_ob && file_size("/clone/money/" + to + ".c") < 0)
                return notify_fail("你想兑换成什么？\n");

        if (! from_ob)
                return notify_fail("你身上没有带这种钱。\n");

        if (from_ob == to_ob)
                return notify_fail("你有毛病啊？\n");

        if (amount < 1)
                return notify_fail("你想白赚啊？\n");

        if ((int)from_ob->query_amount() < amount)
                return notify_fail("你带的" + from_ob->query("name") +
                                   "不够。\n");

        bv1 = from_ob->query("base_value");
        if (! bv1)
                return notify_fail("这样东西不值钱。\n");

        bv2 = to_ob ? to_ob->query("base_value")
                    : call_other("/clone/money/" + to, "query", "base_value");
        if (bv1 < bv2)  amount -= amount % (bv2 / bv1);
        if (amount == 0)
                return notify_fail("这些" + from_ob->query("name") +
                                   "不够换。\n ");

        if (bv1 > bv2)
        {
                bv = bv1 / bv2;
                if (bv * amount > 10000 || bv * amount / amount != bv)
                        return notify_fail("哦呦...我这一下子还拿不出这么多散钱...\n");
        }

        // allowed to convert now
        me->start_busy(1);

        from_ob->add_amount(-amount);

        if (! to_ob)
        {
                to_ob = new("/clone/money/" + to);
                to_ob->set_amount(atoi(count_div(count_mul(amount, bv1), bv2)));
                to_ob->move(me, 1);
        }
        else
                to_ob->add_amount(atoi(count_div(count_mul(amount, bv1), bv2)));

        message_vision(sprintf("$N从身上取出%s%s%s，换成了%s%s%s。\n",
                               chinese_number(amount),
                               from_ob->query("base_unit"),
                               from_ob->query("name"),
                               chinese_number(atoi(count_div(count_mul(amount, bv1), bv2))),
                               to_ob->query("base_unit"),
                               to_ob->query("name")), me);

        return 1;
}                                                                               

int do_deposit(string arg)                                                      
{
        string what;
        int amount;
        object what_ob, me;

        me = this_player();

        if (me->is_busy())
                return notify_fail("你还是等有空了再说吧！\n");

        if (this_object()->is_fighting())
                return notify_fail("哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");

        if (! arg || sscanf(arg, "%d %s", amount, what) != 2)
                return notify_fail("命令格式：deposit:cun <数量> <货币单位>\n");

        what_ob = present(what + "_money", me);
        if (! what_ob)
                return notify_fail("你身上没有带这种钱。\n");

        if (amount < 1)
                return notify_fail("你想存多少" + what_ob->query("name") +
                                   "？\n");

        if ((int)what_ob->query_amount() < amount)
                return notify_fail("你带的" + what_ob->query("name") +
                                   "不够。\n");

        // deposit it
        me->start_busy(1);
        
#ifdef LONELY_IMPROVED
        me->add("balance", count_mul(what_ob->query("base_value"), amount));
#else 
        me->add("balance", what_ob->query("base_value") * amount);
#endif
        what_ob->add_amount(-amount);
        message_vision(sprintf("$N拿出%s%s%s，存进了银号。\n",
                               chinese_number(amount), what_ob->query("base_unit"),
        what_ob->query("name")), me);

        return 1;
}


int do_withdraw(string arg)
{
        string what, banghui;
        int amount;
        object what_ob, me, area;
        mixed v, s, tax; 

        me = this_player();

        if (me->is_busy())
                return notify_fail("你还是等有空了再说吧！\n");

        if (this_object()->is_fighting())
                return notify_fail("哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");

        if (!arg || sscanf(arg, "%d %s", amount, what) != 2)
                return notify_fail("命令格式：withdraw|qu <数量> <货币单位>\n");

        if (amount < 1)
                return notify_fail("你想取出多少钱？\n");

        if(amount >= 10000)
                return notify_fail("这么大的数目。本店没这么多零散现金。\n");

        if (file_size("/clone/money/" + what + ".c") < 0)
                return notify_fail("你想取出什么钱？\n");

        what = "/clone/money/" + what;
        v = count_mul(amount, what->query("base_value"));
        
        area = environment(this_object());
        if (! area->is_area_room() || ! stringp(area->query("bunch/bunch_name")) ||
            area->query("bunch/npc_file") != base_name(this_object()) ||
            ! intp(area->query("bunch/tax")) || (int)area->query("bunch/tax") < 1 ||     
            (int)area->query("bunch/tax") > 10)     
                // tax = v * 1 / 10;
                tax = 0;
        else
                tax = atoi(count_div(count_mul(v, (int)area->query("bunch/tax")), 100));
                
        banghui = (string)this_player()->query("bunch/bunch_name");
        if(! stringp(banghui) || banghui != (string)this_object()->query("bunch/bunch_name")) 
               s = count_add(v, tax);
        else
               s = v;
               
#ifdef LONELY_IMPROVED
        if (count_gt(s, me->query("balance")) || count_le(v, 0))
#else
        if (s > me->query("balance") || v <= 0)
#endif
               return notify_fail("你存的钱不够取。\n");

        me->start_busy(1);

        me->set("balance", count_sub(me->query("balance"), s));

        if (s > v && stringp(banghui = this_object()->query("bunch/bunch_name")))
               BUNCH_D->add_info_into_bunch(banghui, "npc_money", tax);
               
        if (what == "/clone/money/cash" ||
            what == "/clone/money/thousand-gold")
        {
                what_ob = new(what);
                what_ob->set_amount(amount);
                what_ob->move(me, 1);

                message_vision(sprintf("$N从银号里取出%s%s%s。\n", 
                               chinese_number(amount), 
                               what_ob->query("base_unit"),
                               what_ob->name(1)), me);

                return 1;
        }

        MONEY_D->pay_player(me, v);
        message_vision(sprintf("$N从银号里取出%s。\n", MONEY_D->money_str(v)), me);
        
        return 1;
}

int do_transfer(string arg)
{
        string what, banghui, who;
        string v, s;
        object me, ob, area;
        int amount;
        mixed tax;

        me = this_player();

        if (me->is_busy())
                return notify_fail("你还是等有空了再说吧！\n");

        if (this_object()->is_fighting())
                return notify_fail("哟，抱歉啊，我这儿正忙着呢……您请稍候。\n");
        
        if (REBOOT_CMD->is_rebooting()) 
                return notify_fail("现在本站正在准备重新启动，本钱庄暂停转帐功能。\n");
                
        if (!arg || sscanf(arg, "%d %s to %s", amount, what, who) != 3)
                return notify_fail("命令格式：transfer|zhuanzhang <数量> <货币单位> to <某人>\n");
        
        if (amount < 1)
                return notify_fail("你想转帐多少钱？\n");

        if(amount > 1000000)
                return notify_fail("这么大的数目，有洗钱的嫌疑，本店可不敢犯法。\n");

        if (file_size("/clone/money/" + what + ".c") < 0)
                return notify_fail("你想转帐钱的单位是？\n");
        
        if (! (ob = find_player(who)))
                return notify_fail("你想转帐给的人不存在，或者不在线上！\n");

        what = "/clone/money/" + what;
        v = count_mul(amount, what->query("base_value"));
        
        area = environment(this_object());
        if (! area->is_area_room() || ! stringp(area->query("bunch/bunch_name")) ||
            area->query("bunch/npc_file") != base_name(this_object()) ||
            ! intp(area->query("bunch/tax")) || (int)area->query("bunch/tax") < 1 ||     
            (int)area->query("bunch/tax") > 10)     
                // tax = v * 1 / 10;
                tax = 0;
        else
                tax = atoi(count_div(count_mul(v, (int)area->query("bunch/tax")), 100));
                
        banghui = (string)this_player()->query("bunch/bunch_name");
        if(! stringp(banghui) || banghui != (string)this_object()->query("bunch/bunch_name")) 
               s = count_add(v, tax);
        else
               s = v;
               
#ifdef LONELY_IMPROVED
        if (count_gt(s, me->query("balance")))
#else
        if (s > me->query("balance") || v <= 0)
#endif
               return notify_fail("你存的钱不够取。\n");

        me->start_busy(1);

        me->set("balance", count_sub(me->query("balance"), s));
        ob->set("balance", count_add(ob->query("balance"), s));

        if (count_gt(s, v) && stringp(banghui = this_object()->query("bunch/bunch_name")))
               BUNCH_D->add_info_into_bunch(banghui, "npc_money", tax);
         
        tell_object(ob, "\a", 0);       
        tell_object(ob, HIR + me->query_idname(1) + HIR "从银号里划转"+ MONEY_D->money_str(v) + "到你的帐户上。\n" NOR);
        tell_object(me, HIR "你从银号里划转"+ MONEY_D->money_str(v) + "到" + ob->query_idname(1) + HIR "的帐户上。\n" NOR);

        return 1;
}
        
int do_tax(string arg)
{
        object me, area;
        string bunch;
        int tax;
        
        me = this_player();
        
        if (! arg || sscanf(arg, "%d", tax) != 1) 
               return notify_fail("你要设定百分之多少的取款手续费？\n"); 

        if (! intp(tax) || tax < 1 || tax > 10)
                return notify_fail("设定取款手续费要介于百分之一和百分之十之间。\n");
                
        if (! stringp(bunch = me->query("bunch/bunch_name")) ||
            bunch != this_object()->query("bunch/bunch_name"))
                return notify_fail("敝商号没有被你控制，你无权设定取款手续费。\n");
                
        if (me->query("bunch/level") != 9)
                return notify_fail("只有帮主才有权利设定这里的取款手续费。\n");

        area = environment(this_object());
        
        if (! area->is_area_room() || ! stringp(area->query("bunch/bunch_name")) ||
            area->query("bunch/npc_file") != base_name(this_object()))
                return notify_fail("设定取款手续费失败。\n");                
                          
        area->set("bunch/tax", tax);
        area->save();
        
        write("你设定敝商号的取钱的手续费为百分之" + chinese_number(tax) + "成功。\n");   
        return 1;
}
        
int do_bcheck(string arg)
{
        object me;
        string str;

        me = this_player();
        if (! me->query("bunch/bunch_name"))
                return notify_fail("你还没有加入任何帮派，查什么帐户？\n");

        if (me->query("bunch/level") < 5)
                return notify_fail("你在帮派中的地位太低，无权查询本帮帐户！\n");

        if (! arg)
                str = BUNCH_D->count_query(me);
        else
                str = BUNCH_D->count_query(me, arg);

        tell_object(me, str);
        return 1;
}

//储物柜业务
int do_cw(string arg)
{
        string cmd;
        object me=this_player();
        if(!arg||arg=="")
        {
                write("请使用cw help来查询使用帮助。\n");
                return 1;
        }
        arg=lower_case(arg);
        if(arg=="help"||arg=="cancel"||arg=="list")
                cmd=arg;
        else if(sscanf(arg,"%s %s",cmd,arg)!=2)
        {
                //dohelp2
                write("请使用cw help来查询使用帮助。\n");
                return 1;
        }
        switch(cmd)
        {
        case "open":    //开帐户
                STORAGE_D->OpenAccount(me,arg);
                break;
        case "put":
                STORAGE_D->PutObject(me,arg);
                break;
        case "get":
                STORAGE_D->GetObject(me,arg);
                break;
        case "fee":
                STORAGE_D->DoFee(me,arg);
                break;
        case "list":
                STORAGE_D->ListObject(me);
                break;
        case "cancel":
                STORAGE_D->CancelAccount(me);
                break;
        case "help":
                STORAGE_D->ShowHelp();
                break;
        default:
                write("你想要做什么啊？还是使用cw help来查看一下帮助吧！\n");
        }
        return 1;
}
