/************************************************
 * vrm_server.c                                 *
 * 虚拟的随机迷宫创造器 (Virtual Random Maze)   *
 *                                              *
 * by Find.                                     *
 * updated by Lonely                            *
 ************************************************/

/******************************************************
 * 迷宫是一个游戏里经常要用到的东西，以前的迷宫都需要 *
 * 巫师一个房间一个房间地手工绘制，费时费力，而且一旦 *
 * 被玩家找出正确的线路，迷宫格局被泄漏，迷宫就不称其 *
 * 为迷宫了，所以巫师们都绞尽脑汁把迷宫设计的尽量复杂,*
 * 但再复杂的迷宫早晚也会被找到正确的路线，而且过于复 *
 * 杂难走的迷宫也使玩家感觉过于繁琐，降低乐趣。因此产 *
 * 生此想法。                                         *
 * 随机迷宫的产生算法尽量简单，迷宫的储存尽量节省记忆 *
 * 体，迷宫房间采用虚拟物件，处理灵活，迷宫房间只有在 *
 * 玩家走到时才会装进内存，而且迷宫房间也象普通的ROOM *
 * 一样当一段时间没有被参考到可以销毁节省记忆体，当整 *
 * 个迷宫一段时间没有被参考到可以被完全摧毁，下次再需 *
 * 要的时候会重新建立，又会产生一个新的迷宫。区域巫师 *
 * 写作随机迷宫只需规定一些预设的参数如迷宫的单边长、 *
 * 房间描述、出入口描述，几十个乃至几千个房间、路线时 *
 * 时不同的随机迷宫就建立好了，大大提高了区域写作效率 *
 * 和游戏的可玩性。                                   *
 * 此物件目前适合于随机的迷宫，即：迷宫内房间的描述基 *
 * 本相同，比如一片树林、一片坟地等，如要此物件创作完 *
 * 整的随机区域即有一定的情节、一定格局的区域，则需要 *
 * 根据自己的情况规定出迷宫内房间描述的一些规则，使相 *
 * 邻房间的描述变化合理，房间内物件与描述协调。如果愿 *
 * 意巫师可以只制作迷宫间的连接部分，而用几个迷宫组合 *
 * 成一个完全随机的区域，哈，那以后做巫师可轻松多了。 *
 * 目前本游戏使用的迷宫一般为单边长10至40，到底能做多 *
 * 大的迷宫我也不知道，下面对此有一个说明，要根据自己 *
 * 的服务器性能来讲，不过我想最普通的机器制作一个面积 *
 * 为100x100的迷宫应该也是一件轻松的事情。            *
 * 由于采用 virtual object，牵涉到一点安全问题，需要根*
 * 据自己的系统考量调整。                             *
 ******************************************************/

#pragma optimize

#define N               8
#define S               4
#define W               2
#define E               1
#define ALL             15

/****************************************************
 * 迷宫的单边长最大值目前暂定为 100，由于随机迷宫的
 * 创造和操作极其耗费资源单边长 100 的迷宫'面积'就是
 * 100x100 等于 10000 个房间的一个迷宫，一般恐怕是用
 * 不到。一般的实时迷宫（实时迷宫是指在游戏运行过程
 * 中随时被 destruct 随着需要又会随时被创建的迷宫）的
 * 单边长以 10 到 50 之间为宜。如需创造巨型迷宫如有几
 * 万乃至十几万个房间的迷宫，应将创建工作放置于游戏启
 * 动时做，游戏启动的一段时间（比如20秒）禁止玩家登入。
 * 游戏通过定期重新启动来更新此迷宫。
 * 不知谁会用到这么大的迷宫。。。。。。
 ****************************************************/
#define MAX_LONG        100

// 只要能与其他房间相连的房间就肯定有一个入口.
// 而可能的出口有三个.
// define 这项规定房间最多只能有两个出口.
// 也就是对于有三个出口的房间会随机关闭一个.
// 这样减小了计算量,但迷宫复杂程度也相应降低了.
#define TWO_VALID_LEAVES

#define PAINT_MAP

#include <ansi.h>
inherit F_CLEAN_UP;
inherit F_DBASE;

class coordinate{ int x; int y; }
class coordinate *newpath = ({}),/*待处理队列*/
        enter,/* 入口坐标 */
        leave;/* 出口坐标 */

nosave string *valid_dirs = ({ "south","north","west","east" });
nosave mapping reverse_dir = ([
"north" : "south",
"south" : "north",
"west"  : "east",
"east"  : "west",
]);

// 全迷宫出口阵列.
nosave mixed *all;

