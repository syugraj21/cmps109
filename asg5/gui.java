// $Id: gui.java,v 1.1 2010-02-23 19:35:28-08 - - $

import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import javax.swing.*;
import java.util.*;
import java.util.concurrent.*;

import static java.lang.String.*;
import static java.lang.System.*;


class gui {
  /* inner class main_gui_client
   * Starts the the gui, threads 
   * gui code from examples
   */ 
  static class main_gui_client implements Runnable{
    String username;
    int portnum;
    String pro_name;
    chatter.options opt;
    main_gui_client (chatter.options opts){
      opt = opts;
      username = opts.username;
      portnum = opts.server_portnumber;
      pro_name = opts.prgnm;
    }
    public void run(){
      try{
        Socket client = new Socket(username,portnum);
        Font courier = new Font ("Courier", Font.PLAIN, 12);
        JFrame frame = new JFrame ("textarea"); 
        Container pane = frame.getContentPane ();
        
        GridBagLayout layout = new GridBagLayout ();
        GridBagConstraints constraints = new GridBagConstraints ();
        constraints.anchor = GridBagConstraints.EAST;
        constraints.gridwidth = GridBagConstraints.REMAINDER;
        
        Button quit = new Button ("Quit");
        confirm quitlistener = new confirm ();
        quit.addActionListener (quitlistener);
        
        String spaces = "     ";
        pane.add (quit);
        pane.add (new JLabel (spaces + "Client: " + portnum));
        pane.add (new JLabel (spaces + "Server: " + portnum));
        pane.add (new JLabel (spaces + "User: "+ username));
        JLabel progname = new JLabel (spaces + pro_name);
        layout.setConstraints (progname, constraints);
        pane.add (progname);
        
        JTextArea textarea = new JTextArea (25, 80);
        textarea.setEditable (false);
        textarea.setFont (courier);
        JScrollPane scroll = new JScrollPane (textarea);
        constraints.anchor = GridBagConstraints.WEST;
        layout.setConstraints (scroll, constraints);
        
        JTextField textfield = new JTextField (80);
        textfield.setFont (courier);
        layout.setConstraints (textfield, constraints);
        
        pane.setLayout (layout);
        pane.add (scroll);
        pane.add (textfield);
        
        sender textlistener = new sender (opt,client);
        textfield.addActionListener (textlistener);
        
        frame.pack ();
        textfield.requestFocusInWindow ();
        frame.setLocation (256, 256);
        frame.setVisible (true);
        textarea.append ("Starting...\n");
        
        Thread writer = new Thread (textlistener);
        Thread reader = new Thread (new receiver (client, textarea));
        writer.start();
        reader.start();
      } catch (IOException exn){
        System.err.println("GUI Error: " + exn);
      }
    }
  }
  
  /* inner class confirm
   * closes the gui
   * from examplle
   */ 
  static class confirm implements ActionListener {
    public void actionPerformed (ActionEvent event){
      int response = JOptionPane.showConfirmDialog (null,"Really?"); 
      if (response == JOptionPane.YES_OPTION) exit (0);
    }
  }
  
  /* inner class sender
   * sends to the server
   */ 
  static class sender implements ActionListener, Runnable {
    //JTextArea textarea;
    chatter.options opts;
    PrintWriter output;
    static BlockingQueue <String> messages;
    sender (chatter.options opts, Socket client) {
      try{
        messages = new LinkedBlockingQueue<String>();
        this.opts = opts;
        output = new PrintWriter(client.getOutputStream());
      } catch (IOException exn){
        System.err.println("GUI Error: " + exn); 
      }
    }
    public void actionPerformed (ActionEvent event) {
      JTextField source = (JTextField) event.getSource ();
      String text = source.getText ();
      put(messages,text);
      source.setText ("");
    }
    
    public void run(){
      output.printf ("%s: %n", opts.username);
      output.flush ();
      for(;;) {
        String line = poll (messages, 1000);
        output.printf ("%s%n", line);
        output.flush ();
      } 
    }
  }
  /* reads input from the server
   * 
   */ 
  static class receiver implements Runnable {
    JTextArea textarea;
    Scanner input;
    receiver (Socket client,JTextArea text) {
      try{
        input = new Scanner(client.getInputStream());
        textarea = text;
      } catch (IOException exn){
        System.err.println("GUI Error: " + exn); 
      }
    }
    public void run(){
      while (input.hasNextLine()){
        textarea.append(input.nextLine()+ "\n");
      }
      input.close();
    }
  }
  
  /*static class sender implements Runnable {
   public void run () {
   }
   }*/
  
  static void put (BlockingQueue <String> source, String msg) {
    try {
      source.put (msg);
    }catch (InterruptedException exn) {
      System.err.println("GUI Error: "+exn);
    }
  }
  
  static String poll (BlockingQueue <String> source, long msec) {
    String msg = null;
    
    try {
      msg = source.poll (msec, TimeUnit.MILLISECONDS);
    }catch (InterruptedException exn) {
      msg = null;  
    }
    return msg;
  }
}


