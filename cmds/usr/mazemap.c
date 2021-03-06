// mazemap.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
        int x, y, t;
        string pname;
        string file, str;
        string filename, dir;
        
        if (me->is_busy())
                return notify_fail("你现在正忙。\n");

        if (! wizardp(me) && (time() - me->query_temp("maze/map") < 15))
                return notify_fail("系统气喘嘘地叹道：慢慢来 ....\n");  
        
        if (! environment(me)->query("maze"))
                return notify_fail("这里不是迷宫区域，请用 help here 查看。\n");  
                
        if (me->query("jing") < 5)
                return notify_fail("你现在精神状态不佳，还是等会再查吧。\n");

        me->add("jing", -5);
        me->set_temp("maze/map", time());
        
        dir = MAZE_D->query_maze_dir(me);
        filename = dir + "maze.map";
        write(read_file(filename) + "\n");
        write("地图说明：●表示入口和出口，◎表示迷宫的房间。\n");
        return 1;

        file = base_name(environment(me));
        pname = file;
        
        while (1)
        {
                int idx = strsrch(pname, "/", -1);

                if(idx == -1)
                        return 0;

                if(idx != 0)
                        pname = pname[0..idx-1];

                if(file_size(pname + ".c") >= 0)
                {
                        str = file[idx + 1..];
                        if (str == "entry" || str == "exit")
                        {
                                pname->paint_dynamic_map(str);
                                return 1;
                        }
                        idx = member_array('/', str);
                        
                        if (idx == -1) return 0;
                        
                        if(!sscanf(str[0..idx-1],"%d",x))
                                return 0;
                        if(!sscanf(str[idx+1..],"%d",y))
                                return 0;
                        pname->paint_dynamic_map(x, y);
                        return 1;
                }
                if(! idx)
                        return 0;
        }
        
        return 1;
}


