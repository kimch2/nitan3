// /d/playertown/road1.c ��ʯС��
// Build 1999.12.10 bmw

inherit BUILD_ROOM;

void create()
{
	set("short","��ש·");
	set("long",
"����һ���ǳ�ƽ������ש�̳ɵ�·��������һ���ǳ����µ�С��԰��\n"
"��������һ�������ڡ�\n"
);
	set("recordable_room",1);
	set("outdoors","playertown");	
	set("exits",([
		"northeast" : __DIR__"huayuan1",
		"west" : __DIR__"road11",
		"southeast" : __DIR__"road8",
	]));
	setup();
	// replace_program(ROOM);
}