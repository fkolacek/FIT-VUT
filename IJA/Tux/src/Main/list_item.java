/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Main;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author tuxik
 */
public class list_item {
    
    private List<Position> pos;
    private boolean take;
       
    public list_item()
    {
        this.pos = new ArrayList();
        take = false;
    }
    public void add_item(Position p)
    {
        this.pos.add(p);
    }
    public Position get_item(int index)
    {
        return this.pos.get(index);
    }
    public int get_size()
    {
        return this.pos.size();
    }
    public void clear_list()
    {
        pos.clear();
    }
    public void set_take(boolean take)
    {
        this.take = take;
    }
    public boolean get_take()
    {
        return take;
    }
}
