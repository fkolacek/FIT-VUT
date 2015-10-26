/*
 * Projekt: Queen
 * Predmet: IJA - Seminar Java
 * Soubor: Possibility.java
 * Autori:
 *          xkolac12 < xkolac12 @ stud.fit.vutbr.cz >
 *          xmatya03 < xmatya03 @ stud.fit.vutbr.cz >
 * 
 * Trida Possibility reprezentuje mozne tahy na sachovnici
 */

package queen.basis;

import java.util.Vector;

/**
 * @author      Frantisek Kolacek <xkolac12 @ stud.fit.vutbr.cz>
 * @author      Petr Matyas <xmatya03 @ stud.fit.vutbr.cz>
 * @version     0.91
 * @since       2013-04-30
 */
public class Possibility {

    private Position killer;
    private Position position;
    private Vector victims;

    /**
     * Konstruktor mozne pozice pro pohyb a odstranenych figurek
     * @param position pozice kam se figurka presouva
     */
    public Possibility(Position killer, Position position) {
        this.killer = new Position(killer);
        this.position = new Position(position);
        this.victims = new Vector();
    }

    /**
     * Konstruktor mozne pozice pro pohyb a odstranenych figurek
     * @param possibility mozny pohyb
     */
    public Possibility(Possibility possibility){
        this.killer = new Position(possibility.getKiller());
        this.position = new Position(possibility.getPosition());
        this.victims = new Vector(possibility.getVictims());
    }

    /**
     * Konstruktor mozne pozice pro pohyb a odstranenych figurek
     * @param position pozice kam se figurka presouva
     * @param victim vektor odstranenych figurek
     */
    public Possibility(Position killer, Position position, Vector victim){
        this.killer = new Position(killer);
        this.position = new Position(position);
        this.victims = new Vector(victim);
    }
    
    /**
     * Getter pozice kam zabijaka
     * @return pozice zabijaka
     */
    public Position getKiller(){
        return this.killer;
    }

    /**
     * Getter pozice kam se figurka presouva
     * @return pozice kam se figurka presouva
     */
    public Position getPosition(){
        return this.position;
    }

    /**
     * Pridani dalsi odstranene figurky
     * @param victim pozice odstranene figurky
     */
    public void killVictim(Position victim){
        this.victims.add(new Position(victim));
    }

    /**
     * Getter odstranenych figurek
     * @return vektor ostranenych figurek
     */
    public Vector getVictims(){
        return this.victims;
    }

    /**
     * Getter poctu odstranenych figurek
     * @return pocet odstranenych figurek
     */
    public int killed(){
        return this.victims.size();
    }

    /**
     * Porovnani dvou objektu Position
     * @param object objekt pro porovnani
     * @return true pokud jsou shodne, jinak false
     */
    @Override
    public boolean equals(Object object){
        if(object == null)
            return false;
        else if(object.getClass() == Position.class){
            Position position = (Position) object;

            if(this.position.getColumn() == position.getColumn() && this.position.getRow() == position.getRow())
                return true;
        }

        return false;
    }
}