/***************** 迷宫的一些预设特性：*****************/
nosave int l;                           // 迷宫的单边长
nosave string *inherit_rooms = ({});    // 迷宫允许继承的档案名称
nosave string *valid_rooms = ({});      // 迷宫可使用的房间文件名 (****)
nosave string entry_dir;                // 迷宫入口方向
nosave string link_entry_dir;           // 迷宫入口与区域的连接方向
nosave string link_entry_room;          // 迷宫入口所连接区域档案的文件名
private int link_entry_room_x;          // 迷宫入口的x坐标
private int link_entry_room_y;          // 迷宫入口的y坐标
private int link_entry_room_z;          // 迷宫入口的z坐标
nosave string link_exit_dir;            // 迷宫出口与区域的连接方向
nosave string link_exit_room;           // 迷宫出口所连接区域档案的文件名
nosave string entry_short;              // 迷宫入口的短描述
nosave string entry_desc;               // 迷宫入口的长描述
nosave string exit_short;               // 迷宫出口的短描述
nosave string exit_desc;                // 迷宫出口的长描述
nosave string *maze_room_desc   = ({}); // 迷宫房间的长描述
nosave string maze_room_short;          // 迷宫房间的短描述
nosave int is_outdoors = 0;             // 迷宫房间是否为户外
nosave mixed maze_npcs;                 // 迷宫中的怪物
nosave mixed entry_npcs;                // 迷宫入口的怪物
nosave mixed exit_npcs;                 // 迷宫出口的怪物
nosave string *unique_rooms = ({});;    // 迷宫唯一的房间
private int refresh_delay = 0;          // 迷宫重建时间, default is 0 means no recreation
nosave int is_nodeath_room = 0;
/******************* ---- END ---- *********************/

// 建立标记.
nosave int maze_built = 0;

// 迷宫房间来源转换开关
nosave int switch_flag = 0;

// 重置全域变量.
protected void refresh_vars();

// 建立迷宫
void create_maze();

// 选择随机出口.
protected int random_out(int x,int y,int n);

// 处理连接.
protected void link_to_north(int x,int y);
protected void link_to_south(int x,int y);
protected void link_to_west(int x,int y);
protected void link_to_east(int x,int y);

private int player_in(object maze); 

// 绘制已建成迷宫的地图.
void paint_vrm_map();
nomask object query_maze_room(string str);

protected string mroom_fname(int x,int y)
{ return sprintf("%s/%d/%d",base_name(this_object()),x,y);}


protected void refresh_vars() // 重置全域变量.
{
        newpath = ({});
        all = 0;
}

// 对一些必设参数的合法性检查
protected int check_vars()
{
        int i,n;

        if( (l < 5) || l > MAX_LONG )
                return 0;

        if(!switch_flag)
        {
                if(!stringp(entry_dir) || (member_array(entry_dir,valid_dirs) == -1) )
                        return 0;

                maze_room_desc -=({0});
                if( !n = sizeof(maze_room_desc) )
                        return 0;

                for(i=0;i<n;i++)
                        if(!stringp(maze_room_desc[i]) || (maze_room_desc[i] == ""))
                                return 0;

                if(!stringp(maze_room_short) || (maze_room_short == ""))
                        return 0;
        }

        else
        {
                valid_rooms -=({0});
                if( !n = sizeof(valid_rooms) )
                        return 0;
                for(i=0;i<n;i++)
                        if(!stringp(valid_rooms[i]) || (valid_rooms[i] == ""))
                                return 0;
        }
        /*
        if(!stringp(link_entry_dir) || (member_array(link_entry_dir,valid_dirs) == -1) )
                return 0;

        if(!stringp(link_exit_dir) || (member_array(link_exit_dir,valid_dirs) == -1) )
                return 0;
        */

        inherit_rooms -=({0});
        if( !n = sizeof(inherit_rooms) )
                return 0;

        for(i=0;i<n;i++)
                if(!stringp(inherit_rooms[i]) || (inherit_rooms[i] == ""))
                        return 0;

        if(!stringp(link_entry_room) || (link_entry_room == ""))
                return 0;

        if(!stringp(link_exit_room) || (link_exit_room == ""))
                return 0;

        if(!stringp(entry_short) || (entry_short == ""))
                return 0;

        if(!stringp(exit_short) || (exit_short == ""))
                return 0;

        if(!stringp(entry_desc) || (entry_desc == ""))
                return 0;

        if(!stringp(exit_desc) || (exit_desc == ""))
                return 0;

        return 1;
}

protected int random_out(int x,int y,int n) // 选择随机出口函数.
{
        int *outs = ({}), retn = 0;
        class coordinate temp;

        // The west room is (x-1,y)
        if( n&W
        && ((x-1) >= 0)
        && !all[x-1][y] )
        {
                temp = new(class coordinate);
                temp->x = x-1;
                temp->y = y;

                // 西面的房间不在待处理列表 newpath 中.
                //if( member_array(temp,newpath) == -1 )
                        outs += ({ W });
        }

        // The east room is (x+1,y)
        if( n&E
        && ((x+1) < l)
        && !all[x+1][y] )
        {
                temp = new(class coordinate);
                temp->x = x+1;
                temp->y = y;

                // 东面的房间不在待处理列表 newpath 中.
                //if( member_array(temp,newpath) == -1 )
                        outs += ({ E });
        }

        // The south room is (x,y-1)
        if( n&S
        && ((y-1) >= 0)
        && !all[x][y-1] )
        {
                temp = new(class coordinate);
                temp->x = x;
                temp->y = y-1;

                // 南面的房间不在待处理列表 newpath 中.
                //if( member_array(temp,newpath) == -1 )
                        outs += ({ S });
        }

        // The north room is (x,y+1)
        if( n&N
        && ((y+1) < l)
        && !all[x][y+1] )
        {
                temp = new(class coordinate);
                temp->x = x;
                temp->y = y+1;

                // 北面的房间不在待处理列表 newpath 中.
                //if( member_array(temp,newpath) == -1 )
                        outs += ({ N });
        }

#ifdef TWO_VALID_LEAVES
        // 如果有三个出口,随机关闭一个.
        if(sizeof(outs) >= 3)
                outs -= ({ outs[random(sizeof(outs))] });
#endif

        for(int i=0;i<sizeof(outs);i++)
                retn |= outs[i];

        return retn;
}

