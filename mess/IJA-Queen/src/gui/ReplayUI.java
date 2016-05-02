/*
 * Projekt: Queen
 * Predmet: IJA - Seminar Java
 * Soubor: ReplayUI.java
 * Autori:
 *          xkolac12 < xkolac12 @ stud.fit.vutbr.cz >
 *          xmatya03 < xmatya03 @ stud.fit.vutbr.cz >
 * 
 * Trida ReplayUI predstavuje okno pro prehravani zaznamu
 */

package gui;

import gui.basis.*;

import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import queen.basis.Move;

/**
 * @author      Frantisek Kolacek <xkolac12 @ stud.fit.vutbr.cz>
 * @author      Petr Matyas <xmatya03 @ stud.fit.vutbr.cz>
 * @version     0.91
 * @since       2013-04-30
 */
public class ReplayUI extends JFrame {

    private JToolBar mainMenuBar;
    private JButton mainMenuPrev;
    private JButton mainMenuPlay;
    private JButton mainMenuPause;
    private JButton mainMenuStop;
    private JButton mainMenuNext;
    private JButton mainMenuOpen;
    private JButton mainMenuLoad;
    private JTextField mainMenuLastFrame;
    private JButton mainMenuIncrease;
    private JTextField mainMenuInterval;
    private JButton mainMenuDecrease;
    
    private JButton mainMenuHelp;
    private JButton mainMenuQuit;

    private Container content;
    
    private BattleGroundUI battleground;
    
    private int interval;
    private int lastFrame;
    private Vector rounds;
    
    private int roundNumber;
    private Timer roundTimer;
    private boolean roundTimerLock;

    /**
     * Vytvori objekt typu ReplayUI
     */
    public ReplayUI() {
        this.interval = 1;
        this.lastFrame = 0;
        this.rounds = new Vector();
        this.roundNumber = 0;
        this.roundTimer = null;
        this.roundTimerLock = false;
        
        this.initWindow();
        this.initMenu();
        this.initContent();

        this.pack();
    }

    /**
     * Inicializuje okno replay
     */    
    private void initWindow(){
        this.setTitle("Queen - Přehrávač partií");
        this.setIconImage(new ImageIcon(getClass().getResource("/icon.png")).getImage());

        this.setSize(600, 400);
        this.setResizable(false);
        this.setLocation(50, 50);

        this.setLocationRelativeTo(this);
        this.setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);

