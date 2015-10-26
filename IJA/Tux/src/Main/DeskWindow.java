package Main;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import javax.swing.ImageIcon;
import javax.swing.JTabbedPane;
import java.util.List;
import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JTextField;
import javax.swing.filechooser.FileNameExtensionFilter;
        
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.OutputKeys;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;


class Replay extends Thread 
{
    DeskWindow par;
    
    Replay(DeskWindow aThis) 
    {
        this.par = aThis;
    }
    @Override
    public void run() 
    {
        while(!this.par.loaded_move_list.isEmpty())
        {
            this.par.selected = this.par.loaded_move_list.get(0);
            Position to = this.par.loaded_move_list.get(1);
            this.par.move(to);
            this.par.loaded_move_list.remove(0);
            this.par.loaded_move_list.remove(0);
            this.par.draw_board();
            this.par.wait(this.par.replay_interval);
        }
    }
}

class Listen_client extends Thread
{
    DeskWindow par;
    
    Listen_client(DeskWindow aThis) 
    {
        this.par = aThis;
    }
    @Override
    public void run() 
    {
        try {
            this.par.TextArea.setText("Očekávám hru!");
            
            ServerSocket sSocket = new ServerSocket(6666);             
            this.par.socket=sSocket.accept();  

            this.par.TextArea.setText("");
                     
            
            
            OutputStream os = this.par.socket.getOutputStream(); 
            this.par.oos = new ObjectOutputStream(os);
            this.par.oos.flush();
            
            InputStream is = this.par.socket.getInputStream();  
            this.par.ois = new ObjectInputStream(is);  
            
            
            packet_struct to = (packet_struct)this.par.ois.readObject(); 
            
            while(true)
            {
                System.out.println("WAITER CMD:"+to.command);
                if(to.command == 3)
                {
                    this.par.selected = to.from;
                    this.par.move(to.to);   
                    this.par.draw_board();
                    this.par.selected = new Position(-1,-1);
                }
                else if(to.command == 2)
                {
                    if(this.par.on_move == this.par.WHITE)
                        this.par.Enable_white_buttons();
                    else
                        this.par.Enable_black_buttons();
                }
                else if(to.command == 1)
                {
                    this.par.selected = to.from;
                    this.par.move(to.to);   
                    this.par.draw_board();
                    this.par.selected = new Position(-1,-1);
                }
                else if(to.command == 4)
                {
                    break;
                }
                to = (packet_struct)this.par.ois.readObject();
            }
            
            this.par.draw_board();
            this.par.Disable_all_buttons();
            
            //is.close();  
            //this.par.socket.close();  
            //sSocket.close();  
                 
            //if (!sSocket.isClosed())  
            //    sSocket.close();
            
        } catch (IOException ex) {
            Logger.getLogger(Listen_client.class.getName()).log(Level.SEVERE, null, ex);
        } catch (ClassNotFoundException ex) {
            Logger.getLogger(Listen_client.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}

class Connect_client extends Thread 
{
    DeskWindow par;
    
    Connect_client(DeskWindow aThis) 
    {
        this.par = aThis;
    }
    @Override
    public void run() 
    {
        try {
            connect_struct struct;
            Connect_dialog d;

            d = new Connect_dialog(null,true);

            struct = d.showDialog();
  
            // CONNECT NOW!
            this.par.socket = new Socket(struct.IP, struct.port);  
 

            OutputStream os = this.par.socket.getOutputStream(); 
            this.par.oos = new ObjectOutputStream(os);
            this.par.oos.flush();
            
            InputStream is = this.par.socket.getInputStream();  
            this.par.ois = new ObjectInputStream(is);  
            
            
            //OutputStream os = this.par.socket.getOutputStream();  
            //this.par.oos = new ObjectOutputStream(os);
            
            //InputStream is = this.par.socket.getInputStream();  
            //this.par.ois = new ObjectInputStream(is);

            // 1 - iv made a move!
            // 2 - your turn
            // 3 - load move, dont do anythink!
            // 4 - end

            if(!struct.file_path.isEmpty() || !struct.manual_text.isEmpty())
            {

                load_struct l_struc = new load_struct();
                l_struc.file_path = struct.file_path;
                l_struc.file_ext = struct.file_ext;
                l_struc.manual_text = struct.manual_text;
                
                this.par.Load_from_file(l_struc);
                
                
                while(!this.par.loaded_move_list.isEmpty())
                {
                    this.par.selected = this.par.loaded_move_list.get(0);
                    Position to = this.par.loaded_move_list.get(1);
                    
                    this.par.move(to);                  
                    
                    this.par.loaded_move_list.remove(0);
                    this.par.loaded_move_list.remove(0);
                    
                    packet_struct packet = new packet_struct();
                    packet.command = 3;
                    packet.from = this.par.selected;
                    packet.to = to;
                    this.par.oos.writeObject(packet);
                    this.par.oos.flush();
                    
                    this.par.selected = new Position(-1,-1);
                }
                                
                this.par.draw_board();
                this.par.Disable_all_buttons();
                this.par.selected = new Position(-1,-1);
            }
      
            if(this.par.on_move == this.par.WHITE)
                if(struct.host_color == this.par.WHITE)
                {
                    this.par.selected = new Position(-1,-1);
                    this.par.Enable_white_buttons();
                }
                else
                {
                    packet_struct packet = new packet_struct();
                    packet.command = 2;
                    this.par.oos.writeObject(packet);
                    this.par.oos.flush();
                }
            else
                if(struct.host_color == this.par.BLACK)
                {
                    this.par.selected = new Position(-1,-1);
                    this.par.Enable_black_buttons();
                }
                else    
                {
                    packet_struct packet = new packet_struct();
                    packet.command = 2;
                    this.par.oos.writeObject(packet);
                    this.par.oos.flush();
                }
            
            packet_struct to; 
            
            while(true)
            {
                System.out.println("INITIATOR LOOP");
                to = (packet_struct)this.par.ois.readObject();
                
                System.out.println("INITIATOR CMD:"+to.command);
                
                if(to.command == 3)
                {
                    this.par.selected = to.from;
                    this.par.move(to.to);   
                    this.par.draw_board();
                }
                else if(to.command == 2)
                {
                    if(this.par.on_move == this.par.WHITE)
                        this.par.Enable_white_buttons();
                    else
                        this.par.Enable_black_buttons();
                }
                else if(to.command == 1)
                {
                    this.par.selected = to.from;
                    this.par.move(to.to);   
                    this.par.draw_board();
                    this.par.selected = new Position(-1,-1);
                }
                else if(to.command == 4)
                {
                    break;
                }
                
            }
            
            this.par.draw_board();
            this.par.Disable_all_buttons();            
            
        } catch (UnknownHostException ex) {
            Logger.getLogger(Connect_client.class.getName()).log(Level.SEVERE, null, ex);  
        } catch (IOException ex) {
            Logger.getLogger(Connect_client.class.getName()).log(Level.SEVERE, null, ex);
        } catch (ClassNotFoundException ex) {
            Logger.getLogger(Connect_client.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}



public class DeskWindow extends javax.swing.JPanel
{
    private JTabbedPane panel;
    
    final int WHITE = 1;
    final int BLACK = 2;
    
    final int EMPTY = 0;
    final int W_PAWN = 1;
    final int B_PAWN = 2;
    final int W_QUEEN = 3;
    final int B_QUEEN = 4;
    
    public int game_mode;
    public int count_of_moves=1;
    private boolean must_take;
    private boolean at_least_one_can_move=true;
    
    public javax.swing.JButton[][] button_array;
    public int[][] desk;
    
    public Position selected = new Position(-1,-1);
    public List<Position> loaded_move_list = new ArrayList();
    public int replay_interval;
    
    public int on_move = WHITE;
    
    private int white_count = 12;
    private int black_count = 12;
    
    public Socket socket;
    ObjectOutputStream oos;
    ObjectInputStream ois;
            
    private ImageIcon[] img_icon_list;
    
    public DeskWindow(JTabbedPane panel) throws FileNotFoundException, IOException 
    {        
        this.panel = new JTabbedPane();
        this.panel = panel;
        initComponents();
        
        initDesk();
        
        img_icon_list = new ImageIcon[10];
        img_icon_list[0] = new ImageIcon("img/disabled.png");
        img_icon_list[1] = new ImageIcon("img/disabledw.png");
        img_icon_list[2] = new ImageIcon("img/disabledb.png");
        img_icon_list[3] = new ImageIcon("img/disabledwq.png");
        img_icon_list[4] = new ImageIcon("img/disabledbq.png");
        img_icon_list[5] = new ImageIcon("img/enabled.png");
        img_icon_list[6] = new ImageIcon("img/enabledw.png");
        img_icon_list[7] = new ImageIcon("img/enabledb.png");
        img_icon_list[8] = new ImageIcon("img/enabledwq.png");
        img_icon_list[9] = new ImageIcon("img/enabledbq.png");
        
        initButtonList();
        
        draw_board();
                
        Disable_all_buttons();        
    }
    
    public void Load_from_file(load_struct struct) throws FileNotFoundException, IOException
    {
        if(!struct.manual_text.isEmpty() || struct.file_ext.equals("txt"))
        {
            String text="";
            if(struct.manual_text.isEmpty())
            {
                BufferedReader br;
                br = new BufferedReader(new FileReader(struct.file_path));
                try {
                    StringBuilder sb = new StringBuilder();
                    String line = br.readLine();
     
                    while (line != null) 
                    {
                        sb.append(line);
                        sb.append("\n");
                        line = br.readLine();
                    }
                    text = sb.toString()+" ";
                      
                } catch (IOException ex) {
                    Logger.getLogger(DeskWindow.class.getName()).log(Level.SEVERE, null, ex);
                } finally {
                    br.close();
                }
            }
            else
            {
                text = struct.manual_text+" ";
            }
            String text_back = text;
            while(true)
            {
                Position from;
                Position to;
                       
                text = text.substring(str_find(text, " ")+1);
                if(!text.trim().isEmpty())
                {
                    selected = new Position((text.charAt(1)-'0')-1,(text.charAt(0)-'a'));
                    to = new Position((text.charAt(4)-'0')-1,(text.charAt(3)-'a'));

                    if(canMove(to, true).isEmpty())
                    {
                        loaded_move_list.add(selected);
                        loaded_move_list.add(to);
                        move(to);
                    }
                    else
                    {
                        JOptionPane.showMessageDialog(null, "Vámi vybraný soubor je poškozen, nebo byl záměrně Vámi upraven!", "! CHYBA !", JOptionPane.ERROR_MESSAGE);
                        loaded_move_list.clear();
                        break;
                    }
                }
                else
                    break;
                text = text.substring(str_find(text, " ")+1);
                if(!text.trim().isEmpty())
                {
                    selected = new Position((text.charAt(1)-'0')-1,(text.charAt(0)-'a'));
                    to = new Position((text.charAt(4)-'0')-1,(text.charAt(3)-'a'));
                                
                    if(canMove(to, true).isEmpty())
                    {
                        loaded_move_list.add(selected);
                        loaded_move_list.add(to);
                        move(to);
                    }
                    else
                    {
                        JOptionPane.showMessageDialog(null, "Vámi vybraný soubor je poškozen, nebo byl záměrně Vámi upraven!", "! CHYBA !", JOptionPane.ERROR_MESSAGE);
                        loaded_move_list.clear();
                        break;
                    }
                }
                else
                    break;
            }
        }
        else if(struct.file_ext.equals("xml"))
        {
            try{

                DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
                DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
                Document doc = dBuilder.parse(new File(struct.file_path));
                                              
                doc.getDocumentElement().normalize();
                     
                System.out.println("Root element :" + doc.getDocumentElement().getNodeName());
                NodeList nList = doc.getElementsByTagName("partie");
                        
                if(nList.getLength()>0)
                {
                    Node nNode = nList.item(0);
                        
                    if (nNode.getNodeType() == Node.ELEMENT_NODE) 
                    {
                        Element eElement = (Element) nNode;

                        for(int i=0;i<eElement.getElementsByTagName("move").getLength();i++)
                        {
                            String temp = eElement.getElementsByTagName("move").item(i).getTextContent();

                            if(temp.length() == 5)
                            {
                                selected = new Position((temp.charAt(1)-'0')-1,(temp.charAt(0)-'a'));
                                Position to = new Position((temp.charAt(4)-'0')-1,(temp.charAt(3)-'a'));
                                loaded_move_list.add(selected);
                                loaded_move_list.add(to);
                                move(to);               
                            }
                            else
                                throw new Exception("oops");
                        }
                    }
                    else
                        throw new Exception("oops");
                }
                else
                    throw new Exception("oops");
            }
            catch(Exception ex)
            {
                if(ex.getMessage().equals("oops"))
                {
                    Disable_all_buttons();
                    JOptionPane.showMessageDialog(null, "Vámi vybraný soubor je poškozen, nebo byl záměrně Vámi upraven!", "! CHYBA !", JOptionPane.ERROR_MESSAGE);
                }
            }
            catch (Throwable t) {
            t.printStackTrace ();
            }
        }
        
        white_count=12;
        black_count=12;
        count_of_moves = 1;
        TextArea.setText("");
        initDesk();
    }
    
    public void Call_functions(int command) throws FileNotFoundException, IOException 
    {
        if(command == 0) // Player vs Player
        {
            game_mode = 0;
            Enable_white_buttons();
        }
        else if(command == 1) // Player vs PC
        {
            game_mode = 1;
            Enable_white_buttons();
        }
        else if(command == 2) // PC vs Player
        {
            game_mode = 2;
            AI_move(WHITE);
        }
        else if(command == 3 || command == 4) // Nacteni lokalni hry
        {
            load_struct struct;
            Load_game_dialog d;
            
            if(command == 3)
                d = new Load_game_dialog(null,true,false);
            else
                d = new Load_game_dialog(null,true,true);
            
            struct = d.showDialog();
            
            if(command == 4)
                game_mode = 4;
            else if(struct.w_player && struct.b_player)
                game_mode = 0;
            else if(struct.w_player && !struct.b_player)
                game_mode = 1;
            else if(!struct.w_player && struct.b_player)
                game_mode = 2;
            
            
            if(command == 4)
                replay_interval = Integer.parseInt(struct.interval);
            
            Load_from_file(struct);
                
            
        }
        else if(command == 5) // Vytvorit sitovou hru
        {
            Thread t = new Connect_client(this);
            t.start();
            game_mode = 5;
        }    
        else if(command == 6) // Ocekavat sitovou hru
        {
            Thread t = new Listen_client(this);
            t.start();
            game_mode = 5;
        }
        
        
        
        
        if(command == 3) // Nacteni lokalni hry
        {
            while(!loaded_move_list.isEmpty())
            {
                selected = loaded_move_list.get(0);
                Position to = loaded_move_list.get(1);
                move(to);
                loaded_move_list.remove(0);
                loaded_move_list.remove(0);
            }
            
            draw_board();
            Disable_all_buttons();
            selected = new Position(-1,-1);
            
            if(game_mode == 0)
            {
                if(on_move == WHITE)
                    Enable_white_buttons();
                else
                    Enable_black_buttons();
            }
            if(game_mode == 1)
            {
                if(on_move == WHITE)
                    Enable_white_buttons();
                else
                    AI_move(BLACK);
            }
            if(game_mode == 2)
            {
                if(on_move == WHITE)
                    AI_move(WHITE);
                else
                    Enable_black_buttons();
            }
        }
        if(command == 4) // Nacteni lokalni hry nebo prehrani hry
        {
            Thread t1 = new Replay(this);
            t1.start();
            
        }
    }
    
    private void Button_pressed(Position pos) 
    {              
        if(!selected.isSet())
        {
            selected = pos;
            Disable_all_buttons();
            button_array[pos.x][pos.y].setEnabled(true);
            canMove(pos,false);
        }
        else if(selected.equals(pos))
        {        
            draw_board();
            
            if(on_move == WHITE)
                Enable_white_buttons();         
            else
                Enable_black_buttons();
            selected = new Position(-1,-1);
        }
        else
        {
            move(pos);

            
            
            draw_board();
            Disable_all_buttons();
            
            if(!is_win())
            {
                if(game_mode == 0)
                {
                    if(on_move == WHITE)
                        Enable_white_buttons();
                    else
                        Enable_black_buttons();
                }
                if(game_mode == 1)
                    AI_move(BLACK);
                else if(game_mode == 2)
                    AI_move(WHITE);
                else if(game_mode == 5)
                {
                    try {
                        packet_struct packet = new packet_struct();
                        packet.command = 1;
                        packet.from = selected;
                        packet.to = pos;
                        oos.writeObject(packet);
                        System.out.println("sending move!");
                        oos.flush();
                        System.out.println("flush!");
                        
                        packet_struct packet2 = new packet_struct();
                        packet2.command = 2;
                        oos.writeObject(packet2);
                        System.out.println("sending 2!");
                        oos.flush();
                        System.out.println("flush 2!");
                        
                    } catch (IOException ex) {
                        Logger.getLogger(DeskWindow.class.getName()).log(Level.SEVERE, null, ex);
                    }
                }
            }
            
            selected = new Position(-1,-1);
            
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        btn_close = new javax.swing.JButton();
        p71 = new javax.swing.JButton();
        jLabel1 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        p73 = new javax.swing.JButton();
        jLabel5 = new javax.swing.JLabel();
        p75 = new javax.swing.JButton();
        jLabel6 = new javax.swing.JLabel();
        p77 = new javax.swing.JButton();
        p60 = new javax.swing.JButton();
        jLabel11 = new javax.swing.JLabel();
        p62 = new javax.swing.JButton();
        jLabel12 = new javax.swing.JLabel();
        p64 = new javax.swing.JButton();
        p66 = new javax.swing.JButton();
        jLabel13 = new javax.swing.JLabel();
        jLabel14 = new javax.swing.JLabel();
        jLabel7 = new javax.swing.JLabel();
        p40 = new javax.swing.JButton();
        p51 = new javax.swing.JButton();
        jLabel15 = new javax.swing.JLabel();
        jLabel8 = new javax.swing.JLabel();
        p53 = new javax.swing.JButton();
        p42 = new javax.swing.JButton();
        jLabel16 = new javax.swing.JLabel();
        jLabel9 = new javax.swing.JLabel();
        p44 = new javax.swing.JButton();
        p55 = new javax.swing.JButton();
        jLabel17 = new javax.swing.JLabel();
        jLabel10 = new javax.swing.JLabel();
        p46 = new javax.swing.JButton();
        p57 = new javax.swing.JButton();
        jLabel18 = new javax.swing.JLabel();
        jLabel19 = new javax.swing.JLabel();
        p20 = new javax.swing.JButton();
        p31 = new javax.swing.JButton();
        jLabel20 = new javax.swing.JLabel();
        jLabel21 = new javax.swing.JLabel();
        p22 = new javax.swing.JButton();
        p33 = new javax.swing.JButton();
        jLabel22 = new javax.swing.JLabel();
        jLabel23 = new javax.swing.JLabel();
        p24 = new javax.swing.JButton();
        p35 = new javax.swing.JButton();
        jLabel24 = new javax.swing.JLabel();
        jLabel25 = new javax.swing.JLabel();
        p26 = new javax.swing.JButton();
        p37 = new javax.swing.JButton();
        jLabel26 = new javax.swing.JLabel();
        jLabel27 = new javax.swing.JLabel();
        p00 = new javax.swing.JButton();
        p11 = new javax.swing.JButton();
        jLabel28 = new javax.swing.JLabel();
        jLabel29 = new javax.swing.JLabel();
        p02 = new javax.swing.JButton();
        p13 = new javax.swing.JButton();
        jLabel30 = new javax.swing.JLabel();
        jLabel31 = new javax.swing.JLabel();
        p04 = new javax.swing.JButton();
        p15 = new javax.swing.JButton();
        jLabel32 = new javax.swing.JLabel();
        jLabel33 = new javax.swing.JLabel();
        p06 = new javax.swing.JButton();
        p17 = new javax.swing.JButton();
        jLabel34 = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        TextArea = new javax.swing.JTextArea();
        jLabel4 = new javax.swing.JLabel();
        jLabel35 = new javax.swing.JLabel();
        jLabel36 = new javax.swing.JLabel();
        jLabel37 = new javax.swing.JLabel();
        jLabel38 = new javax.swing.JLabel();
        jLabel39 = new javax.swing.JLabel();
        jLabel40 = new javax.swing.JLabel();
        jLabel41 = new javax.swing.JLabel();
        jLabel42 = new javax.swing.JLabel();
        jLabel43 = new javax.swing.JLabel();
        jLabel44 = new javax.swing.JLabel();
        jLabel45 = new javax.swing.JLabel();
        jLabel46 = new javax.swing.JLabel();
        jLabel47 = new javax.swing.JLabel();
        jLabel48 = new javax.swing.JLabel();
        jLabel49 = new javax.swing.JLabel();
        btn_save = new javax.swing.JButton();

        btn_close.setText("X");
        btn_close.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btn_closeActionPerformed(evt);
            }
        });

        p71.setBackground(new java.awt.Color(51, 51, 51));
        p71.setMaximumSize(new java.awt.Dimension(65, 65));
        p71.setMinimumSize(new java.awt.Dimension(65, 65));
        p71.setPreferredSize(new java.awt.Dimension(65, 65));
        p71.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p71ActionPerformed(evt);
            }
        });

        jLabel1.setBackground(new java.awt.Color(255, 255, 255));
        jLabel1.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel3.setBackground(new java.awt.Color(255, 255, 255));
        jLabel3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p73.setBackground(new java.awt.Color(51, 51, 51));
        p73.setMaximumSize(new java.awt.Dimension(65, 65));
        p73.setMinimumSize(new java.awt.Dimension(65, 65));
        p73.setPreferredSize(new java.awt.Dimension(65, 65));
        p73.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p73ActionPerformed(evt);
            }
        });

        jLabel5.setBackground(new java.awt.Color(255, 255, 255));
        jLabel5.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p75.setBackground(new java.awt.Color(51, 51, 51));
        p75.setMaximumSize(new java.awt.Dimension(65, 65));
        p75.setMinimumSize(new java.awt.Dimension(65, 65));
        p75.setPreferredSize(new java.awt.Dimension(65, 65));
        p75.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p75ActionPerformed(evt);
            }
        });

        jLabel6.setBackground(new java.awt.Color(255, 255, 255));
        jLabel6.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p77.setBackground(new java.awt.Color(51, 51, 51));
        p77.setMaximumSize(new java.awt.Dimension(65, 65));
        p77.setMinimumSize(new java.awt.Dimension(65, 65));
        p77.setPreferredSize(new java.awt.Dimension(65, 65));
        p77.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p77ActionPerformed(evt);
            }
        });

        p60.setBackground(new java.awt.Color(51, 51, 51));
        p60.setMaximumSize(new java.awt.Dimension(65, 65));
        p60.setMinimumSize(new java.awt.Dimension(65, 65));
        p60.setPreferredSize(new java.awt.Dimension(65, 65));
        p60.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p60ActionPerformed(evt);
            }
        });

        jLabel11.setBackground(new java.awt.Color(255, 255, 255));
        jLabel11.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p62.setBackground(new java.awt.Color(51, 51, 51));
        p62.setMaximumSize(new java.awt.Dimension(65, 65));
        p62.setMinimumSize(new java.awt.Dimension(65, 65));
        p62.setPreferredSize(new java.awt.Dimension(65, 65));
        p62.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p62ActionPerformed(evt);
            }
        });

        jLabel12.setBackground(new java.awt.Color(255, 255, 255));
        jLabel12.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p64.setBackground(new java.awt.Color(51, 51, 51));
        p64.setMaximumSize(new java.awt.Dimension(65, 65));
        p64.setMinimumSize(new java.awt.Dimension(65, 65));
        p64.setPreferredSize(new java.awt.Dimension(65, 65));
        p64.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p64ActionPerformed(evt);
            }
        });

        p66.setBackground(new java.awt.Color(51, 51, 51));
        p66.setMaximumSize(new java.awt.Dimension(65, 65));
        p66.setMinimumSize(new java.awt.Dimension(65, 65));
        p66.setPreferredSize(new java.awt.Dimension(65, 65));
        p66.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p66ActionPerformed(evt);
            }
        });

        jLabel13.setBackground(new java.awt.Color(255, 255, 255));
        jLabel13.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel14.setBackground(new java.awt.Color(255, 255, 255));
        jLabel14.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel7.setBackground(new java.awt.Color(255, 255, 255));
        jLabel7.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p40.setBackground(new java.awt.Color(51, 51, 51));
        p40.setMaximumSize(new java.awt.Dimension(65, 65));
        p40.setMinimumSize(new java.awt.Dimension(65, 65));
        p40.setPreferredSize(new java.awt.Dimension(65, 65));
        p40.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p40ActionPerformed(evt);
            }
        });

        p51.setBackground(new java.awt.Color(51, 51, 51));
        p51.setMaximumSize(new java.awt.Dimension(65, 65));
        p51.setMinimumSize(new java.awt.Dimension(65, 65));
        p51.setPreferredSize(new java.awt.Dimension(65, 65));
        p51.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p51ActionPerformed(evt);
            }
        });

        jLabel15.setBackground(new java.awt.Color(255, 255, 255));
        jLabel15.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel8.setBackground(new java.awt.Color(255, 255, 255));
        jLabel8.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p53.setBackground(new java.awt.Color(51, 51, 51));
        p53.setMaximumSize(new java.awt.Dimension(65, 65));
        p53.setMinimumSize(new java.awt.Dimension(65, 65));
        p53.setPreferredSize(new java.awt.Dimension(65, 65));
        p53.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p53ActionPerformed(evt);
            }
        });

        p42.setBackground(new java.awt.Color(51, 51, 51));
        p42.setMaximumSize(new java.awt.Dimension(65, 65));
        p42.setMinimumSize(new java.awt.Dimension(65, 65));
        p42.setPreferredSize(new java.awt.Dimension(65, 65));
        p42.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p42ActionPerformed(evt);
            }
        });

        jLabel16.setBackground(new java.awt.Color(255, 255, 255));
        jLabel16.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel9.setBackground(new java.awt.Color(255, 255, 255));
        jLabel9.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p44.setBackground(new java.awt.Color(51, 51, 51));
        p44.setMaximumSize(new java.awt.Dimension(65, 65));
        p44.setMinimumSize(new java.awt.Dimension(65, 65));
        p44.setPreferredSize(new java.awt.Dimension(65, 65));
        p44.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p44ActionPerformed(evt);
            }
        });

        p55.setBackground(new java.awt.Color(51, 51, 51));
        p55.setMaximumSize(new java.awt.Dimension(65, 65));
        p55.setMinimumSize(new java.awt.Dimension(65, 65));
        p55.setPreferredSize(new java.awt.Dimension(65, 65));
        p55.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p55ActionPerformed(evt);
            }
        });

        jLabel17.setBackground(new java.awt.Color(255, 255, 255));
        jLabel17.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel10.setBackground(new java.awt.Color(255, 255, 255));
        jLabel10.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p46.setBackground(new java.awt.Color(51, 51, 51));
        p46.setMaximumSize(new java.awt.Dimension(65, 65));
        p46.setMinimumSize(new java.awt.Dimension(65, 65));
        p46.setPreferredSize(new java.awt.Dimension(65, 65));
        p46.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p46ActionPerformed(evt);
            }
        });

        p57.setBackground(new java.awt.Color(51, 51, 51));
        p57.setMaximumSize(new java.awt.Dimension(65, 65));
        p57.setMinimumSize(new java.awt.Dimension(65, 65));
        p57.setPreferredSize(new java.awt.Dimension(65, 65));
        p57.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p57ActionPerformed(evt);
            }
        });

        jLabel18.setBackground(new java.awt.Color(255, 255, 255));
        jLabel18.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel19.setBackground(new java.awt.Color(255, 255, 255));
        jLabel19.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p20.setBackground(new java.awt.Color(51, 51, 51));
        p20.setMaximumSize(new java.awt.Dimension(65, 65));
        p20.setMinimumSize(new java.awt.Dimension(65, 65));
        p20.setPreferredSize(new java.awt.Dimension(65, 65));
        p20.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p20ActionPerformed(evt);
            }
        });

        p31.setBackground(new java.awt.Color(51, 51, 51));
        p31.setMaximumSize(new java.awt.Dimension(65, 65));
        p31.setMinimumSize(new java.awt.Dimension(65, 65));
        p31.setPreferredSize(new java.awt.Dimension(65, 65));
        p31.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p31ActionPerformed(evt);
            }
        });

        jLabel20.setBackground(new java.awt.Color(255, 255, 255));
        jLabel20.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel21.setBackground(new java.awt.Color(255, 255, 255));
        jLabel21.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p22.setBackground(new java.awt.Color(51, 51, 51));
        p22.setMaximumSize(new java.awt.Dimension(65, 65));
        p22.setMinimumSize(new java.awt.Dimension(65, 65));
        p22.setPreferredSize(new java.awt.Dimension(65, 65));
        p22.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p22ActionPerformed(evt);
            }
        });

        p33.setBackground(new java.awt.Color(51, 51, 51));
        p33.setMaximumSize(new java.awt.Dimension(65, 65));
        p33.setMinimumSize(new java.awt.Dimension(65, 65));
        p33.setPreferredSize(new java.awt.Dimension(65, 65));
        p33.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p33ActionPerformed(evt);
            }
        });

        jLabel22.setBackground(new java.awt.Color(255, 255, 255));
        jLabel22.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel23.setBackground(new java.awt.Color(255, 255, 255));
        jLabel23.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p24.setBackground(new java.awt.Color(51, 51, 51));
        p24.setMaximumSize(new java.awt.Dimension(65, 65));
        p24.setMinimumSize(new java.awt.Dimension(65, 65));
        p24.setPreferredSize(new java.awt.Dimension(65, 65));
        p24.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p24ActionPerformed(evt);
            }
        });

        p35.setBackground(new java.awt.Color(51, 51, 51));
        p35.setMaximumSize(new java.awt.Dimension(65, 65));
        p35.setMinimumSize(new java.awt.Dimension(65, 65));
        p35.setPreferredSize(new java.awt.Dimension(65, 65));
        p35.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p35ActionPerformed(evt);
            }
        });

        jLabel24.setBackground(new java.awt.Color(255, 255, 255));
        jLabel24.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel25.setBackground(new java.awt.Color(255, 255, 255));
        jLabel25.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p26.setBackground(new java.awt.Color(51, 51, 51));
        p26.setMaximumSize(new java.awt.Dimension(65, 65));
        p26.setMinimumSize(new java.awt.Dimension(65, 65));
        p26.setPreferredSize(new java.awt.Dimension(65, 65));
        p26.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p26ActionPerformed(evt);
            }
        });

        p37.setBackground(new java.awt.Color(51, 51, 51));
        p37.setMaximumSize(new java.awt.Dimension(65, 65));
        p37.setMinimumSize(new java.awt.Dimension(65, 65));
        p37.setPreferredSize(new java.awt.Dimension(65, 65));
        p37.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p37ActionPerformed(evt);
            }
        });

        jLabel26.setBackground(new java.awt.Color(255, 255, 255));
        jLabel26.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel27.setBackground(new java.awt.Color(255, 255, 255));
        jLabel27.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p00.setBackground(new java.awt.Color(51, 51, 51));
        p00.setMaximumSize(new java.awt.Dimension(65, 65));
        p00.setMinimumSize(new java.awt.Dimension(65, 65));
        p00.setPreferredSize(new java.awt.Dimension(65, 65));
        p00.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p00ActionPerformed(evt);
            }
        });

        p11.setBackground(new java.awt.Color(51, 51, 51));
        p11.setMaximumSize(new java.awt.Dimension(65, 65));
        p11.setMinimumSize(new java.awt.Dimension(65, 65));
        p11.setPreferredSize(new java.awt.Dimension(65, 65));
        p11.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p11ActionPerformed(evt);
            }
        });

        jLabel28.setBackground(new java.awt.Color(255, 255, 255));
        jLabel28.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel29.setBackground(new java.awt.Color(255, 255, 255));
        jLabel29.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p02.setBackground(new java.awt.Color(51, 51, 51));
        p02.setMaximumSize(new java.awt.Dimension(65, 65));
        p02.setMinimumSize(new java.awt.Dimension(65, 65));
        p02.setPreferredSize(new java.awt.Dimension(65, 65));
        p02.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p02ActionPerformed(evt);
            }
        });

        p13.setBackground(new java.awt.Color(51, 51, 51));
        p13.setMaximumSize(new java.awt.Dimension(65, 65));
        p13.setMinimumSize(new java.awt.Dimension(65, 65));
        p13.setPreferredSize(new java.awt.Dimension(65, 65));
        p13.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p13ActionPerformed(evt);
            }
        });

        jLabel30.setBackground(new java.awt.Color(255, 255, 255));
        jLabel30.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel31.setBackground(new java.awt.Color(255, 255, 255));
        jLabel31.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p04.setBackground(new java.awt.Color(51, 51, 51));
        p04.setMaximumSize(new java.awt.Dimension(65, 65));
        p04.setMinimumSize(new java.awt.Dimension(65, 65));
        p04.setPreferredSize(new java.awt.Dimension(65, 65));
        p04.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p04ActionPerformed(evt);
            }
        });

        p15.setBackground(new java.awt.Color(51, 51, 51));
        p15.setMaximumSize(new java.awt.Dimension(65, 65));
        p15.setMinimumSize(new java.awt.Dimension(65, 65));
        p15.setPreferredSize(new java.awt.Dimension(65, 65));
        p15.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p15ActionPerformed(evt);
            }
        });

        jLabel32.setBackground(new java.awt.Color(255, 255, 255));
        jLabel32.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        jLabel33.setBackground(new java.awt.Color(255, 255, 255));
        jLabel33.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        p06.setBackground(new java.awt.Color(51, 51, 51));
        p06.setMaximumSize(new java.awt.Dimension(65, 65));
        p06.setMinimumSize(new java.awt.Dimension(65, 65));
        p06.setPreferredSize(new java.awt.Dimension(65, 65));
        p06.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p06ActionPerformed(evt);
            }
        });

        p17.setBackground(new java.awt.Color(51, 51, 51));
        p17.setMaximumSize(new java.awt.Dimension(65, 65));
        p17.setMinimumSize(new java.awt.Dimension(65, 65));
        p17.setPreferredSize(new java.awt.Dimension(65, 65));
        p17.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                p17ActionPerformed(evt);
            }
        });

        jLabel34.setBackground(new java.awt.Color(255, 255, 255));
        jLabel34.setIcon(new javax.swing.ImageIcon(getClass().getResource("/img/white_back.png"))); // NOI18N

        TextArea.setColumns(20);
        TextArea.setRows(5);
        jScrollPane1.setViewportView(TextArea);

        jLabel4.setText("a");

        jLabel35.setText("b");

        jLabel36.setText("c");

        jLabel37.setText("d");

        jLabel38.setText("e");

        jLabel39.setText("f");

        jLabel40.setText("g");

        jLabel41.setText("h");

        jLabel42.setText("1");

        jLabel43.setText("2");

        jLabel44.setText("4");

        jLabel45.setText("3");

        jLabel46.setText("5");

        jLabel47.setText("6");

        jLabel48.setText("7");

        jLabel49.setText("8");

        btn_save.setText("Uložit");
        btn_save.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btn_saveActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel42, javax.swing.GroupLayout.PREFERRED_SIZE, 8, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel43, javax.swing.GroupLayout.PREFERRED_SIZE, 8, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel45, javax.swing.GroupLayout.PREFERRED_SIZE, 8, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel44, javax.swing.GroupLayout.PREFERRED_SIZE, 8, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel46, javax.swing.GroupLayout.PREFERRED_SIZE, 8, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel47, javax.swing.GroupLayout.PREFERRED_SIZE, 8, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel48, javax.swing.GroupLayout.PREFERRED_SIZE, 8, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel49, javax.swing.GroupLayout.PREFERRED_SIZE, 8, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(jLabel14)
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jLabel1)
                                .addGap(6, 6, 6)
                                .addComponent(p71, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jLabel3)
                                .addGap(6, 6, 6)
                                .addComponent(p73, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jLabel5)
                                .addGap(6, 6, 6)
                                .addComponent(p75, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jLabel6)
                                .addGap(6, 6, 6)
                                .addComponent(p77, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(btn_save, javax.swing.GroupLayout.PREFERRED_SIZE, 87, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(btn_close)
                        .addContainerGap())
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addGroup(layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(layout.createSequentialGroup()
                                        .addComponent(p40, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel15)
                                        .addGap(6, 6, 6)
                                        .addComponent(p42, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel16)
                                        .addGap(6, 6, 6)
                                        .addComponent(p44, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel17)
                                        .addGap(6, 6, 6)
                                        .addComponent(p46, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                        .addComponent(jLabel18)
                                        .addGroup(layout.createSequentialGroup()
                                            .addComponent(jLabel7)
                                            .addGap(6, 6, 6)
                                            .addComponent(p51, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                            .addComponent(jLabel8)
                                            .addGap(6, 6, 6)
                                            .addComponent(p53, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                            .addComponent(jLabel9)
                                            .addGap(6, 6, 6)
                                            .addComponent(p55, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                            .addComponent(jLabel10)
                                            .addGap(6, 6, 6)
                                            .addComponent(p57, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                    .addGroup(layout.createSequentialGroup()
                                        .addComponent(p20, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel20)
                                        .addGap(6, 6, 6)
                                        .addComponent(p22, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel22)
                                        .addGap(6, 6, 6)
                                        .addComponent(p24, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel24)
                                        .addGap(6, 6, 6)
                                        .addComponent(p26, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                        .addComponent(jLabel26)
                                        .addGroup(layout.createSequentialGroup()
                                            .addComponent(jLabel19)
                                            .addGap(6, 6, 6)
                                            .addComponent(p31, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                            .addComponent(jLabel21)
                                            .addGap(6, 6, 6)
                                            .addComponent(p33, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                            .addComponent(jLabel23)
                                            .addGap(6, 6, 6)
                                            .addComponent(p35, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                            .addComponent(jLabel25)
                                            .addGap(6, 6, 6)
                                            .addComponent(p37, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                    .addGroup(layout.createSequentialGroup()
                                        .addComponent(p00, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel28)
                                        .addGap(6, 6, 6)
                                        .addComponent(p02, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel30)
                                        .addGap(6, 6, 6)
                                        .addComponent(p04, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel32)
                                        .addGap(6, 6, 6)
                                        .addComponent(p06, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                    .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                        .addComponent(jLabel34)
                                        .addGroup(layout.createSequentialGroup()
                                            .addComponent(jLabel27)
                                            .addGap(6, 6, 6)
                                            .addComponent(p11, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                            .addComponent(jLabel29)
                                            .addGap(6, 6, 6)
                                            .addComponent(p13, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                            .addComponent(jLabel31)
                                            .addGap(6, 6, 6)
                                            .addComponent(p15, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                            .addComponent(jLabel33)
                                            .addGap(6, 6, 6)
                                            .addComponent(p17, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                    .addGroup(layout.createSequentialGroup()
                                        .addComponent(p60, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel11)
                                        .addGap(6, 6, 6)
                                        .addComponent(p62, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel12)
                                        .addGap(6, 6, 6)
                                        .addComponent(p64, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel13)
                                        .addGap(6, 6, 6)
                                        .addComponent(p66, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED))
                            .addGroup(layout.createSequentialGroup()
                                .addGap(26, 26, 26)
                                .addComponent(jLabel4)
                                .addGap(67, 67, 67)
                                .addComponent(jLabel35)
                                .addGap(64, 64, 64)
                                .addComponent(jLabel36)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(jLabel37)
                                .addGap(61, 61, 61)
                                .addComponent(jLabel38)
                                .addGap(65, 65, 65)
                                .addComponent(jLabel39)
                                .addGap(67, 67, 67)
                                .addComponent(jLabel40)
                                .addGap(62, 62, 62)
                                .addComponent(jLabel41)
                                .addGap(35, 35, 35)))
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(0, 12, Short.MAX_VALUE))))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(btn_close)
                            .addComponent(btn_save))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jScrollPane1)
                        .addContainerGap())
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addGroup(layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(p77, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel6)
                                    .addComponent(p75, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel5)
                                    .addComponent(p73, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel3)
                                    .addComponent(p71, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel1))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(p66, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel13)
                                    .addComponent(p64, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel12)
                                    .addComponent(p62, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel11)
                                    .addComponent(p60, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel14))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(p57, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel10)
                                    .addComponent(p55, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel9)
                                    .addComponent(p53, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel8)
                                    .addComponent(p51, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel7))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(p46, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel17)
                                    .addComponent(p44, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel16)
                                    .addComponent(p42, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel15)
                                    .addComponent(p40, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel18))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(p37, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel25)
                                    .addComponent(p35, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel23)
                                    .addComponent(p33, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel21)
                                    .addComponent(p31, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel19))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(p26, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel24)
                                    .addComponent(p24, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel22)
                                    .addComponent(p22, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel20)
                                    .addComponent(p20, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel26))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(p17, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel33)
                                    .addComponent(p15, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel31)
                                    .addComponent(p13, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel29)
                                    .addComponent(p11, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel27))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(p06, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel32)
                                    .addComponent(p04, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel30)
                                    .addComponent(p02, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel28)
                                    .addComponent(jLabel34)
                                    .addComponent(p00, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(jLabel4)
                                    .addComponent(jLabel35)
                                    .addComponent(jLabel36)
                                    .addComponent(jLabel37)
                                    .addComponent(jLabel38)
                                    .addComponent(jLabel39)
                                    .addComponent(jLabel40)
                                    .addComponent(jLabel41)))
                            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                                .addGap(25, 25, 25)
                                .addComponent(jLabel49)
                                .addGap(54, 54, 54)
                                .addComponent(jLabel48)
                                .addGap(58, 58, 58)
                                .addComponent(jLabel47)
                                .addGap(52, 52, 52)
                                .addComponent(jLabel46)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(jLabel44)
                                .addGap(55, 55, 55)
                                .addComponent(jLabel45)
                                .addGap(55, 55, 55)
                                .addComponent(jLabel43)
                                .addGap(62, 62, 62)
                                .addComponent(jLabel42)
                                .addGap(44, 44, 44)))
                        .addGap(0, 0, Short.MAX_VALUE))))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void btn_closeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btn_closeActionPerformed
        panel.remove(panel.getSelectedComponent());
    }//GEN-LAST:event_btn_closeActionPerformed

    private void p00ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p00ActionPerformed
        Button_pressed(new Position(0,0)); 
    }//GEN-LAST:event_p00ActionPerformed

    private void p02ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p02ActionPerformed
        Button_pressed(new Position(0,2)); 
    }//GEN-LAST:event_p02ActionPerformed

    private void p04ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p04ActionPerformed
        Button_pressed(new Position(0,4)); 
    }//GEN-LAST:event_p04ActionPerformed

    private void p06ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p06ActionPerformed
        Button_pressed(new Position(0,6)); 
    }//GEN-LAST:event_p06ActionPerformed

    private void p11ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p11ActionPerformed
        Button_pressed(new Position(1,1)); 
    }//GEN-LAST:event_p11ActionPerformed

    private void p13ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p13ActionPerformed
        Button_pressed(new Position(1,3)); 
    }//GEN-LAST:event_p13ActionPerformed

    private void p15ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p15ActionPerformed
        Button_pressed(new Position(1,5)); 
    }//GEN-LAST:event_p15ActionPerformed

    private void p17ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p17ActionPerformed
        Button_pressed(new Position(1,7)); 
    }//GEN-LAST:event_p17ActionPerformed

    private void p20ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p20ActionPerformed
        Button_pressed(new Position(2,0)); 
    }//GEN-LAST:event_p20ActionPerformed

    private void p22ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p22ActionPerformed
        Button_pressed(new Position(2,2)); 
    }//GEN-LAST:event_p22ActionPerformed

    private void p24ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p24ActionPerformed
        Button_pressed(new Position(2,4)); 
    }//GEN-LAST:event_p24ActionPerformed

    private void p26ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p26ActionPerformed
        Button_pressed(new Position(2,6)); 
    }//GEN-LAST:event_p26ActionPerformed

    private void p31ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p31ActionPerformed
        Button_pressed(new Position(3,1)); 
    }//GEN-LAST:event_p31ActionPerformed

    private void p33ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p33ActionPerformed
        Button_pressed(new Position(3,3)); 
    }//GEN-LAST:event_p33ActionPerformed

    private void p35ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p35ActionPerformed
        Button_pressed(new Position(3,5)); 
    }//GEN-LAST:event_p35ActionPerformed

    private void p37ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p37ActionPerformed
        Button_pressed(new Position(3,7)); 
    }//GEN-LAST:event_p37ActionPerformed

    private void p40ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p40ActionPerformed
        Button_pressed(new Position(4,0)); 
    }//GEN-LAST:event_p40ActionPerformed

    private void p42ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p42ActionPerformed
        Button_pressed(new Position(4,2)); 
    }//GEN-LAST:event_p42ActionPerformed

    private void p44ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p44ActionPerformed
        Button_pressed(new Position(4,4)); 
    }//GEN-LAST:event_p44ActionPerformed

    private void p46ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p46ActionPerformed
        Button_pressed(new Position(4,6)); 
    }//GEN-LAST:event_p46ActionPerformed

    private void p51ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p51ActionPerformed
        Button_pressed(new Position(5,1)); 
    }//GEN-LAST:event_p51ActionPerformed

    private void p53ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p53ActionPerformed
        Button_pressed(new Position(5,3)); 
    }//GEN-LAST:event_p53ActionPerformed

    private void p55ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p55ActionPerformed
        Button_pressed(new Position(5,5)); 
    }//GEN-LAST:event_p55ActionPerformed

    private void p57ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p57ActionPerformed
        Button_pressed(new Position(5,7)); 
    }//GEN-LAST:event_p57ActionPerformed

    private void p60ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p60ActionPerformed
        Button_pressed(new Position(6,0)); 
    }//GEN-LAST:event_p60ActionPerformed

    private void p62ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p62ActionPerformed
        Button_pressed(new Position(6,2)); 
    }//GEN-LAST:event_p62ActionPerformed

    private void p64ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p64ActionPerformed
        Button_pressed(new Position(6,4)); 
    }//GEN-LAST:event_p64ActionPerformed

    private void p66ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p66ActionPerformed
        Button_pressed(new Position(6,6)); 
    }//GEN-LAST:event_p66ActionPerformed

    private void p71ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p71ActionPerformed
        Button_pressed(new Position(7,1)); 
    }//GEN-LAST:event_p71ActionPerformed

    private void p73ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p73ActionPerformed
        Button_pressed(new Position(7,3)); 
    }//GEN-LAST:event_p73ActionPerformed

    private void p75ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p75ActionPerformed
        Button_pressed(new Position(7,5)); 
    }//GEN-LAST:event_p75ActionPerformed

    private void p77ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_p77ActionPerformed
        Button_pressed(new Position(7,7)); 
    }//GEN-LAST:event_p77ActionPerformed

    private void btn_saveActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btn_saveActionPerformed
      
        JFileChooser c = new JFileChooser();
     
        c.setAcceptAllFileFilterUsed(false);
        
        c.addChoosableFileFilter(new FileNameExtensionFilter("txt","txt"));  
        c.addChoosableFileFilter(new FileNameExtensionFilter("xml","xml"));  
        
        int rVal = c.showSaveDialog(this);
        JTextField filename = new JTextField(), dir = new JTextField();
        
        
        if (rVal == JFileChooser.APPROVE_OPTION) 
        {
            filename.setText(c.getSelectedFile().getName());
            dir.setText(c.getCurrentDirectory().toString());
            String extension = c.getFileFilter().getDescription();

            String full_name = dir.getText() + "/" + filename.getText()+"."+extension;
            
            try
            { 
                
                String text = TextArea.getText();
                if(text.endsWith(". "))
                {           
                    
                    while(!text.endsWith("\n") && !text.isEmpty())
                        text = text.substring(0, text.length()-1);                   
                }
                if(extension.equals("txt"))
                {
                    FileWriter fstream = new FileWriter(full_name);
                    BufferedWriter out = new BufferedWriter(fstream);
                    out.write(text);
                    out.close();
                }
                else
                {
                    DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
                    DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
                    
                    Document doc = docBuilder.newDocument();
                    Element rootElement = doc.createElement("partie");
                    doc.appendChild(rootElement);
                    
                    
                    if(text.isEmpty())
                    {
                        JOptionPane.showMessageDialog(null, "Hru musíte nejprve rozehrát!", "POZOR!", JOptionPane.INFORMATION_MESSAGE);
                    }
                    else
                    {
                        text = text.substring(str_find(text," ")+1);
                        System.out.println(":::"+text+":::");
                        while(!text.isEmpty())
                        {
                            String move;

                            if(str_find(text, " ") != -1)
                                move = text.substring(0, str_find(text," "));
                            else
                                break;
                            System.out.println(":::"+move+":::");
                            Element e;
                            e = doc.createElement("move");
                            e.appendChild(doc.createTextNode(move));
                            rootElement.appendChild(e);

                            text = text.substring(str_find(text," ")+1);
                            if(str_find(text, "\n") != -1)
                                move = text.substring(0, str_find(text,"\n"));
                            else
                                break;

                            e = null;
                            e = doc.createElement("move");
                            e.appendChild(doc.createTextNode(move));
                            rootElement.appendChild(e);

                            if(str_find(text, " ") != -1)
                                text = text.substring(str_find(text," ")+1);
                            else
                                break;
                        }

                        TransformerFactory transformerFactory = TransformerFactory.newInstance();
                        Transformer transformer = transformerFactory.newTransformer();
                        transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "2");
                        transformer.setOutputProperty(OutputKeys.INDENT, "yes");
                        DOMSource source = new DOMSource(doc);
                        StreamResult result = new StreamResult(new File(full_name));
                        transformer.transform(source, result);            
                    }
                }
                
            }catch (Exception e)
            {
                System.err.println("Error: " + e.getMessage());
            }
        
        
        
        }
      
    }//GEN-LAST:event_btn_saveActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    public javax.swing.JTextArea TextArea;
    private javax.swing.JButton btn_close;
    private javax.swing.JButton btn_save;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel10;
    private javax.swing.JLabel jLabel11;
    private javax.swing.JLabel jLabel12;
    private javax.swing.JLabel jLabel13;
    private javax.swing.JLabel jLabel14;
    private javax.swing.JLabel jLabel15;
    private javax.swing.JLabel jLabel16;
    private javax.swing.JLabel jLabel17;
    private javax.swing.JLabel jLabel18;
    private javax.swing.JLabel jLabel19;
    private javax.swing.JLabel jLabel20;
    private javax.swing.JLabel jLabel21;
    private javax.swing.JLabel jLabel22;
    private javax.swing.JLabel jLabel23;
    private javax.swing.JLabel jLabel24;
    private javax.swing.JLabel jLabel25;
    private javax.swing.JLabel jLabel26;
    private javax.swing.JLabel jLabel27;
    private javax.swing.JLabel jLabel28;
    private javax.swing.JLabel jLabel29;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel30;
    private javax.swing.JLabel jLabel31;
    private javax.swing.JLabel jLabel32;
    private javax.swing.JLabel jLabel33;
    private javax.swing.JLabel jLabel34;
    private javax.swing.JLabel jLabel35;
    private javax.swing.JLabel jLabel36;
    private javax.swing.JLabel jLabel37;
    private javax.swing.JLabel jLabel38;
    private javax.swing.JLabel jLabel39;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel40;
    private javax.swing.JLabel jLabel41;
    private javax.swing.JLabel jLabel42;
    private javax.swing.JLabel jLabel43;
    private javax.swing.JLabel jLabel44;
    private javax.swing.JLabel jLabel45;
    private javax.swing.JLabel jLabel46;
    private javax.swing.JLabel jLabel47;
    private javax.swing.JLabel jLabel48;
    private javax.swing.JLabel jLabel49;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JLabel jLabel7;
    private javax.swing.JLabel jLabel8;
    private javax.swing.JLabel jLabel9;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JButton p00;
    private javax.swing.JButton p02;
    private javax.swing.JButton p04;
    private javax.swing.JButton p06;
    private javax.swing.JButton p11;
    private javax.swing.JButton p13;
    private javax.swing.JButton p15;
    private javax.swing.JButton p17;
    private javax.swing.JButton p20;
    private javax.swing.JButton p22;
    private javax.swing.JButton p24;
    private javax.swing.JButton p26;
    private javax.swing.JButton p31;
    private javax.swing.JButton p33;
    private javax.swing.JButton p35;
    private javax.swing.JButton p37;
    private javax.swing.JButton p40;
    private javax.swing.JButton p42;
    private javax.swing.JButton p44;
    private javax.swing.JButton p46;
    private javax.swing.JButton p51;
    private javax.swing.JButton p53;
    private javax.swing.JButton p55;
    private javax.swing.JButton p57;
    private javax.swing.JButton p60;
    private javax.swing.JButton p62;
    private javax.swing.JButton p64;
    private javax.swing.JButton p66;
    private javax.swing.JButton p71;
    private javax.swing.JButton p73;
    private javax.swing.JButton p75;
    private javax.swing.JButton p77;
    // End of variables declaration//GEN-END:variables
    
    private void initDesk() // done
    {
        desk = new int[8][8];
        
        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++)
            {
                // --------------------- Nastavime neexistujici policka (bile)
                desk[i][j] = 0;
                if( i%2 == 1)
                    if(j%2 == 0)
                        desk[i][j] = -1;
                
                if( i%2 == 0)
                    if(j%2 == 1)
                        desk[i][j] = -1;
                
                // --------------------- Nastavime pocatecni figurky.
                if( i%2 == 0 )
                {
                    if(j%2 == 0)
                        if(i >= 0 && i < 3)
                           desk[i][j] = WHITE;         // BILA figurka
                        else if( i >=5 && i < 8 )  
                            desk[i][j] = BLACK;        // CERNA figurka
                }
                else
                {
                    if(j%2 == 1)
                        if(i >= 0 && i < 3)
                            desk[i][j] = WHITE;        // BILA figurka
                        else if( i >=5 && i < 8 )  
                            desk[i][j] = BLACK;        // CERNA figurka
                }
            }
    }
    
    private void initButtonList() // done
    {
        button_array = new javax.swing.JButton[8][8];
        button_array[0][0] = p00;
        button_array[0][1] = null;
        button_array[0][2] = p02;
        button_array[0][3] = null;
        button_array[0][4] = p04;
        button_array[0][5] = null;
        button_array[0][6] = p06;
        button_array[0][7] = null;
        
        button_array[1][0] = null;
        button_array[1][1] = p11;
        button_array[1][2] = null;
        button_array[1][3] = p13;
        button_array[1][4] = null;
        button_array[1][5] = p15;
        button_array[1][6] = null;
        button_array[1][7] = p17;
        
        button_array[2][0] = p20;
        button_array[2][1] = null;
        button_array[2][2] = p22;
        button_array[2][3] = null;
        button_array[2][4] = p24;
        button_array[2][5] = null;
        button_array[2][6] = p26;
        button_array[2][7] = null;
        
        button_array[3][0] = null;
        button_array[3][1] = p31;
        button_array[3][2] = null;
        button_array[3][3] = p33;
        button_array[3][4] = null;
        button_array[3][5] = p35;
        button_array[3][6] = null;
        button_array[3][7] = p37;
        
        button_array[4][0] = p40;
        button_array[4][1] = null;
        button_array[4][2] = p42;
        button_array[4][3] = null;
        button_array[4][4] = p44;
        button_array[4][5] = null;
        button_array[4][6] = p46;
        button_array[4][7] = null;
        
        button_array[5][0] = null;
        button_array[5][1] = p51;
        button_array[5][2] = null;
        button_array[5][3] = p53;
        button_array[5][4] = null;
        button_array[5][5] = p55;
        button_array[5][6] = null;
        button_array[5][7] = p57;
        
        button_array[6][0] = p60;
        button_array[6][1] = null;
        button_array[6][2] = p62;
        button_array[6][3] = null;
        button_array[6][4] = p64;
        button_array[6][5] = null;
        button_array[6][6] = p66;
        button_array[6][7] = null;
        
        button_array[7][0] = null;
        button_array[7][1] = p71;
        button_array[7][2] = null;
        button_array[7][3] = p73;
        button_array[7][4] = null;
        button_array[7][5] = p75;
        button_array[7][6] = null;
        button_array[7][7] = p77;
        
    }
    
    public void Disable_all_buttons() // done
    {                                
        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++)
                if(button_array[i][j] != null)
                {
                    button_array[i][j].setEnabled(false);
                }
    }
    public void Enable_white_buttons() // done
    {      
        List<Position> pos_list;
        pos_list = must_move(WHITE);
        if(!is_pat())
        {
            if(pos_list.isEmpty())
            {
                for(int i=0;i<8;i++)
                    for(int j=0;j<8;j++)
                        if(desk[i][j] != 0 && desk[i][j]%2 == 1)
                        {
                            button_array[i][j].setEnabled(true);
                        }
            }
            else
            {
                for(int i=0;i<pos_list.size();i++)
                    button_array[pos_list.get(i).x][pos_list.get(i).y].setEnabled(true);
            }
        }      
    }
    public void Enable_black_buttons() // done
    {                                
        List<Position> pos_list;
        pos_list = must_move(BLACK);
        if(!is_pat())
        {
            if(pos_list.isEmpty())
            {
                for(int i=0;i<8;i++)
                    for(int j=0;j<8;j++)
                        if(desk[i][j] != 0 && desk[i][j]%2 == 0)
                        {
                            button_array[i][j].setEnabled(true);
                        }
            }
            else
            {
                for(int i=0;i<pos_list.size();i++)
                    button_array[pos_list.get(i).x][pos_list.get(i).y].setEnabled(true);
            }
        }
    }
    
    public void draw_board() // done
    {                                
        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++)
                if(desk[i][j] != -1)
                {
                    button_array[i][j].setIcon(img_icon_list[desk[i][j]+5]);
                    button_array[i][j].setDisabledIcon(img_icon_list[desk[i][j]]);                    
                }
    }
    
    
    public void move(Position pos) // done
    {
        int dif_x = pos.x - selected.x;
        int dif_y = pos.y - selected.y; 
        int diff_x=dif_x;
        int diff_y=dif_y;
        
        if(dif_x < 0)
            diff_x = dif_x * -1;
        if(dif_y < 0)
            diff_y = dif_y * -1;
         
        if(diff_x >= 2)
        {
            if(dif_x >= 2 && dif_y >= 2)
            {
                for(int i=1;i<diff_x;i++)
                    remove_figure(new Position(pos.x-i,pos.y-i));
            }
            else if(dif_x >= 2 && dif_y <= -2)
            {
                for(int i=1;i<diff_x;i++)
                    remove_figure(new Position(pos.x-i,pos.y+i));
            }
            else if(dif_x <= -2 && dif_y >= 2)
            {
                for(int i=1;i<diff_x;i++)
                    remove_figure(new Position(pos.x+i,pos.y-i));
            }
            else if(dif_x <= -2 && dif_y <= -2)
            {
                for(int i=1;i<diff_x;i++)
                    remove_figure(new Position(pos.x+i,pos.y+i));
            }
        }
        
        make_move(pos);
        

        if(must_take)
            if(on_move == WHITE)
            {
                TextArea.append((char)((char)selected.y+'a')+""+(selected.x+1)+"x"+(char)((char)pos.y+'a')+""+(pos.x+1));
                count_of_moves++;
            }
            else
            {
                if(count_of_moves == 1)
                    TextArea.append(count_of_moves+". "+(char)((char)selected.y+'a')+""+(selected.x+1)+"x"+(char)((char)pos.y+'a')+""+(pos.x+1)+" ");
                else
                    TextArea.append("\n"+count_of_moves+". "+(char)((char)selected.y+'a')+""+(selected.x+1)+"x"+(char)((char)pos.y+'a')+""+(pos.x+1)+" ");
            }
        else
            if(on_move == WHITE)
            {
                TextArea.append((char)((char)selected.y+'a')+""+(selected.x+1)+"-"+(char)((char)pos.y+'a')+""+(pos.x+1));
                count_of_moves++;
            }
            else
            {    
                if(count_of_moves == 1)
                    TextArea.append(count_of_moves+". "+(char)((char)selected.y+'a')+""+(selected.x+1)+"-"+(char)((char)pos.y+'a')+""+(pos.x+1)+" ");
                else
                    TextArea.append("\n"+count_of_moves+". "+(char)((char)selected.y+'a')+""+(selected.x+1)+"-"+(char)((char)pos.y+'a')+""+(pos.x+1)+" ");
            }

        if(pos.x == 7 && desk[pos.x][pos.y] == W_PAWN)
            desk[pos.x][pos.y] = W_QUEEN;
        if(pos.x == 0 && desk[pos.x][pos.y] == B_PAWN)
            desk[pos.x][pos.y] = B_QUEEN;
    }
    private void remove_figure(Position pos) // done
    {
       if(desk[pos.x][pos.y] != 0 && desk[pos.x][pos.y]%2 == WHITE)
           white_count--;
       else if(desk[pos.x][pos.y] != 0 && ((desk[pos.x][pos.y]+1)%2)+1 == BLACK)
           black_count--;
       
       desk[pos.x][pos.y] = 0;
    }
    private void make_move(Position pos) // done
    {
        desk[pos.x][pos.y] = desk[selected.x][selected.y];
        desk[selected.x][selected.y] = 0;
        if(on_move == WHITE)
            on_move = BLACK;
        else
            on_move = WHITE;
    }
    
    
    public List<Position> canMove(Position pos, boolean check)
    {
        List<Position> canMove_list = new ArrayList(); 
        ArrayList<list_item> tmp_list = new ArrayList();
        boolean jumper = false;
        
        
        if(desk[pos.x][pos.y] == W_PAWN)
        {
            for(int i=1;i<2;i++)
            {
                tmp_list.add(new list_item());
                if(pos.x+i < 8 && pos.y+i < 8)
                    if(desk[pos.x+i][pos.y+i] == 0)
                    {
                        tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x+i,pos.y+i));
                    }
                    else if(desk[pos.x+i][pos.y+i]%2 == 1) // WHITE - cant jump over own figure
                        break;
                    else if(desk[pos.x+i][pos.y+i]%2 == 0)
                        if(pos.x+i+1 < 8 && pos.y+i+1 < 8)
                            if(desk[pos.x+i+1][pos.y+i+1] == 0)
                            {                               
                                tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x+i+1,pos.y+i+1));
                                tmp_list.get(tmp_list.size()-1).set_take(true);
                                
                                jumper = true;
                                break;
                            }
            }
            for(int i=1;i<2;i++)
            {
                tmp_list.add(new list_item());
                if(pos.x+i < 8 && pos.y-i >= 0)
                    if(desk[pos.x+i][pos.y-i] == 0)
                    {
                        tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x+i,pos.y-i));
                    }
                    else if(desk[pos.x+i][pos.y-i]%2 == 1) // WHITE - cant jump over own figure
                        break;
                    else if(desk[pos.x+i][pos.y-i]%2 == 0)
                        if(pos.x+i+1 < 8 && pos.y-i-1 >=0)
                            if(desk[pos.x+i+1][pos.y-i-1] == 0)
                            {
                                tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x+i+1,pos.y-i-1));
                                tmp_list.get(tmp_list.size()-1).set_take(true);
                                
                                jumper = true;
                                break;
                            }
            }
        }
        if(desk[pos.x][pos.y] == B_PAWN)
        {
            for(int i=1;i<2;i++)
            {
                tmp_list.add(new list_item());
                if(pos.x-i >=0 && pos.y+i < 8)
                    if(desk[pos.x-i][pos.y+i] == 0)
                    {
                        tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x-i,pos.y+i));
                    }
                    else if(desk[pos.x-i][pos.y+i]%2 == 0)
                        break;
                    else if(desk[pos.x-i][pos.y+i]%2 == 1)
                        if(pos.x-i-1 >=0 && pos.y+i+1 < 8)
                            if(desk[pos.x-i-1][pos.y+i+1] == 0)
                            {
                                tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x-i-1,pos.y+i+1));
                                tmp_list.get(tmp_list.size()-1).set_take(true);
                                
                                jumper = true;
                                break;
                            }
            }
            for(int i=1;i<2;i++)
            {
                tmp_list.add(new list_item());
                if(pos.x-i >= 0 && pos.y-i >= 0)
                    if(desk[pos.x-i][pos.y-i] == 0)
                    {
                        tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x-i,pos.y-i));
                    }
                    else if(desk[pos.x-i][pos.y-i]%2 == 0)
                        break;
                    else if(desk[pos.x-i][pos.y-i]%2 == 1)
                        if(pos.x-i-1 >= 0 && pos.y-i-1 >=0)
                            if(desk[pos.x-i-1][pos.y-i-1] == 0)
                            {
                                tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x-i-1,pos.y-i-1));
                                tmp_list.get(tmp_list.size()-1).set_take(true);
                                
                                jumper = true;
                                break;
                            }
            }
        }
                
        if(desk[pos.x][pos.y] == W_QUEEN || desk[pos.x][pos.y] == B_QUEEN)
        {
            for(int i=1;i<8;i++)
            {
                tmp_list.add(new list_item());
                if(pos.x+i < 8 && pos.y+i < 8)
                    if(desk[pos.x+i][pos.y+i] == 0)
                    {
                        tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x+i,pos.y+i));
                    }
                    else if(desk[pos.x+i][pos.y+i]%2 == 1 && desk[pos.x][pos.y] == W_QUEEN) // WHITE - cant jump over own figure
                        break;
                    else if(desk[pos.x+i][pos.y+i]%2 == 0 && desk[pos.x][pos.y] == B_QUEEN) // WHITE - cant jump over own figure
                        break;
                    else
                        if(pos.x+i+1 < 8 && pos.y+i+1 < 8)
                            if(desk[pos.x+i+1][pos.y+i+1] == 0)
                            {                               
                                tmp_list.get(tmp_list.size()-1).clear_list();
                                tmp_list.get(tmp_list.size()-1).set_take(true);
                                
                                for(int j=0;j<6;j++)
                                {
                                    if(pos.x+i+1+j >= 8 || pos.y+i+1+j >= 8)
                                        break;
                                    if(desk[pos.x+i+1+j][pos.y+i+1+j] != 0)
                                        break;
                                    tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x+i+1+j,pos.y+i+1+j));
                                }
                                
                                jumper = true;
                                break;
                            }
                            else
                                break;
            }
            for(int i=1;i<8;i++)
            {
                tmp_list.add(new list_item());
                if(pos.x+i < 8 && pos.y-i >= 0)
                    if(desk[pos.x+i][pos.y-i] == 0)
                    {
                        tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x+i,pos.y-i));
                    }
                    else if(desk[pos.x+i][pos.y-i]%2 == 1 && desk[pos.x][pos.y] == W_QUEEN) // WHITE - cant jump over own figure
                        break;
                    else if(desk[pos.x+i][pos.y-i]%2 == 0 && desk[pos.x][pos.y] == B_QUEEN) // WHITE - cant jump over own figure
                        break;
                    else
                        if(pos.x+i+1 < 8 && pos.y-i-1 >= 0)
                            if(desk[pos.x+i+1][pos.y-i-1] == 0)
                            {                               
                                tmp_list.get(tmp_list.size()-1).clear_list();
                                tmp_list.get(tmp_list.size()-1).set_take(true);
                                
                                for(int j=0;j<6;j++)
                                {
                                    if(pos.x+i+1+j >= 8 || pos.y-i-1-j < 0)
                                        break;
                                    if(desk[pos.x+i+1+j][pos.y-i-1-j] != 0)
                                        break;
                                    tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x+i+1+j,pos.y-i-1-j));
                                }
                                
                                jumper = true;
                                break;
                            }
                            else
                                break;
            }
            for(int i=1;i<8;i++)
            {
                tmp_list.add(new list_item());
                if(pos.x-i >= 0 && pos.y+i < 8)
                    if(desk[pos.x-i][pos.y+i] == 0)
                    {
                        tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x-i,pos.y+i));
                    }
                    else if(desk[pos.x-i][pos.y+i]%2 == 1 && desk[pos.x][pos.y] == W_QUEEN) // WHITE - cant jump over own figure
                        break;
                    else if(desk[pos.x-i][pos.y+i]%2 == 0 && desk[pos.x][pos.y] == B_QUEEN) // WHITE - cant jump over own figure
                        break;
                    else
                        if(pos.x-i-1 >= 0 && pos.y+i+1 < 8)
                            if(desk[pos.x-i-1][pos.y+i+1] == 0)
                            {                               
                                tmp_list.get(tmp_list.size()-1).clear_list();
                                tmp_list.get(tmp_list.size()-1).set_take(true);
                                
                                for(int j=0;j<6;j++)
                                {
                                    if(pos.x-i-1-j < 0 || pos.y+i+1+j >= 8)
                                        break;
                                    if(desk[pos.x-i-1-j][pos.y+i+1+j] != 0)
                                        break;
                                    tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x-i-1-j,pos.y+i+1+j));
                                }
                                
                                jumper = true;
                                break;
                            }
                            else
                                break;
            }
            for(int i=1;i<8;i++)
            {
                tmp_list.add(new list_item());
                if(pos.x-i >= 0 && pos.y-i >= 0)
                    if(desk[pos.x-i][pos.y-i] == 0)
                    {
                        tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x-i,pos.y-i));
                    }
                    else if(desk[pos.x-i][pos.y-i]%2 == 1 && desk[pos.x][pos.y] == W_QUEEN) // WHITE - cant jump over own figure
                        break;
                    else if(desk[pos.x-i][pos.y-i]%2 == 0 && desk[pos.x][pos.y] == B_QUEEN) // WHITE - cant jump over own figure
                        break;
                    else
                        if(pos.x-i-1 >= 0 && pos.y-i-1 >= 0)
                            if(desk[pos.x-i-1][pos.y-i-1] == 0)
                            {                               
                                tmp_list.get(tmp_list.size()-1).clear_list();
                                tmp_list.get(tmp_list.size()-1).set_take(true);
                                
                                for(int j=0;j<6;j++)
                                {
                                    if(pos.x-i-1-j < 0 || pos.y-i-1-j < 0)
                                        break;
                                    if(desk[pos.x-i-1-j][pos.y-i-1-j] != 0)
                                        break;
                                    tmp_list.get(tmp_list.size()-1).add_item(new Position(pos.x-i-1-j,pos.y-i-1-j));
                                }
                                
                                jumper = true;
                                break;
                            }
                            else
                                break;
            }
        }
        
        
        // MAKE THE LIST!
        
        for(int i=0;i<tmp_list.size();i++)
        {
            if(jumper)
            {
                must_take = true;
                if(tmp_list.get(i).get_take())
                {
                    for(int j=0;j<tmp_list.get(i).get_size();j++)
                    {
                        Position tmp_pos = tmp_list.get(i).get_item(j);
                        canMove_list.add(tmp_pos);
                        if(!check)
                        {
                            button_array[tmp_pos.x][tmp_pos.y].setEnabled(true);
                            button_array[tmp_pos.x][tmp_pos.y].setIcon(new ImageIcon("img/selected.png"));
                        }
                    }
                }
            }
            else
            {
                for(int j=0;j<tmp_list.get(i).get_size();j++)
                {
                    Position tmp_pos = tmp_list.get(i).get_item(j);
                    canMove_list.add(tmp_pos);
                    if(!check)
                    {
                        button_array[tmp_pos.x][tmp_pos.y].setEnabled(true);
                        button_array[tmp_pos.x][tmp_pos.y].setIcon(new ImageIcon("img/selected.png"));
                    }
                }
            }
            
        }
        
        return canMove_list;
    }
    
    private List<Position> must_move(int color)
    {
        at_least_one_can_move = false;
        List<Position> queen_list = new ArrayList();
        List<Position> pawn_list = new ArrayList();
        
        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++)
            {
                if(desk[i][j] == 1 && color == WHITE)
                {
                    must_take = false;
                    if(!canMove(new Position(i,j),true).isEmpty())
                        at_least_one_can_move = true;
                    if(must_take)
                        pawn_list.add(new Position(i,j));
                }
                if(desk[i][j] == 3 && color == WHITE)
                {
                    must_take = false;
                    if(!canMove(new Position(i,j),true).isEmpty())
                        at_least_one_can_move = true;
                    if(must_take)
                        queen_list.add(new Position(i,j));
                }
                if(desk[i][j] == 2 && color == BLACK)
                {
                    must_take = false;
                    if(!canMove(new Position(i,j),true).isEmpty())
                        at_least_one_can_move = true;
                    if(must_take)
                        pawn_list.add(new Position(i,j));
                }
                if(desk[i][j] == 4 && color == BLACK)
                {
                    must_take = false;
                    if(!canMove(new Position(i,j),true).isEmpty())
                        at_least_one_can_move = true;
                    if(must_take)
                        queen_list.add(new Position(i,j));
                }
            }
        if(queen_list.isEmpty())
            return pawn_list;
        return queen_list;
    }
    
    public void AI_move(int color) // done
    {
        Random randomGenerator = new Random();
        
        List<Position> pos_list;
        if(color == WHITE)
            pos_list = must_move(WHITE);
        else
            pos_list = must_move(BLACK);
        
        if(!is_pat())
        {
            if(!pos_list.isEmpty())
            {
                int index = randomGenerator.nextInt(pos_list.size());
                selected = pos_list.get(index);
                move(canMove(pos_list.get(index),false).get(0));
                selected = new Position(-1,-1);
            }
            else
            {
                List<Position> move_list = new ArrayList();
                Position figure_to_move = null;

                while(move_list.isEmpty())
                {
                    int figure_id;
                    if(color == WHITE)
                        figure_id = randomGenerator.nextInt(white_count);
                    else
                        figure_id = randomGenerator.nextInt(black_count);
                    int counter = 0;
                    for(int i=0;i<8 && move_list.isEmpty();i++)
                        for(int j=0;j<8;j++)
                            if(((desk[i][j]+1)%2)+1 == color && desk[i][j] != 0)
                            {
                                if(counter == figure_id)
                                {
                                    figure_to_move = new Position(i, j);
                                    move_list = canMove(figure_to_move,true);
                                    break;
                                }
                                counter++;
                            }
                }

                selected = figure_to_move;
                move(move_list.get(0));
                selected = new Position(-1,-1);

            }

            if(!is_win())
            {
                Disable_all_buttons();
                draw_board();
                if(color == WHITE)
                    Enable_black_buttons();
                else
                    Enable_white_buttons();
            }
        }
    }
    
    private boolean is_win() // done
    {
        if(black_count == 0)
        {
            JOptionPane.showMessageDialog(null, "Bílý hráč zvítězil.", "! GRATULACE !", JOptionPane.INFORMATION_MESSAGE);
            Disable_all_buttons();
            return true;
        }
        if(white_count == 0)
        {
            JOptionPane.showMessageDialog(null, "Černý hráč zvítězil.", "! GRATULACE !", JOptionPane.INFORMATION_MESSAGE);
            Disable_all_buttons();
            return true;
        }
        return false;
    }
    
    private boolean is_pat() // done
    {
        if(!at_least_one_can_move)
        {
            JOptionPane.showMessageDialog(null, "Nastala patová situace!", "! PAT !", JOptionPane.INFORMATION_MESSAGE);
            Disable_all_buttons();
            return true;
        }
        return false;
    }
    
    private int str_find(String text, String sub)
    {
        int index;
        for(index=0;index < text.length();index++)
        {
            if(text.charAt(index) == sub.charAt(0))
                return index;
        }
        return -1;
    }
    
    public void wait(int k)
    {
        long time0, time1;
        time0 = System.currentTimeMillis();
        do
        {
            time1 = System.currentTimeMillis();
        }while((time1 - time0) < k);
    }
}