void create_maze()
{
        int i;
        class coordinate *valid_leaves=({}),temp;

        refresh_vars(); // 重置全域变量.
        if( !check_vars() )   // 对一些预设变量进行检查。
                return;

        // 1.确定迷宫单边长.
        all = allocate(l);
        for(i=0;i<l;i++)
                all[i] = allocate(l);   // 建立数组.
                
        enter = new(class coordinate);

        switch (entry_dir)
        {
                case "south":
                        // enter 入口坐标.
                        enter->x = to_int(l/2); // 取中迷宫比较平衡。
                        enter->y = 0;
                        all[enter->x][enter->y] |= S;
                        break;
                case "north":
                        enter->x = to_int(l/2);
                        enter->y = l-1;
                        all[enter->x][enter->y] |= N;
                        break;
                case "west":
                        enter->y = to_int(l/2);
                        enter->x = 0;
                        all[enter->x][enter->y] |= W;
                        break;
                case "east":
                        enter->y = to_int(l/2);
                        enter->x = l-1;
                        all[enter->x][enter->y] |= E;
                        break;
        }

        // 存入待处理队列.
        newpath += ({ enter });

        // 进入主循环.
        do
        {
                int x,y,out,numb;

                // 进行一些监测与初始化.
                if( !(numb=sizeof(newpath)) )
                        continue;
                numb = random(numb);
                reset_eval_cost();
                x = newpath[numb]->x;
                y = newpath[numb]->y;

                // 如果有三个可能的出口随机关闭一个出口:
                out = ALL^(all[x][y]);
                out = random_out(x,y,out);

                if(!out) // 没有可能的出口了.
                {
                        newpath -= ({ newpath[numb] });
                        continue;
                }

                // 处理连接.
                if(out&W) link_to_west(x,y);
                if(out&E) link_to_east(x,y);
                if(out&N) link_to_north(x,y);
                if(out&S) link_to_south(x,y);

                // 当前房间处理完毕.
                newpath -= ({ newpath[numb] });
        }
        while (sizeof(newpath));

        switch (entry_dir)
        {
                case "west":
                        for(i=0;i<l;i++)
                                if(all[l-1][i])
                                {
                                        temp = new(class coordinate);
                                        temp->x = l-1;
                                        temp->y = i;
                                        valid_leaves += ({temp});
                                }
                        break;
                case "east":
                        for(i=0;i<l;i++)
                                if(all[0][i])
                                {
                                        temp = new(class coordinate);
                                        temp->x = 0;
                                        temp->y = i;
                                        valid_leaves += ({temp});
                                }
                        break;
                case "south":
                        for(i=0;i<l;i++)
                                if(all[i][l-1])
                                {
                                        temp = new(class coordinate);
                                        temp->x = i;
                                        temp->y = l-1;
                                        valid_leaves += ({temp});
                                }
                        break;
                case "north":
                        for(i=0;i<l;i++)
                                if(all[i][0])
                                {
                                        temp = new(class coordinate);
                                        temp->x = i;
                                        temp->y = 0;
                                        valid_leaves += ({temp});
                                }
                        break;
        }

        if( !(i=sizeof(valid_leaves)) ) // 没有出口 须重新建立
        {
                //log_file("zvall",sprintf("%O\n",all));
                call_other(this_object(),"create_maze");
                return;
        }

        if(i == 1)
                leave = valid_leaves[0];
        else
                leave = valid_leaves[random(i)]; // 随机选一个.

        switch (entry_dir)
        {
                case "south":
                        all[leave->x][leave->y] |= N;
                        break;
                case "north":
                        all[leave->x][leave->y] |= S;
                        break;
                case "west":
                        all[leave->x][leave->y] |= E;
                        break;
                case "east":
                        all[leave->x][leave->y] |= W;
                        break;
        }

        // 迷宫创建完毕。
        maze_built = 1;

        if( refresh_delay > 0 )
                SCHEDULE_D->set_event(refresh_delay, 1, this_object(), "refresh_maze"); 

        // 绘制完成的迷宫地图。
        // 地图文件为同目录下同名的'.map' 文件，
        // 绘制地图也许可利于区域巫师的工作。
        // 如需要可开放物件对于本目录的'写'。
#ifdef PAINT_MAP
        paint_vrm_map();
#endif
}

