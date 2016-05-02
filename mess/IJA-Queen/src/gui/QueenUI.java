/*
 * Projekt: Queen
 * Predmet: IJA - Seminar Java
 * Soubor: QueenUI.java
 * Autori:
 *          xkolac12 < xkolac12 @ stud.fit.vutbr.cz >
 *          xmatya03 < xmatya03 @ stud.fit.vutbr.cz >
 * 
 * Trida QueenUI predstavuje hlavni okno aplikace
 */

package gui;

import gui.basis.GameDifficulty;
import gui.basis.GameType;
import gui.basis.Notation;
import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;

/**
 * @author      Frantisek Kolacek <xkolac12 @ stud.fit.vutbr.cz>
 * @author      Petr Matyas <xmatya03 @ stud.fit.vutbr.cz>
 * @version     0.91
 * @since       2013-04-30
 */
public class QueenUI extends JFrame {

    private JMenuBar mainMenuBar;
    private JMenu mainMenuGame;
    private JMenuItem mainMenuGameNew;
    private JMenuItem mainMenuGameSave;
    private JMenuItem mainMenuGameReplay;
    private JMenuItem mainMenuGameMoveHinting;
    
    private JMenuItem mainMenuGameQuit;
    private JMenu mainMenuHelp;
    private JMenuItem mainMenuHelpAbout;

    private Container content;
    private BattleGroundUI battleground;
    
    private boolean MoveHinting;
    private GameDifficulty gameDifficulty;

    /**
     * Vytvori objekt typu QueenUI
     */
    public QueenUI() {
        initWindow();
        initMenu();
        initContent();

        this.pack();
    }

    /**
     * Inicializuje hlavni okno
     */   
    private void initWindow(){
        this.setTitle("Queen - IJA projekt");
        this.setIconImage(new ImageIcon(getClass().getResource("/icon.png")).getImage());

        this.setSize(600, 400);
        this.setResizable(false);
        this.setLocation(50, 50);

        this.setLocationRelativeTo(this);
        this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        this.setVisible(true);
    }