        this.setVisible(true);
    }

    /**
     * Inicializuje menu replay
     */
    private void initMenu(){
        this.mainMenuBar = new JToolBar();
        this.mainMenuBar.setFloatable(false);


        this.mainMenuPrev = new JButton();
        this.mainMenuPlay = new JButton();
        this.mainMenuPause = new JButton();
        this.mainMenuStop = new JButton();
        this.mainMenuNext = new JButton();
        this.mainMenuOpen = new JButton();
        this.mainMenuLoad = new JButton();
        
        this.mainMenuLastFrame = new JTextField(Integer.toString(this.lastFrame), 1 );
        this.mainMenuLastFrame.setHorizontalAlignment(JTextField.RIGHT);
        this.mainMenuLastFrame.setBackground(this.mainMenuNext.getBackground());
        
        this.mainMenuIncrease = new JButton();
        
        this.mainMenuInterval = new JTextField(Integer.toString(this.interval), 1 );
        this.mainMenuInterval.setEditable(false);
        
        this.mainMenuInterval.setHorizontalAlignment(JTextField.RIGHT);

        this.mainMenuDecrease = new JButton();
        
        this.mainMenuHelp = new JButton();
        this.mainMenuQuit = new JButton();

        this.mainMenuPrev.setIcon(new ImageIcon(getClass().getResource("/replay_prev.png")));
        this.mainMenuPlay.setIcon(new ImageIcon(getClass().getResource("/replay_play.png")));
        this.mainMenuPause.setIcon(new ImageIcon(getClass().getResource("/replay_pause.png")));
        this.mainMenuStop.setIcon(new ImageIcon(getClass().getResource("/replay_stop.png")));
        this.mainMenuNext.setIcon(new ImageIcon(getClass().getResource("/replay_next.png")));
        this.mainMenuOpen.setIcon(new ImageIcon(getClass().getResource("/replay_open.png")));
        this.mainMenuLoad.setIcon(new ImageIcon(getClass().getResource("/replay_load.png")));
        this.mainMenuIncrease.setIcon(new ImageIcon(getClass().getResource("/replay_increase.png")));
        this.mainMenuDecrease.setIcon(new ImageIcon(getClass().getResource("/replay_decrease.png")));
        this.mainMenuHelp.setIcon(new ImageIcon(getClass().getResource("/replay_help.png")));
        this.mainMenuQuit.setIcon(new ImageIcon(getClass().getResource("/replay_quit.png")));
        
        this.mainMenuPrev.setToolTipText("Předchozí tah");
        this.mainMenuPlay.setToolTipText("Přehrát záznam");
        this.mainMenuPause.setToolTipText("Pozastavit záznam");
        this.mainMenuStop.setToolTipText("Zastavit záznam");
        this.mainMenuNext.setToolTipText("Další tah");
        this.mainMenuOpen.setToolTipText("Načíst záznam ze souboru");
        this.mainMenuLoad.setToolTipText("Načíst záznam ze vstupu");
        this.mainMenuLastFrame.setToolTipText("Poslední tah");
        this.mainMenuIncrease.setToolTipText("Zvýšit prodlevu");
        this.mainMenuInterval.setToolTipText("Prodleva");
        this.mainMenuDecrease.setToolTipText("Snížit prodlevu");
        this.mainMenuHelp.setToolTipText("O programu");
        this.mainMenuQuit.setToolTipText("Ukončit");
        
        this.mainMenuBar.add(this.mainMenuPrev);
        this.mainMenuBar.add(this.mainMenuPlay);
        this.mainMenuBar.add(this.mainMenuPause);
        this.mainMenuBar.add(this.mainMenuStop);
        this.mainMenuBar.add(this.mainMenuNext);
        this.mainMenuBar.add(this.mainMenuLastFrame);
        this.mainMenuBar.add(new JSeparator());
        this.mainMenuBar.add(this.mainMenuOpen);
        this.mainMenuBar.add(this.mainMenuLoad);
        this.mainMenuBar.add(new JSeparator());

        this.mainMenuBar.add(this.mainMenuDecrease);
        this.mainMenuBar.add(this.mainMenuInterval);
        this.mainMenuBar.add(this.mainMenuIncrease);        
        
        this.mainMenuBar.add(new JSeparator());
        this.mainMenuBar.add(this.mainMenuHelp);
        this.mainMenuBar.add(this.mainMenuQuit);

        //this.mainMenuOpen.setAccelerator(KeyStroke.getKeyStroke('O', KeyEvent.CTRL_DOWN_MASK));
        //this.mainMenuQuit.setAccelerator(KeyStroke.getKeyStroke('Q', KeyEvent.CTRL_DOWN_MASK));

        this.mainMenuPrev.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handlePrev(e);
            }
        });
        
        this.mainMenuPlay.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handlePlay(e);
            }
        });
        
        this.mainMenuPause.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handlePause(e);
            }
        });
        
        this.mainMenuStop.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handleStop(e);
            }
        });
        
        this.mainMenuNext.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handleNext(e);
            }
        });        
        
        this.mainMenuOpen.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handleDialogOpen(e);
            }
        });
        
        this.mainMenuLoad.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handleDialogLoad(e);
            }
        });
        
        this.mainMenuIncrease.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handleIncrease(e);
            }
        });
        
        this.mainMenuDecrease.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handleDecrease(e);
            }
        });

        this.mainMenuHelp.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                handleDialogAbout(e);
            }
        });

        // Bind close event to Quit
        this.mainMenuQuit.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if(roundTimer != null)
                    roundTimer.stop();
                
                dispose();
            }
        });
    }

    /**
     * Inicializuje obsah okna replay
     */
    private void initContent(){
        this.content = this.getContentPane();
        this.content.removeAll();

        this.content.add(this.mainMenuBar, BorderLayout.NORTH);

        this.battleground = new BattleGroundUI();
        this.battleground.initReplay();

        this.content.add(this.battleground, BorderLayout.SOUTH);
        
        this.setContentPane(this.content);
        this.pack();
    }
    
    private void handlePrev(ActionEvent e){
        if(this.rounds.isEmpty())
            return;
        
        if(this.roundNumber <= 0)
            return;
        
        this.roundNumber--;
        
        int count = this.roundNumber;
        
        this.roundTimerLock = true;
        this.initContent();
        this.roundNumber = 0;
        for(int i = 0 ; i < count; i++)
            this.handleNext(e);            

        this.roundTimerLock = false;
    }
    
    private void handlePlay(ActionEvent e){
        if(this.rounds.isEmpty())
            return;
        
        if(this.roundNumber >= this.rounds.size()){
            if(this.roundTimer != null)
                this.roundTimer.stop();
            
            return;
        }
        
        if(this.roundTimer != null)
            this.roundTimer.stop();
        
        ActionListener taskPerformer = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if(!roundTimerLock)
                    handleNext(e);
            }
        };
        
        this.roundTimer = new Timer(this.interval * 1000, taskPerformer);
        this.roundTimer.start();
    }
    
    private void handlePause(ActionEvent e){
        if(this.roundTimer != null)
            this.roundTimer.stop();
    }
    
    private void handleStop(ActionEvent e){
        if(this.roundTimer != null)
            this.roundTimer.stop();
        
        this.initContent();
        this.roundNumber = 0;
    }
    
    private void handleNext(ActionEvent e){
        if(this.rounds.isEmpty())
            return;  
        
        if(this.roundNumber >= this.rounds.size())
            return;
        
        int tmp;
        try{
            tmp = Integer.parseInt(this.mainMenuLastFrame.getText());
        }
        catch(NumberFormatException err){
            tmp = -1;
        }
        
        if(tmp <= 0 || tmp > (this.rounds.size() / 2 + 1))
            tmp = this.rounds.size() / 2 + 1;
        
        this.mainMenuLastFrame.setText(Integer.toString(tmp));
        
        if(this.roundNumber >= tmp * 2){
            if(this.roundTimer != null)
                this.roundTimer.stop();
            
            return;
            
        }
        
        
        Move round = (Move)this.rounds.get(this.roundNumber);
        this.battleground.getBattleground().move(round.getFrom(), round.getTo());
        this.battleground.reload();
        this.roundNumber++;
    }

    private void handleDialogOpen(ActionEvent e){
            JFileChooser dialog = new JFileChooser();
            
            try{
                File f = new File(new File("./examples/").getCanonicalPath());
                dialog.setCurrentDirectory(f);
            } catch(IOException err){}

            dialog.setAcceptAllFileFilterUsed(false);
            dialog.addChoosableFileFilter(new FileNameExtensionFilter("txt", "txt"));
            dialog.addChoosableFileFilter(new FileNameExtensionFilter("xml", "xml"));

            if(dialog.showOpenDialog(this) == JFileChooser.APPROVE_OPTION){
                String filename = dialog.getSelectedFile().getName();
                String directory = dialog.getCurrentDirectory().toString();
                //String extension = dialog.getFileFilter().getDescription();
                String fullPath = directory + "/" + filename;
                
                try {
                    Notation notation = new Notation();
                    
                    notation.loadFromFile(fullPath);
                    
                    this.rounds = notation.getRounds();
                    this.roundNumber = 0;
                    this.lastFrame = this.rounds.size() / 2 + 1;
                    this.mainMenuLastFrame.setText(Integer.toString(this.lastFrame));
                    
                    // Clear desk
                    this.initContent();
                    
                    JOptionPane.showMessageDialog(this, "Hra byla úspěšně načtena", "Queen - Načtení hry", JOptionPane.INFORMATION_MESSAGE);
                    
                } catch (FileNotFoundException ex) {
                    JOptionPane.showMessageDialog(this, "Hru se nepodařilo načíst!", "Queen - Chyba při načítání hry", JOptionPane.ERROR_MESSAGE);
                } catch (IOException ex) {
                    JOptionPane.showMessageDialog(this, "Hru se nepodařilo načíst!", "Queen - Chyba při načítání hry", JOptionPane.ERROR_MESSAGE);
                }
                
            }

    }
    
    private void handleDialogLoad(ActionEvent e){
        DialogRawInput dialog = new DialogRawInput(this, true);
        dialog.setLocationRelativeTo(this);

        dialog.setVisible(true);
        
        if(dialog.isAccepted()){
            try {
                Notation notation = new Notation();
            
                notation.loadFromRaw(dialog.getInputText());
                
                this.rounds = notation.getRounds();
                this.roundNumber = 0;
                
                
                this.lastFrame = this.rounds.size() / 2 + 1;
                this.mainMenuLastFrame.setText(Integer.toString(this.lastFrame));
                
                // Clear desk
                this.initContent();

                JOptionPane.showMessageDialog(this, "Hra byla úspěšně načtena", "Queen - Načtení hry", JOptionPane.INFORMATION_MESSAGE);
            } catch (IOException ex) {
                JOptionPane.showMessageDialog(this, "Hru se nepodařilo načíst!", "Queen - Chyba při načítání hry", JOptionPane.ERROR_MESSAGE);
            }
        }
    }
    
    private void handleIncrease(ActionEvent e){
        this.interval++;
        
        if(this.interval > 10)
            this.interval = 10;
        
        this.mainMenuInterval.setText(Integer.toString(this.interval));
        
        this.handlePause(e);
        this.handlePlay(e);
    }
    
    private void handleDecrease(ActionEvent e){
        this.interval--;
        
        if(this.interval < 1)
            this.interval = 1;
        
        this.mainMenuInterval.setText(Integer.toString(this.interval));
        
        this.handlePause(e);
        this.handlePlay(e);
    }

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

    // Variables declaration - do not modify//GEN-BEGIN:variables
    // End of variables declaration//GEN-END:variables
}