protected void link_to_west(int x,int y)        // The west room is (x-1,y)
{
        class coordinate temp;
        // can't link. 当前房间已经是最西面的房间了.
        if( (x-1) < 0 )
                return;

        temp = new(class coordinate);
        temp->x = x-1;
        temp->y = y;

        // 西面的房间已经于 path 中,或者 已在待处理列表 newpath 中.
        if(all[temp->x][temp->y] /*|| member_array(temp,newpath)*/)
                return;

        all[x][y] |= W;
        all[temp->x][temp->y] |= E;
        newpath += ({temp});
}

protected void link_to_east(int x,int y)        // The east room is (x+1,y)
{
        class coordinate temp;
        // can't link. 当前房间已经是最东面的房间了.
        if( (x+1) >= l )
                return;

        temp = new(class coordinate);
        temp->x = x+1;
        temp->y = y;

        // 东面的房间已经于 path 中,或者 已在待处理列表 newpath 中.
        if(all[temp->x][temp->y] /*|| member_array(temp,newpath)*/)
                return;

        all[x][y] |= E;
        all[temp->x][temp->y] |= W;
        newpath += ({temp});
}

protected void link_to_south(int x,int y)       // The south room is (x,y-1)
{
        class coordinate temp;
        // can't link. 当前房间已经是最南端的房间了.
        if( (y-1) <0 )
                return;

        temp = new(class coordinate);
        temp->x = x;
        temp->y = y-1;

        // 南端的房间已经于 path 中,或者 已在待处理列表 newpath 中.
        if(all[temp->x][temp->y] /*|| member_array(temp,newpath)*/)
                return;

        all[x][y] |= S;
        all[temp->x][temp->y] |= N;
        newpath += ({temp});
}

protected void link_to_north(int x,int y)       // The north room is (x,y+1)
{
        class coordinate temp;
        // can't link. 当前房间已经是最北端的房间了.
        if( (y+1) >= l )
                return;

        temp = new(class coordinate);
        temp->x = x;
        temp->y = y+1;

        // 北端的房间已经于 path 中,或者 已在待处理列表 newpath 中.
        if(all[temp->x][temp->y] /*|| member_array(temp,newpath)*/)
                return;

        all[x][y] |= N;
        all[temp->x][temp->y] |= S;
        newpath += ({temp});
}

// 绘制已建成迷宫的地图.
void paint_vrm_map()
{
        string hor = "─" ,ver = "│  ",room = "◎",sroom = "●";
        int x,y;
        string output = "",map_file;

        for(y=(l-1);y>=0;y--)
        {
                reset_eval_cost();

                output += sprintf("y=%-3d: ",y);
                for(x=0;x<l;x++)
                {
                        output += sprintf("%s",
                                (( (x==enter->x) && (y==enter->y))
                                || ( (x==leave->x) && (y==leave->y) ))?
                                sroom:room);

                        if( (all[x][y])&E ) // have east
                                output += hor;
                        else
                                output += "  ";
                }

                output += "\n";
                output += "       ";
                for(x=0;x<l;x++)
                {
                        if( (all[x][y])&S ) // have south
                                output += ver;
                        else
                        output += "    ";
                }
                output += "\n";
        }

        map_file = sprintf( "%s.map",base_name(this_object()) );
        write_file(map_file,output,1);
}

// 绘制动态地图 by Lonely@nitan3
varargs void paint_dynamic_map(int x1, int y1, string arg)
{
        string hor = "─" ,ver = "│  ",room = "◎",sroom = "●";
        int x,y;
        string output = "\n迷宫地图，" HIG "●" NOR "->入口，" HIM "●" NOR "->出口，";
        output += BLINK "◎" NOR "->位置\n\n";

        if (arg && arg == "entry")
        {
                x1 = enter->x;
                y1 = enter->y;
        }

        if (arg && arg == "exit")
        {
                x1 = leave->x;
                y1 = leave->y;
        }

        for(y=(l-1);y>=0;y--)
        {
                reset_eval_cost();

                // output += sprintf("y=%-3d: ",y);
                for(x=0;x<l;x++)
                {
                        if (x == x1 && y == y1)
                                output += BLINK + room + NOR;
                        else
                        if ((x==enter->x) && (y==enter->y))
                                output += HIG + sroom + NOR;
                        else
                        if ((x==leave->x) && (y==leave->y))
                                output += HIM + sroom + NOR;
                        /*
                        else
                        if (query_maze_room(sprintf("%d/%d",x,y))->query("objects"))
                                output += HIR + room + NOR;
                        */
                        else
                                output += room;

                        if( (all[x][y])&E ) // have east
                                output += hor;
                        else
                                output += "  ";
                }

                output += "\n";
                // output += "       ";
                for(x=0;x<l;x++)
                {
                        if( (all[x][y])&S ) // have south
                                output += ver;
                        else
                        output += "    ";
                }
                output += "\n";
        }

        write(output);
}

