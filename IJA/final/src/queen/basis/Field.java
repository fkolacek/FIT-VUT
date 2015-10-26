/*
 * Projekt: Queen
 * Predmet: IJA - Seminar Java
 * Soubor: Field.java
 * Autori:
 *          xkolac12 < xkolac12 @ stud.fit.vutbr.cz >
 *          xmatya03 < xmatya03 @ stud.fit.vutbr.cz >
 * 
 * Trida Field reprezentuje herni pole
 */

package queen.basis;

import java.awt.*;

/**
 * @author      Frantisek Kolacek <xkolac12 @ stud.fit.vutbr.cz>
 * @author      Petr Matyas <xmatya03 @ stud.fit.vutbr.cz>
 * @version     0.91
 * @since       2013-04-30
 */
public class Field {

    private Position position;
    private Color color;
    private Figure figure;

    /**
     * Konstruktor pro objekt Field
     * @param position pozice
     * @param color barva
     */
    public Field(Position position, Color color){
        this.position = position;
        this.color = color;
        this.figure = null;
    }

    /**
     * Konstruktor pro objekt Field
     * @param position pozice
     * @param color barva
     * @param figure typ figurky
     */
    public Field(Position position, Color color, Figure figure){
        this.position = position;
        this.color = color;
        this.figure = figure;
    }

    /**
     * Getter pro ziskani pozice herniho pole
     * @return pozice herniho pole
     */
    public Position getPosition(){
        return this.position;
    }

    /**
     * Getter pro ziskani barvy policka
     * @return barva policka
     */
    public Color getColor(){
        return this.color;
    }

    /**
     * Getter pro ziskani odkazu na figurku
     * @return odkaz na figurku
     */
    public Figure getFigure(){
        return this.figure;
    }

    /**
     * Umisteni figurky na herni desku
     * @param figure odkaz na figurku
     * @return puvodni figurka na herni desce
     */
    public Figure setFigure(Figure figure){
        Figure tmp = this.figure;

        this.figure = figure;

        return tmp;
    }

    /**
     * Odstraneni figurky z herniho planu
     * @return odkaz na odstranenou figurku
     */
    public Figure removeFigure(){
        Figure tmp = this.figure;

        this.figure = null;

        return tmp;
    }
}
