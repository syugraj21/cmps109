// $Id: client.java,v 1.1 2010-02-23 19:35:28-08 - - $
import java.io.*;
import java.net.*;
import java.util.*;
import static java.lang.System.*;

class client {
  static String username;
  static Socket socket;
  
  client (chatter.options opt){
    try{
      username = opt.username;
      socket = new Socket (opt.server_hostname,opt.server_portnumber);
      Thread reader = new Thread(new receiver(socket));
      Thread writer = new Thread(new sender(socket));
      reader.start();
      writer.start();
    } catch (IOException exn){
      System.err.println("Client Error: " +exn);
    }
  }
  
  /* inner class receiver
   * Thread that recieves messages from  a server
   * 
   */ 
  static class receiver implements Runnable {
    private Socket client;
    receiver (Socket s){
      client = s;
    }
    public void run () {
      try{
        Scanner input = new Scanner(client.getInputStream());
        while (input.hasNextLine()){
          String line = input.nextLine();
          out.printf("%s%n",line);
        }
        input.close();
      } catch (IOException exn){
        System.err.println("Client Error: " + exn); 
      }
    }
  }
  /* inner class sender
   * Thread that sends out messages to the server 
   */ 
  static class sender implements Runnable {
    private Socket client;
    sender (Socket s){
      client = s; 
    }
    public void run () {
      try{
        PrintWriter out = new PrintWriter(client.getOutputStream());
        Scanner user_input = new Scanner(System.in);
        out.printf("%s: %n",username);
        out.flush();
        while (user_input.hasNextLine()){
          String line = user_input.nextLine();
          out.printf ("%s%n",line);
          out.flush();
        }
        user_input.close();
        out.close();
      } catch (IOException exn){
        System.err.println("Client Error: "+exn); 
      }
    }
  }
  
}