nomask int clean_up()
{
        string fname;
        int x,y;
        object room;

        if(!maze_built)
        {
                destruct(this_object());
                return 0;
        }

        fname = base_name(this_object());

        if( objectp(room = find_object(sprintf("%s/entry",fname))) )
        {
                room->clean_up();
                if( objectp(room) )
                        return 1;
        }

        if( objectp(room = find_object(sprintf("%s/exit",fname))) )
        {
                room->clean_up();
                if( objectp(room) )
                        return 1;
        }

        for(x=0;x<l;x++)
                for(y=0;y<l;y++)
                        if(objectp(room = find_object(sprintf("%s/%d/%d",fname,x,y))))
                        {
                                //maze_objs += ({find_object(sprintf("%s/%d/%d",fname,x,y))});
                                room->clean_up();
                                if( objectp(room) )
                                        return 1;
                        }
        /*
        maze_objs->clean_up();
        maze_objs -= ({0});

        if(sizeof(maze_objs))
                return 1;
        else
        {
        */
                destruct(this_object());
                return 0;
//      }
}

void remove(string euid)
{
        string fname = base_name(this_object());
        object m_room;
        int x,y;

        for(x=0;x<l;x++)
                for(y=0;y<l;y++)
                        if(objectp(m_room = find_object(sprintf("%s/%d/%d",fname,x,y))))
                                destruct(m_room);
        if(m_room = find_object(sprintf("%s/entry",fname)))
                destruct(m_room);
        if(m_room = find_object(sprintf("%s/exit",fname)))
                destruct(m_room);
}

void refresh_maze()
{
        if (this_object()->has_player())
        {
                SCHEDULE_D->set_event(10, 0, this_object(), "refresh_maze"); 
                return;
        }
        // remove();
        destruct(this_object());
        return;
} 

int has_player()
{
        string fname = base_name(this_object());
        object mroom;
        int x,y; 
        for(x=0;x<l;x++)
        {
                for(y=0;y<l;y++)
                {
                        if(objectp(mroom = find_object(sprintf("%s/%d/%d",fname,x,y))))
                                if (player_in(mroom))
                                        return 1; 
                }
        }
                                
        if(objectp(mroom = find_object(sprintf("%s/entry",fname))))
                if (player_in(mroom))
                        return 1; 
        if(objectp(mroom = find_object(sprintf("%s/exit",fname))))
                if (player_in(mroom))
                        return 1;
        return 0;
} 

//      检查某个房间里是否有玩家。 
int player_in(object mroom)
{
        object *inv;
        inv = all_inventory(mroom);
        for (int i = 0;i < sizeof(inv) ; i++)
                //if player in the room return
                if (userp(inv[i]))
                        return 1; 
        return 0;
} 

//      把所有玩家从迷宫里搬走。
int remove_all_players(object maze_room,object exile_room,string remove_msg){
        
        string fname = base_name(maze_room);
        object m_room,*inv;
        int x,y,k,num,length; 
        length=maze_room->query_maze_length();
        
        for(x=0;x<length;x++)
        {
                for(y=0;y<length;y++)
                {
                        if(objectp(m_room = find_object(sprintf("%s/%d/%d",fname,x,y)))) {
                                tell_room(m_room,remove_msg);
                                inv=all_inventory(m_room);
                                num=sizeof(inv);
                                while(num--) {
                                        if (userp(inv[num]))
                                                inv[num]->move(exile_room);     
                                }       
                        }        
                }
        }
                                
        if(objectp(m_room = find_object(sprintf("%s/entry",fname)))) {
                tell_room(m_room,remove_msg);
                inv=all_inventory(m_room);
                num=sizeof(inv);
                while(num--) {
                        if (userp(inv[num]))
                                inv[num]->move(exile_room);     
                }
        }
        
        if(objectp(m_room = find_object(sprintf("%s/exit",fname))))     {
                tell_room(m_room,remove_msg);
                inv=all_inventory(m_room);
                num=sizeof(inv);
                while(num--) {
                        if (userp(inv[num]))

                                inv[num]->move(exile_room);     
                }
        }
        
}   

/**** 以下是预设迷宫参数的接口函数 ****/
// 迷宫的单边长
void set_maze_long(int mlong)
{
        if(!intp(mlong))
                return;

        // 最小为 5，再小了没什么意义。
        if( (mlong < 5) || mlong > MAX_LONG )
                return;

        l = mlong;
}

// 迷宫可使用的房间文件名 (****)
void set_valid_rooms(mixed room_files)
{
        if(stringp(room_files))
        {
                // 档案是否存在
                if(file_size(sprintf("%s.c",room_files)) > 0)
                {
                        object ob = find_object(room_files);

                        if(!ob)
                                ob = load_object(room_files);
                        if(ob && ob->query("short") && ob->query("long"))
                        {
                                valid_rooms = ({ room_files });
                                switch_flag = 1;
                        }
                }
                return;
        }

        else if(arrayp(room_files))
        {
                foreach(string f in room_files)
                {
                        object ob;

                        if(!stringp(f) || f == "")
                                return;
                        if(file_size(sprintf("%s.c",f)) <= 0)
                                return;
                        if(!ob = find_object(f))
                                ob = load_object(f);
                        if(!ob || !ob->query("short") || !ob->query("long") )
                                return;
                }
                valid_rooms = room_files;
                switch_flag = 1;
                return;
        }

}

