inherit ROOM;

void create()
{
        set("short", "����");
        set("long", @LONG
������һ�䰲�����������������Ǹ������е��Ӵ�������
֮����������������ط����������š���λ����ʦ̫���׼�Ů
�����������붨��
LONG);
        set("exits", ([
                "west" : __DIR__"lingwenge",
        ]));
        set("sleep_room", 1);
        set("no_clean_up", 0);
        set("no_fight", 1);
	set("coor/x", -430);
	set("coor/y", -240);
	set("coor/z", 70);
	setup();
        replace_program(ROOM);
}