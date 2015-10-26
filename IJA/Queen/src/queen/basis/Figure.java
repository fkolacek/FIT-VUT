/*
 * Projekt: Queen
 * Predmet: IJA - Seminar Java
 * Soubor: Figure.java
 * Autori:
 *          xkolac12 < xkolac12 @ stud.fit.vutbr.cz >
 *          xmatya03 < xmatya03 @ stud.fit.vutbr.cz >
 * 
 * Abstraktni trida Figure reprezentuje figurku
 */

package queen.basis;

import java.awt.*;
import java.util.Vector;

/**
 * @author      Frantisek Kolacek <xkolac12 @ stud.fit.vutbr.cz>
 * @author      Petr Matyas <xmatya03 @ stud.fit.vutbr.cz>
 * @version     0.91
 * @since       2013-04-30
 */
abstract public class Figure {

    protected Desk desk;
    protected Position position;
    protected Color color;

    /**
     * Konstruktor pro objekt Figure
     * @param position  pozice herniho pole
     * @param color     barva herniho pole
     */
    public Figure(Position position, Color color){
        this(null, position, color);
    }

    /**
     * Konstruktor pro objekt Figure
     * @param desk      reference na hraci desku
     * @param position  pozice herniho pole
     * @param color     barva herniho pole
     */
    public Figure(Desk desk, Position position, Color color){
        this.desk = desk;
        this.position = position;
        this.color = color;
    }

    /**
     * Getter pro ziskani pozice herniho pole
     * @return  pozice herniho pole
     */
    public Position getPosition(){
        return this.position;
    }

    /**
     * Getter pro ziskani barvy herniho pole
     * @return  barva herniho pole
     */
    public Color getColor(){
        return this.color;
    }

    /**
     * Getter pro ziskani reference na herni desku
     * @return  reference na herni desku
     */
    public Desk getDesk(){
        return this.desk;
    }


    /**
     * Pohyb figurky
     * @param position nova pozice figurky
     * @return vektor odstranenych nepratelskych figurek
     * @throws RuntimeException nelze se pohnout
     */
    public Vector move(Position position) throws RuntimeException{

        if(!this.canMove(position))
            throw new RuntimeException();

        Vector possibilities = this.canMovePossibilities();
        Possibility possibility;
        Vector victims = null;

        this.position = position;

        for(int i = 0; i < possibilities.size(); i++){
            possibility = (Possibility)possibilities.get(i);

            if(possibility.equals(position)){
                victims = possibility.getVictims();
                break;
            }
        }

        return victims;
    }

    /**
     * Abstraktni metoda pro kontrolu, zda figurka muze pohyb uskutecnit
     * @param position  cilova pozice na herni desce
     * @return          true pokud figurka muze pohyb uskutecnit, jinak false
     */
    abstract public boolean canMove(Position position);

    /**
     * Abstraktni metoda pro zjisteni moznych pohybu figurky
     * @return vektor moznych pohybu a odstranenych figurek
     */
    abstract public Vector canMovePossibilities();
}