// 迷宫房间所继承的物件的档案名称。
void set_inherit_room( mixed rooms )
{
        if(stringp(rooms))
        {
                // 此档案是否存在
                if(file_size(sprintf("%s.c",rooms)) > 0)
                        inherit_rooms = ({ rooms });
                return;
        }

        else if(arrayp(rooms))
        {
                foreach(string f in rooms)
                {
                        if(!stringp(f) || f == "")
                                return;
                        if(file_size(sprintf("%s.c",f)) <= 0)
                                return;
                }
                inherit_rooms = rooms;
                return;
        }

        return;
}

// 入口方向(出口在对面)
void set_entry_dir(string dir)
{
        if(!stringp(dir))
                return;

        // 入口方向的合法性检查.
        if(member_array(dir,valid_dirs) == -1)
                return;

        entry_dir = dir;
}

// 入口与区域的连接方向
void set_link_entry_dir(string dir)
{
        if(!stringp(dir) || dir == "")
                return;

        link_entry_dir = dir;
}

// 迷宫入口所连接区域档案的文件名
void set_link_entry_room(string lroom)
{
        if(!stringp(lroom) || lroom == "")
                return;

        if(file_size(sprintf("%s.c",lroom)) <= 0)
                return;

        link_entry_room = lroom;
}

// 迷宫入口房间的坐标，我们无法用简单的方法把迷宫坐标安排在
// 进口/出口内（这个需要得到迷宫与区域的关系）
// 所以我们认为迷宫是进口区域延伸的一部分，当走出迷宫出口时，跳跃进入下一个区域。
// 缺点是，一个大迷宫的坐标可能与真实区域重叠，这个就要设计者自己留意了，
// 比如说，不要把100x100的迷宫安排在 (1,1)和 (10,10)之间。。。。自己算一下吧 
void set_link_entry_room_x(int x)
{
        if(!intp(x)) link_entry_room_x=0;
                else    link_entry_room_x = x;
}  
void set_link_entry_room_y(int y)
{

        if(!intp(y)) link_entry_room_y=0;
                else    link_entry_room_y = y;
}  
void set_link_entry_room_z(int z)
{
        if(!intp(z)) link_entry_room_z=0;
                else    link_entry_room_z = z;
} 

// 出口与区域的连接方向
void set_link_exit_dir(string dir)
{
        if(!stringp(dir) || dir == "")
                return;

        link_exit_dir = dir;
}

// 迷宫出口所连接区域档案的文件名
void set_link_exit_room(string lroom)
{
        if(!stringp(lroom) || lroom == "")
                return;

        if(file_size(sprintf("%s.c",lroom)) <= 0)
                return;

        link_exit_room = lroom;
}

// 迷宫入口的短描述
void set_entry_short(string desc)
{
        if(!stringp(desc) || desc == "")
                return;

        entry_short = desc;
}

// 迷宫入口的长描述
void set_entry_desc(string desc)
{
        if(!stringp(desc) || desc == "")
                return;

        entry_desc = desc;
}

// 迷宫出口的短描述
void set_exit_short(string desc)
{
        if(!stringp(desc) || desc == "")
                return;

        exit_short = desc;
}

// 迷宫出口的长描述
void set_exit_desc(string desc)
{
        if(!stringp(desc) || desc == "")
                return;

        exit_desc = desc;
}

// 迷宫房间的短描述
void set_maze_room_short(string desc)
{
        if(!stringp(desc) || desc == "")
                return;

        maze_room_short = desc;
}

// 迷宫房间的描述，如果有多条描述，制造每个房
// 间的时候会从中随机选择一个。
void set_maze_room_desc(mixed desces)
{
        if(stringp(desces))
        {
                maze_room_desc = ({ desces });
                return;
        }

        if(arrayp(desces))
        {
                foreach(string desc in desces)
                        if(!stringp(desc))
                                return;
                maze_room_desc = desces;
                return;
        }
}

// 迷宫房间是否为户外房间
void set_outdoors(int outd)
{
        if(!intp(outd))
                return;

        if(outd)
                is_outdoors = 1;
}

// 迷宫中的怪物
void set_maze_npcs(mixed npc)
{
        if(stringp(npc))
        {
                // 此档案是否存在
                if(file_size(sprintf("%s.c",npc)) > 0)
                        maze_npcs = ({ npc });
                return;
        }
        else if(arrayp(npc))
        {
                foreach(string f in npc)
                {
                        if(!stringp(f) || f == "")
                                return;
                        if(file_size(sprintf("%s.c",f)) <= 0)
                                return;
                }
                maze_npcs = npc;
                return;
        }
        else if(mapp(npc))
        {
                string *ns;

                ns = keys(npc);
                foreach(string f in ns)
                {
                        if(!stringp(f) || f == "")
                                return;
                        if(file_size(sprintf("%s.c",f)) <= 0)
                                return;
                }
                maze_npcs = npc;
                return;
        }

        return;

}

