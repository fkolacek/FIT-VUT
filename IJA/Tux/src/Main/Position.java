package Main;

import java.io.Serializable;

public class Position implements Serializable 
{
    public int x;
    public int y;
    
    public Position(int x,int y)
    {
        this.x = x;
        this.y = y;
    }
    public boolean isSet()
    {
        if(x != -1)
            return true;
        return false;
    }
    public boolean equals(Position p)
    {
        if(p.x == this.x && p.y == this.y)
            return true;
        return false;            
    }
}
