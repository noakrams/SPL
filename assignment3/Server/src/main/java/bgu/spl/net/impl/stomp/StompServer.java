package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.newsfeed.NewsFeed;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {//args[0] port. args[1] tcp/reactor
        DataBase dataBase = DataBase.getInstance();
// you can use any server...
        if (args[1].equals("TPC")) {
            Server server = Server.threadPerClient(Integer.parseInt(args[0]), () -> new StompMessagingProtocolImpl(dataBase), () -> new MessageEncoderDecoderImpl() {
            });
            server.serve();
        } else if (args[1].equals("REACTOR")) {
            Server.reactor(Runtime.getRuntime().availableProcessors(), 7777, //port
                    () -> new StompMessagingProtocolImpl(dataBase), //protocol factory
                    () -> new MessageEncoderDecoderImpl() //message encoder decoder factory
            ).serve();
        }
    }
}