// 迷宫入口的怪物 by Lonely@nitan3
void set_entry_npcs(mixed npc)
{
        if(stringp(npc))
        {
                // 此档案是否存在
                if(file_size(sprintf("%s.c",npc)) > 0)
                        entry_npcs = ({ npc });
                return;
        }

        else if(arrayp(npc))
        {
                foreach(string f in npc)
                {
                        if(!stringp(f) || f == "")
                                return;
                        if(file_size(sprintf("%s.c",f)) <= 0)
                                return;
                }
                entry_npcs = npc;
                return;
        }
        else if(mapp(npc))
        {
                string *ns;

                ns = keys(npc);
                foreach(string f in ns)
                {
                        if(!stringp(f) || f == "")
                                return;
                        if(file_size(sprintf("%s.c",f)) <= 0)
                                return;
                }
                entry_npcs = npc;
                return;
        }
        return;

}

// 迷宫出口的怪物 by Lonely@nitan3
void set_exit_npcs(mixed npc)
{
        if(stringp(npc))
        {
                // 此档案是否存在
                if(file_size(sprintf("%s.c",npc)) > 0)
                        exit_npcs = ({ npc });
                return;
        }

        else if(arrayp(npc))
        {
                foreach(string f in npc)
                {
                        if(!stringp(f) || f == "")
                                return;
                        if(file_size(sprintf("%s.c",f)) <= 0)
                                return;
                }
                exit_npcs = npc;
                return;
        }
        else if(mapp(npc))
        {
                string *ns;

                ns = keys(npc);
                foreach(string f in ns)
                {
                        if(!stringp(f) || f == "")
                                return;
                        if(file_size(sprintf("%s.c",f)) <= 0)
                                return;
                }
                exit_npcs = npc;
                return;
        }
        return;
}

void set_unique_room(mixed room)
{
        if(stringp(room))
        {
                // 此档案是否存在
                if(file_size(sprintf("%s.c",room)) > 0)
                        unique_rooms = ({ room });
                
                if (sizeof(unique_rooms))
                        
                return;
        }

        else if(arrayp(room))
        {
                foreach(string f in room)
                {
                        if(!stringp(f) || f == "")
                                return;
                        if(file_size(sprintf("%s.c",f)) <= 0)
                                return;
                }
                unique_rooms = room;
                return;
        } 
        return; 
}
     
// 迷宫刷新
void set_maze_refresh(int refresh)
{
        if (intp(refresh) && refresh>=1)
                refresh_delay = refresh; 
} 

// 迷宫设计非死亡房间
void set_no_death_room(int is_no_death)
{
        if (intp(is_no_death) && is_no_death>=1)
                is_nodeath_room = is_no_death;
}

/**** 以上是预设迷宫参数的接口函数 ****/

