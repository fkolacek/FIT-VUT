/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Main;

import java.io.Serializable;

/**
 *
 * @author tuxik
 */
public class packet_struct implements Serializable 
{
    private static final long serialVersionUID = 1L;
    public int command;
    public Position from;
    public Position to;
}