// SN:C53RN5ZOWFD1A?KO
// cruise-ob.c

#include "/clone/misc/cruise-ob.h"

string long() 
{
        string msg; 
        
        msg = query("long"); 
        msg += "这是张二小(Testc)的漫游传送包。
";
        msg += extra_long();
        
        return sort_string(msg, 64, 0); 
}

