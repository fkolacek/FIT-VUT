/*
 * Projekt: Queen
 * Predmet: IJA - Seminar Java
 * Soubor: Position.java
 * Autori:
 *          xkolac12 < xkolac12 @ stud.fit.vutbr.cz >
 *          xmatya03 < xmatya03 @ stud.fit.vutbr.cz >
 * 
 * Trida Position reprezentuje pozici na sachovnici
 */

package queen.basis;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @author      Frantisek Kolacek <xkolac12 @ stud.fit.vutbr.cz>
 * @author      Petr Matyas <xmatya03 @ stud.fit.vutbr.cz>
 * @version     0.91
 * @since       2013-04-30
 */
public class Position {

    private char column;
    private int row;

    /**
     * Konstruktor pro objekt Position
     * @param column    znak sloupce
     * @param row       cislo radku
     */
    public Position(char column, int row){
        this.column = column;
        this.row = row;
    }

    /**
     * Kopirovaci konstruktor pro objekt Position
     * @param position  instance objektu Position
     */
    public Position(Position position){
        this.column = position.getColumn();
        this.row = position.getRow();
    }

    /**
     * Konstruktor pozice figurky
     * @param position string s pozici figurky
     */
    public Position(String position){
        //System.out.println("Position init: " + position);
        Pattern regex = Pattern.compile("^([a-z])([0-9])$");
        Matcher match = regex.matcher(position);

        if(match.find()){
            this.column = match.group(1).charAt(0);
            this.row = Integer.parseInt(match.group(2));
        }
        else{
            this.column = 'a';
            this.row = 1;
        }
    }

    /**
     * Getter pro znak sloupce
     * @return  znak sloupce
     */
    public char getColumn(){
        return this.column;
    }

    /**
     * Getter pro cislo radku
     * @return cislo radku
     */
    public int getRow(){
        return this.row;
    }

    /**
     * Porovnani dvou objektu Position
     * @param object druha pozice pro porovnani
     * @return true pokud jsou shodne, jinak false
     */
    @Override
    public boolean equals(Object object){
        if(object == null || object.getClass() != Position.class)
            return false;

        Position position = (Position) object;

        if(this.column == position.getColumn() && this.row == position.getRow())
            return true;
        else
            return false;
    }

    /**
     * Prevedeni pozice na string
     * @return string s pozici
     */
    @Override
    public String toString(){
        return new String(Character.toString(this.column) + Integer.toString(this.row));
    }

}