// 创造迷宫房间，由 VIRTUAL_D 调用。
nomask object query_maze_room(string str)
{
        int random_rate = 50;   // 房间内放置 npc 的可能性
        int idx,x,y,exits;
        object ob;
        string f;
        int r, unique_flag;

        /*
        if( previous_object() && (geteuid(previous_object()) != ROOT_UID) )
                return 0;
        */

        if( !stringp(str) || str == "" )
                return 0;

        if( !maze_built ) // 迷宫未建立
                create_maze();
        if( !maze_built )
                return 0;

        if( str == "entry" )      // 迷宫入口房间
        {
                f = inherit_rooms[random(sizeof(inherit_rooms))];
                
                // ob = load_object(f);
                ob = new(f);
                if( !ob )
                        return 0;
                ob->set("short",entry_short);
                ob->set("long",entry_desc);
                ob->set("maze", 1);
                ob->set("no_magic", 1);
                ob->set("virtual_room",1);
                if( is_outdoors )
                        ob->set("outdoors",1);
                if( is_nodeath_room )
                        ob->set("no_death_penalty",1);
                ob->set(sprintf("exits/%s",link_entry_dir),link_entry_room);
                ob->set(sprintf("exits/%s",reverse_dir[entry_dir]),mroom_fname(enter->x,enter->y));
                ob->set("coor/x",link_entry_room_x);
                ob->set("coor/y",link_entry_room_y);
                ob->set("coor/z",link_entry_room_z);
                if( arrayp(entry_npcs) && sizeof(entry_npcs) )
                {
                        ob->set("objects",([
                                entry_npcs[random(sizeof(entry_npcs))] : 1,
                        ]));
                        ob->setup();
                } else
                if( mapp(entry_npcs) && sizeof(entry_npcs) )
                {
                        ob->set("objects", entry_npcs);
                        ob->setup();
                }

                return ob;
        }

        if( str == "exit" )       // 迷宫出口房间
        {
                f = inherit_rooms[random(sizeof(inherit_rooms))];

                // ob = load_object(f);
                ob = new(f);
                if( !ob )
                        return 0;

                ob->set("short",exit_short);
                ob->set("long",exit_desc);
                ob->set("maze", 1);
                ob->set("no_magic", 1);
                ob->set("virtual_room",1);
                if( is_outdoors )
                        ob->set("outdoors",1);
                if( is_nodeath_room )
                        ob->set("no_death_penalty",1);
                ob->set(sprintf("exits/%s",link_exit_dir),link_exit_room);
                ob->set(sprintf("exits/%s",entry_dir),
                        mroom_fname(leave->x,leave->y));

                if( arrayp(exit_npcs) && sizeof(exit_npcs) )
                {
                        ob->set("objects",([
                                exit_npcs[random(sizeof(exit_npcs))] : 1,
                        ]));
                        ob->setup();
                } else
                if( mapp(exit_npcs) && sizeof(exit_npcs) )
                {
                        ob->set("objects", exit_npcs);
                        ob->setup();
                } 
                
                return ob;
        }

        idx = member_array('/', str);
        if( idx == -1 )
                return 0;

        if( !sscanf(str[0..idx-1],"%d",x) )
                return 0;
        if( !sscanf(str[idx+1..],"%d",y) )
                return 0;

        if( !exits = all[x][y] )
                return 0;
        
        // 处理唯一房间的放置位置
        // 四个边角加中心或者以下方式来处理
        // By Lonely@Nitan3
        unique_flag = 0;
        if( r = sizeof(unique_rooms) )
        {
                if( r >= 1 && x==(l-1) && y == 0 )
                {
                        unique_flag = 1;
                        f = unique_rooms[0];
                } else
                if( r >= 2 && x==(l-1) && y == (l-1) )
                {
                        unique_flag = 2;
                        f = unique_rooms[1];
                } else
                if( r >= 3 && x == 0 && y == (l-1) )
                {
                        unique_flag = 3;
                        f = unique_rooms[2];
                } else
                if( r >= 4 && x == 0 && y == 0 )
                {
                        unique_flag = 4;
                        f = unique_rooms[3];   
                } else
                if( r >= 5 && x == to_int(l/2) && y == to_int(l/2) )                                                            
                {
                        unique_flag = 5;
                        f = unique_rooms[4];  
                }                        
                /*                      
                for( int i=0;i<r-1;i++ )
                        if( x==(l-1)/(i+1) && y==(l-1)/(i+1) )
                        {
                                unique_flag = i;
                                f = unique_rooms[i];
                        }
                */
        }
        
        if( !unique_flag )
        {         
                // 不同的创建方法
                if( switch_flag )
                        f = valid_rooms[random(sizeof(valid_rooms))];
                else
                        f = inherit_rooms[random(sizeof(inherit_rooms))];
        }

        // ob = load_object(f);
        ob = new(f);
        if( !ob )
                return 0;

        if( !switch_flag && !unique_flag )
        {
                ob->set("short",maze_room_short);
                ob->set("long",maze_room_desc[random(sizeof(maze_room_desc))]);
                ob->set("maze", 1);
                ob->set("no_magic", 1);
                ob->set("virtual_room",1);
                if(is_outdoors)
                        ob->set("outdoors",1);
                if(is_nodeath_room)
                        ob->set("no_death_penalty",1);
        }

        // 根据入口坐标和迷宫大小来定义房间坐标
        ob->set("coor/x",x*10-l+link_entry_room_x);
        ob->set("coor/y",y*10-to_int(l/2)+link_entry_room_y);
        ob->set("coor/z",link_entry_room_z);
        
        if( exits&W )
                ob->set("exits/west",mroom_fname(x-1,y));
        if( exits&E )
                ob->set("exits/east",mroom_fname(x+1,y));
        if( exits&N )
                ob->set("exits/north",mroom_fname(x,y+1));
        if( exits&S )
                ob->set("exits/south",mroom_fname(x,y-1));

        if( (x == enter->x) && (y == enter->y) )
                ob->set(sprintf("exits/%s",entry_dir),
                        sprintf("%s/entry",base_name(this_object())));
        if( (x == leave->x) && (y == leave->y) )
                ob->set(sprintf("exits/%s",reverse_dir[entry_dir]),
                        sprintf("%s/exit",base_name(this_object())));
        
        if( !switch_flag && !unique_flag )
        {
                if( mapp(maze_npcs) && sizeof(maze_npcs) && (random(100) <= random_rate) )
                {
                        ob->set("objects", maze_npcs);
                        ob->setup();
                } else
                if( arrayp(maze_npcs) && sizeof(maze_npcs) && (random(100) <= random_rate) )
                {
                        ob->set("objects",([
                                maze_npcs[random(sizeof(maze_npcs))] : 1,
                        ]));
                        ob->setup();
                }
        }
        
        return ob;
}