    /**
     * Inicializuje menu hlavniho okna
     */   
    private void initMenu(){
        this.MoveHinting = true;
        this.gameDifficulty = GameDifficulty.GAME_NORMAL;
        
        //Create menu bar
        this.mainMenuBar = new JMenuBar();

        //Create each menu
        this.mainMenuGame = new JMenu("Hra");
        this.mainMenuHelp = new JMenu("Nápověda");

        this.mainMenuGame.setMnemonic('H');
        this.mainMenuHelp.setMnemonic('N');

        //Create menu items
        this.mainMenuGameNew = new JMenuItem("Nová");
        this.mainMenuGameSave = new JMenuItem("Uložit");
        this.mainMenuGameReplay = new JMenuItem("Přehrát");
        this.mainMenuGameMoveHinting = new JMenuItem("Nápověda tahů");
        
        this.mainMenuGameQuit = new JMenuItem("Ukončit");
        this.mainMenuHelpAbout = new JMenuItem("O programu");

        this.mainMenuGameNew.setIcon(new ImageIcon(getClass().getResource("/icon_new.png")));
        this.mainMenuGameSave.setIcon(new ImageIcon(getClass().getResource("/icon_save.png")));
        this.mainMenuGameReplay.setIcon(new ImageIcon(getClass().getResource("/icon_replay.png")));
        this.mainMenuGameMoveHinting.setIcon(new ImageIcon(getClass().getResource("/icon_tick.png")));
        this.mainMenuGameQuit.setIcon(new ImageIcon(getClass().getResource("/icon_quit.png")));
        this.mainMenuHelpAbout.setIcon(new ImageIcon(getClass().getResource("/icon_help.png")));

        this.mainMenuGameNew.setAccelerator(KeyStroke.getKeyStroke('N', KeyEvent.CTRL_DOWN_MASK));
        this.mainMenuGameSave.setAccelerator(KeyStroke.getKeyStroke('S', KeyEvent.CTRL_DOWN_MASK));
        this.mainMenuGameReplay.setAccelerator(KeyStroke.getKeyStroke('R', KeyEvent.CTRL_DOWN_MASK));
        this.mainMenuGameQuit.setAccelerator(KeyStroke.getKeyStroke('Q', KeyEvent.CTRL_DOWN_MASK));

        this.mainMenuGameNew.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handleDialogNew(e);
            }
        });

        this.mainMenuGameSave.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handleDialogSave(e);
            }
        });

        this.mainMenuGameReplay.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handleDialogReplay(e);
            }
        });
        
        this.mainMenuGameMoveHinting.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handleMoveHinting(e);
            }
        });

        // Bind close event to Quit
        this.mainMenuGameQuit.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                System.exit(0);
            }
        });

        this.mainMenuHelpAbout.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handleDialogAbout(e);
            }
        });

        this.mainMenuGame.add(this.mainMenuGameNew);
        //this.mainMenuGame.add(this.mainMenuGameLoad);
        this.mainMenuGame.add(this.mainMenuGameSave);
        this.mainMenuGame.add(this.mainMenuGameReplay);
        this.mainMenuGame.add(new JSeparator());
        this.mainMenuGame.add(this.mainMenuGameMoveHinting);
        this.mainMenuGame.add(new JSeparator());
        this.mainMenuGame.add(this.mainMenuGameQuit);
        this.mainMenuHelp.add(this.mainMenuHelpAbout);

        this.mainMenuBar.add(this.mainMenuGame);
        this.mainMenuBar.add(this.mainMenuHelp);

        this.setJMenuBar(this.mainMenuBar);
    }

    /**
     * Inicializuje obsah hlavniho okna
     */   
    private void initContent(){
        this.content = this.getContentPane();
        this.content.removeAll();

        this.battleground = new BattleGroundUI();

        this.content.add(this.battleground);
        
        this.setContentPane(this.content);
        this.pack();
    }

    /**
     * Obsluhuje vytvoreni okna pro novou hru
     */   
    private void handleDialogNew(ActionEvent e){
        DialogNew dialog = new DialogNew(this, true);
        dialog.setLocationRelativeTo(this);

        dialog.setVisible(true);

        if(dialog.isAccepted()){
            GameType gameType = dialog.getGameType();
            Color playerColor = dialog.getPlayerColor();
            String remoteHost = dialog.getRemoteHost();
            int remotePort = dialog.getRemotePort();
            int localPort = dialog.getLocalPort();
            String fileName = dialog.getStoredGameFileName(); 
            GameDifficulty gameDifficulty = dialog.getGameDifficulty();
            
            
            
            this.initContent();
            this.battleground.initGame(gameType, playerColor, remoteHost, remotePort, localPort, fileName);
            this.battleground.setMoveHinting(this.MoveHinting);
            this.battleground.setGameDifficulty(gameDifficulty);
        }
    }

    /**
     * Obsluhuje vytvoreni okna pro ulozeni hry
     */ 
    private void handleDialogSave(ActionEvent e){
        JFileChooser dialog = new JFileChooser();
        
        try{
            File f = new File(new File("./examples/").getCanonicalPath());
            dialog.setCurrentDirectory(f);
        } catch(IOException err){}

        dialog.setAcceptAllFileFilterUsed(false);
        dialog.addChoosableFileFilter(new FileNameExtensionFilter("txt", "txt"));
        dialog.addChoosableFileFilter(new FileNameExtensionFilter("xml", "xml"));

        if(dialog.showSaveDialog(this) == JFileChooser.APPROVE_OPTION){
            String filename = dialog.getSelectedFile().getName();
            String directory = dialog.getCurrentDirectory().toString();
            String extension = dialog.getFileFilter().getDescription();
            String fullPath = directory + "/" + filename;
            
                        
            if(!fullPath.matches(".*\\.txt") || fullPath.matches(".*\\.xml"))
                fullPath += "." + extension;

            try {
                Notation.saveToFile(fullPath, this.battleground.getRounds());

                JOptionPane.showMessageDialog(this, "Hra byla úspěšně uložena", "Queen - Uložení hry", JOptionPane.INFORMATION_MESSAGE);
            } catch (FileNotFoundException ex) {
                JOptionPane.showMessageDialog(this, "Hru se nepodařilo uložit!", "Queen - Chyba při ukládání hry", JOptionPane.ERROR_MESSAGE);
            } catch (IOException io) {
                JOptionPane.showMessageDialog(this, "Hru se nepodařilo uložit!", "Queen - Chyba při ukládání hry", JOptionPane.ERROR_MESSAGE);
            }
        }
    }

    /**
     * Obsluhuje vytvoreni okna pro prehrani hry
     */ 
    private void handleDialogReplay(ActionEvent e){
        Thread thread = new Thread(){
            @Override
            public void run(){
                JFrame dialog = new ReplayUI();
                dialog.setVisible(true);
            }
        };

        thread.start();
    }
    
    /**
     * Obsluhuje nastaveni napovedy tahu
     */ 
    private void handleMoveHinting(ActionEvent e){
        this.MoveHinting = !this.MoveHinting;
        this.mainMenuGameMoveHinting.setIcon(new ImageIcon(getClass().getResource((this.MoveHinting)? "/icon_tick.png" : "/icon_cancel.png")));
        this.battleground.setMoveHinting(this.MoveHinting);
    }

    /**
     * Obsluhuje vytvoreni okna pro napovedu
     */ 
    private void handleDialogAbout(ActionEvent e){
        DialogAbout dialog = new DialogAbout(this, true);
        dialog.setLocationRelativeTo(this);

        dialog.setVisible(true);
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 400, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 300, Short.MAX_VALUE)
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents


    /**
     * Hlavni metoda main projektu
     * @param args
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException | InstantiationException | IllegalAccessException | javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(QueenUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            @Override
            public void run() {
                try {
                    UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
                } catch (ClassNotFoundException | InstantiationException | IllegalAccessException | UnsupportedLookAndFeelException ex) {
                    Logger.getLogger(QueenUI.class.getName()).log(Level.SEVERE, null, ex);
                }

                new QueenUI().setVisible(true);
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    // End of variables declaration//GEN-END:variables
}