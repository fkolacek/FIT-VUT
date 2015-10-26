/*
 * Projekt: Queen
 * Predmet: IJA - Seminar Java
 * Soubor: Rook.java
 * Autori:
 *          xkolac12 < xkolac12 @ stud.fit.vutbr.cz >
 *          xmatya03 < xmatya03 @ stud.fit.vutbr.cz >
 * 
 * Trida Rook reprezentuje figurku Dama
 */

package queen.figures;

import queen.basis.*;
import java.awt.*;
import java.util.Vector;

/**
 * @author      Frantisek Kolacek <xkolac12 @ stud.fit.vutbr.cz>
 * @author      Petr Matyas <xmatya03 @ stud.fit.vutbr.cz>
 * @version     0.91
 * @since       2013-04-30
 */
public class Rook extends Figure{

    /**
     * Konstruktor pro objekt Rook
     * @param desk hraci plan
     * @param position pozice
     * @param color barva
     */
    public Rook(Desk desk, Position position, Color color){
        super(desk, position, color);
    }

    /**
     * Overeni, jestli se figurka muze na danou pozici presunout
     * @param position pozice kam se chce presunout
     * @return lze se presunout?
     */
    @Override
    public boolean canMove(Position position){
        Vector possibilities = this.canMovePossibilities();

        for(int i = 0; i < possibilities.size(); i++){
            Possibility possibility = (Possibility)possibilities.get(i);

            if(possibility.getPosition().equals(position))
                return true;
        }

        return false;
    }

    /**
     * Overeni, kam vsude se muze figurka presunout
     * @return vektor moznych pozic pro presun
     */
    @Override
    public Vector canMovePossibilities(){
        Vector possibilities = new Vector();
        Vector kill = new Vector();

        Position step;
        int row;
        char column;
        boolean ok = true;
        Possibility possibility = null;
        boolean areYouThereAssassin = false;
        //vlevo nahoru
        for (row = this.getPosition().getRow()-1, column = (char)(this.getPosition().getColumn()-1); row > 0 && column >= 'a'; row--, column--) {
            step = new Position(column, row);
            if(this.desk.isDeserter(step))
                continue;
            if(this.desk.at(step).getFigure() == null) {
                if (ok == false)
                    ok = true;
                possibility = new Possibility(this.position, step, kill);
                possibilities.add(possibility);
                if (kill.size() > 0)
                    areYouThereAssassin = true;
            }
            else
                if (this.getColor() == this.desk.at(step).getFigure().getColor())
                    break;
                else {
                    if (ok == false)
                        break;
                    kill.add(new Position(step));
                    ok = false;
                }
        }
        kill = new Vector();
        ok = true;
        //vpravo nahoru
        for (row = this.getPosition().getRow()-1, column = (char)(this.getPosition().getColumn()+1); row > 0 && column <= 'h'; row--, column++) {
            step = new Position(column, row);
            if(this.desk.isDeserter(step))
                continue;
            if(this.desk.at(step).getFigure() == null) {
                if (ok == false)
                    ok = true;
                possibility = new Possibility(this.position, step, kill);
                possibilities.add(possibility);
                if (kill.size() > 0)
                    areYouThereAssassin = true;
            }
            else
                if (this.getColor() == this.desk.at(step).getFigure().getColor())
                    break;
                else {
                    if (ok == false)
                        break;
                    kill.add(new Position(step));
                    ok = false;
                }
        }
        kill = new Vector();
        ok = true;
        //vlevo dolu
        for (row = this.getPosition().getRow()+1, column = (char)(this.getPosition().getColumn()-1); row <= 8 && column >= 'a'; row++, column--) {
            step = new Position(column, row);
            if(this.desk.isDeserter(step))
                continue;
            if(this.desk.at(step).getFigure() == null) {
                if (ok == false)
                    ok = true;
                possibility = new Possibility(this.position, step, kill);
                possibilities.add(possibility);
                if (kill.size() > 0)
                    areYouThereAssassin = true;
            }
            else
                if (this.getColor() == this.desk.at(step).getFigure().getColor())
                    break;
                else {
                    if (ok == false)
                        break;
                    kill.add(new Position(step));
                    ok = false;
                }
        }
        kill = new Vector();
        ok = true;
        //vpravo dolu
        for (row = this.getPosition().getRow()+1, column = (char)(this.getPosition().getColumn()+1); row <= 8 && column <= 'h'; row++, column++) {
            step = new Position(column, row);
            if(this.desk.isDeserter(step))
                continue;
            if(this.desk.at(step).getFigure() == null) {
                if (ok == false)
                    ok = true;
                possibility = new Possibility(this.position, step, kill);
                possibilities.add(possibility);
                if (kill.size() > 0)
                    areYouThereAssassin = true;
            }
            else
                if (this.getColor() == this.desk.at(step).getFigure().getColor())
                    break;
                else {
                    if (ok == false)
                        break;
                    kill.add(new Position(step));
                    ok = false;
                }
        }

        if(areYouThereAssassin){ // nekdo bude ostranen
            Vector tmp = possibilities;
            possibilities = new Vector();

            for(int i = 0; i < tmp.size(); i++){
                possibility = (Possibility)tmp.get(i);

                if(possibility.killed() > 0)
                    possibilities.add(possibility);
            }
        }

        return possibilities;
    }
}
