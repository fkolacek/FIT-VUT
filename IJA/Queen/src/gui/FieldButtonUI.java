/*
 * Projekt: Queen
 * Predmet: IJA - Seminar Java
 * Soubor: FieldButtonUI.java
 * Autori:
 *          xkolac12 < xkolac12 @ stud.fit.vutbr.cz >
 *          xmatya03 < xmatya03 @ stud.fit.vutbr.cz >
 * 
 * Trida FieldButtonUI predstavuje pole na sachovnici
 */

package gui;

import java.awt.*;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.*;
import queen.basis.*;

/**
 * @author      Frantisek Kolacek <xkolac12 @ stud.fit.vutbr.cz>
 * @version     0.91
 * @since       2013-04-30
 */
public final class FieldButtonUI extends JButton {
    
    private Field field;
    private Image image;
    private boolean activated;
    
    /**
     * Konstruktor z objektu typu field
     * @param field objekt typu field
     */
    public FieldButtonUI(Field field){
        this(field, "");
    }
    
    /**
     * Pretizeny konstruktor
     * @param field objekt typu field
     * @param title popis tlacitka
     */
    public FieldButtonUI(Field field, String title){
        super(title);
        
        this.setBorder(null);
        this.setSize(50, 50);
        
        this.field = field;
        this.activated = true;
        
        this.toogle();
    }
    
    private String getImageName(){
        String imageName = "/gfx/";
        
        if(this.activated)
            imageName += "marked_";
        
        if(this.field.getFigure() == null)
            imageName += (this.field.getColor() == Color.BLACK)? "black" : "white";
        else{ 
            
            if(this.field.getFigure().getColor() == Color.BLACK)
                imageName += ("Rook".equals(this.field.getFigure().getClass().getSimpleName()))? "black_rook" : "black_stone";
            else
                imageName += ("Rook".equals(this.field.getFigure().getClass().getSimpleName()))? "white_rook" : "white_stone";            
        }    
        
        imageName += ".png";
        
        //System.out.println(imageName);
                
        return imageName;
    }
    
    /**
     * Docasne oznaci tlacitko
     */
    public void mark(){
        this.activated = true;
        this.reload();
        this.activated = false;
    }
    
    /**
     * Zmeni stav z oznaceneho a naopak
     */
    public void toogle(){
        this.activated = !this.activated;        
        this.reload();
    }
    
    /**
     * Znovu nacte odpovidajici obrazek tlacitku
     */
    public void reload(){
        try{
            String fieldImage = this.getImageName();

            this.image = ImageIO.read(getClass().getResource(fieldImage));

            this.setIcon(new ImageIcon(this.image));
        }
        catch(IOException ex){
        
        }
    }
    
    @Override
    public Dimension getPreferredSize(){
        //Dimension size = super.getPreferredSize();
        Dimension size = new Dimension(50, 50);
        return size;
    }    
    
    /**
     * Nastavi pole
     * @param field objekt typu Field
     */
    public void setField(Field field){
        this.field = field;
    }
    
    /**
     * Vrati pole
     * @return objekt typu Field
     */
    public Field getField(){
        return this.field;
    }
    
}
