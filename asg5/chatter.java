// $Id: chatter.java,v 1.1 2010-02-23 19:35:28-08 - - $

import static java.lang.System.*;

class chatter {
  /* quit 
   * exits the program , printing message to standerd error
   */ 
  static void quit (String format, Object... params){
    err.printf (format, params);
    exit(1);
  }
  /* get_name()
   * gets the name of the program
   */ 
  static String get_name(){
    String path = getProperty("java.class.path");
    int lastSlash = path.lastIndexOf('/');
    if (lastSlash  < 0) return path;
    return path.substring(lastSlash + 1);
  }
  /* 
   */
  static String ident(options opts){
    return String.format("%s: port %d",opts.prgnm,
                         opts.server_portnumber);
  }
  /* inner class options
   *  
   * Scans the command line arguments and sets the boolean
   * flags for the server/gui 
   */ 
  static class options {
    final String prgnm = get_name();
    String traceflags;
    boolean client_is_gui = false;
    boolean is_server = true; 
    String server_hostname;
    int server_portnumber;
    String username;
    options (String[] args) {
      try{
        if (args.length < 1 || args.length > 4){
          throw new NumberFormatException();
        }
        for (int i = 0; i < args.length; ++i){
          if (args[i].equals("-g")){
            client_is_gui = true;
            is_server = false;
          } else if (args[i].contains("-@")){
            traceflags = args[i];
          } else if (args[i].contains(":")){
            String[] host_port = args[i].split(":",2);
            server_hostname = host_port[0];
            server_portnumber = Integer.parseInt(host_port[1]);
            is_server = false;
          } else {
            if (!is_server) break;
            server_portnumber = Integer.parseInt(args[i]);
            is_server = true;
          }
        }
        username = args[args.length - 1];
      } catch (NumberFormatException exn){
        quit("Usage:%s [-g] [-@flags] [hostname:]port username",prgnm);
      }
    }
  }
  //main 
  public static void main (String[] args) {
    options opts = new options (args);
    if (opts.client_is_gui){
      Thread gui_thread=new Thread(new gui.main_gui_client(opts));
    } else if (opts.is_server) {
      server start_server = new server(opts);
    } else {
      client start_client = new client(opts);
    }
  }
}
